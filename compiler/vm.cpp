#include "vm.hpp"

VM::VM(std::shared_ptr<Bytecode>&& bytecode) : stack{}, globals{} {
    instructions = std::move(bytecode->instructions);
    constants = std::move(bytecode->constants);

    // Stack pointer starts at 0
    sp = 0;
}

VM::VM(std::shared_ptr<Bytecode>&& bytecode, std::array<std::shared_ptr<Object>, GLOBALSSIZE> s) : stack{}, globals{} {
    instructions = std::move(bytecode->instructions);
    constants = std::move(bytecode->constants);

    // Copy globals from provided source to destination
    std::copy(std::begin(s), std::end(s), std::begin(globals));

    // Stack pointer starts at 0
    sp = 0;
}

std::shared_ptr<Error> VM::push(std::shared_ptr<Object> o) {
    if (sp >= STACKSIZE) {
        return std::make_shared<Error>(Error("stack overflow"));
    }

    // Push object on to top of stack (stack[sp]) and increment stack pointer
    stack[sp++] = o;

    return nullptr;
}

std::shared_ptr<Object> VM::pop() {
    // Pop object on top of stack (stack[sp-1]) and decrement stack pointer
    return stack[--sp];
}

std::shared_ptr<Object> VM::last_popped_stack_elem() {
    return stack[sp];
}

std::shared_ptr<Error> VM::execute_binary_operation(OpType op) {
    // Pop operand values from stack
    auto right = pop();
    auto left = pop();

    // Cast to Integer objects and only proceed if both objects are Integers
    auto left_integer = std::dynamic_pointer_cast<Integer>(left);
    auto right_integer = std::dynamic_pointer_cast<Integer>(right);

    if (left_integer && right_integer) {
        return execute_binary_integer_operation(op, left_integer, right_integer);
    }

    // Cast to String objects and only proceed if both objects are Strings
    auto left_string = std::dynamic_pointer_cast<String>(left);
    auto right_string = std::dynamic_pointer_cast<String>(right);

    if (left_string && right_string) {
        return execute_binary_string_operation(op, left_string, right_string);
    }

    return std::make_shared<Error>(Error("unsupported types for binary operation"));
}

std::shared_ptr<Error> VM::execute_comparison(OpType op) {
    // Pop operand values from stack
    auto right = pop();
    auto left = pop();

    // Try to cast to Integers and defer to execute_integer_comparison if both Integers
    auto left_integer = std::dynamic_pointer_cast<Integer>(left);
    auto right_integer = std::dynamic_pointer_cast<Integer>(right);

    if (left_integer && right_integer) {
        return execute_integer_comparison(op, left_integer, right_integer);
    }

    // Otherwise cast to Boolean and compare pointers of global True/False Objects
    auto left_bool = std::dynamic_pointer_cast<Boolean>(left);
    auto right_bool = std::dynamic_pointer_cast<Boolean>(right);

    if (op == OpType::OpEqual) {
        return push(native_bool_to_boolean_object(right_bool == left_bool));
    } else if (op == OpType::OpNotEqual) {
        return push(native_bool_to_boolean_object(right_bool != left_bool));
    } else {
        return std::make_shared<Error>(Error("unknown operator: " + std::to_string(as_opcode(op))));
    }
}

std::shared_ptr<Error> VM::execute_integer_comparison(OpType op, std::shared_ptr<Integer> left, std::shared_ptr<Integer> right) {
    auto left_value = left->value;
    auto right_value = right->value;

    if (op == OpType::OpEqual) {
        return push(native_bool_to_boolean_object(right_value == left_value));
    } else if (op == OpType::OpNotEqual) {
        return push(native_bool_to_boolean_object(right_value != left_value));
    } else if (op == OpType::OpGreaterThan) {
        return push(native_bool_to_boolean_object(left_value > right_value));
    } else {
        return std::make_shared<Error>(Error("unknown operator: " + std::to_string(as_opcode(op))));
    }
}

std::shared_ptr<Error> VM::execute_binary_integer_operation(OpType op, std::shared_ptr<Integer> left, std::shared_ptr<Integer> right) {
    auto left_value = left->value;
    auto right_value = right->value;

    int result;

    if (op == OpType::OpAdd) {
        result = left_value + right_value;
    } else if (op == OpType::OpSub) {
        result = left_value - right_value;
    } else if (op == OpType::OpMul) {
        result = left_value * right_value;
    } else if (op == OpType::OpDiv) {
        result = left_value / right_value;
    } else {
        return std::make_shared<Error>(Error("unknown integer operator: " + std::to_string(as_opcode(op))));
    }

    // Push result back onto stack
    return push(std::make_shared<Integer>(Integer(result)));
}

std::shared_ptr<Error> VM::execute_binary_string_operation(OpType op, std::shared_ptr<String> left, std::shared_ptr<String> right) {
    if (op != OpType::OpAdd) {
        return std::make_shared<Error>(Error("unknown string operator: " + std::to_string(as_opcode(op))));
    }

    auto left_value = left->value;
    auto right_value = right->value;

    // Push result back onto stack
    return push(std::make_shared<String>(String(left_value + right_value)));
}

std::shared_ptr<Error> VM::execute_bang_operator() {
    auto operand = pop();

    if (operand == get_true_ref()) {
        return push(get_false_ref());
    } else if (operand == get_false_ref()) {
        return push(get_true_ref());
    } else if (operand == get_null_ref()) {
        return push(get_true_ref());
    } else {
        return push(get_false_ref());
    }
}

std::shared_ptr<Error> VM::execute_minus_operator() {
    auto operand = pop();

    // Cast to Integer and extract current value
    auto integer_obj = std::dynamic_pointer_cast<Integer>(operand);
    if (!integer_obj) {
        return std::make_shared<Error>(Error("unsupported type for negation"));
    }
    auto value = integer_obj->value;

    return push(std::make_shared<Integer>(Integer(-value)));
}

std::shared_ptr<Object> VM::build_array(int start_index, int end_index) {    
    auto array = std::make_shared<Array>(Array{});

    for (int i = start_index; i < end_index; i++) {
        array->elements.push_back(stack[i]);
    }

    return array;
}

std::tuple<std::shared_ptr<Object>, std::shared_ptr<Error>> VM::build_hash(int start_index, int end_index) {
    std::map<HashKey, HashPair> hashed_pairs;

    for (int i = start_index; i < end_index; i+=2) {
        auto key = stack[i];
        auto value = stack[i+1];

        auto hash_key = std::dynamic_pointer_cast<Hashable>(key);
        if (!hash_key) {
            return std::make_tuple(nullptr, std::make_shared<Error>(Error("unusable as hash key.")));
        }

        auto hashed = hash_key->hash_key();
        hashed_pairs[hashed] = HashPair{key, value};
    }

    return std::make_tuple(std::make_shared<Hash>(Hash{hashed_pairs}), nullptr);
}

std::shared_ptr<Error> VM::run() {
    for (int ip = 0; ip < instructions.size(); ip++) {
        auto op = static_cast<OpType>(instructions.at(ip));
        if (op == OpType::OpConstant) {
            auto const_index = read_uint_16(instructions.at(ip+1), instructions.at(ip+2));
            ip += 2;

            // Add constant to VM constants
            push(constants.at(const_index));
        // OpPop instruction pops the top element off the stack
        } else if (op == OpType::OpPop) {
            pop();
        // Binary operations operate on two values beneath on stack
        } else if (op == OpType::OpAdd || op == OpType::OpSub || op == OpType::OpMul || op == OpType::OpDiv) {
            auto err = execute_binary_operation(op);
            if (err) {
                return err;
            }
        } else if (op == OpType::OpTrue) {
            auto err = push(get_true_ref());
            if (err) {
                return err;
            }
        } else if (op == OpType::OpFalse) {
            auto err = push(get_false_ref());
            if (err) {
                return err;
            }
        } else if (op == OpType::OpEqual || op == OpType::OpNotEqual || op == OpType::OpGreaterThan) {
            auto err = execute_comparison(op);
            if (err) {
                return err;
            }
        } else if (op == OpType::OpBang) {
            auto err = execute_bang_operator();
            if (err) {
                return err;
            }
        } else if (op == OpType::OpMinus) {
            auto err = execute_minus_operator();
            if (err) {
                return err;
            }
        } else if (op == OpType::OpJumpNotTruthy) {
            // Read jump target into pos
            auto pos = (int) read_uint_16(instructions.at(ip+1), instructions.at(ip+2));
            // Skip two bytes of operand associated with conditional jump
            ip += 2;
            // Pop stack top (condition). If not truthy then jump to target, else execute consequence
            auto condition = pop();
            if (!is_truthy(condition)) {
                // Set instruction pointer to (jump target - 1), as ip is incremented on next iteration
                ip = pos - 1;
            }
        } else if (op == OpType::OpJump) {
            // Read jump target into pos
            auto pos = (int) read_uint_16(instructions.at(ip+1), instructions.at(ip+2));
            // Set instruction pointer to (jump target - 1), as ip is incremented on next iteration
            ip = pos - 1;
        } else if (op == OpType::OpNull) {
            auto err = push(get_null_ref());
            if (err) {
                return err;
            }
        } else if (op == OpType::OpSetGlobal) {
            auto global_index = read_uint_16(instructions.at(ip+1), instructions.at(ip+2));
            ip += 2;

            globals[global_index] = pop();
        } else if (op == OpType::OpGetGlobal) {
            auto global_index = read_uint_16(instructions.at(ip+1), instructions.at(ip+2));
            ip += 2;

            auto err = push(globals[global_index]);
            if (err) {
                return err;
            }
        } else if (op == OpType::OpArray) {
            auto num_elements = read_uint_16(instructions.at(ip+1), instructions.at(ip+2));
            ip += 2;

            auto array = build_array(sp - num_elements, sp);
            sp -= num_elements;

            auto err = push(array);
            if (err) {
                return err;
            }
        } else if (op == OpType::OpHash) {
            auto num_elements = read_uint_16(instructions.at(ip+1), instructions.at(ip+2));
            ip += 2;

            auto [hash, err] = build_hash(sp - num_elements, sp);
            if (err) {
                return err;
            }
            sp -= num_elements;

            err = push(hash);
            if (err) {
                return err;
            }
        }
    }

    return nullptr; // TODO: should we instead return an Error?
}

std::shared_ptr<Boolean> native_bool_to_boolean_object(bool input) {
    if (input) {
        return get_true_ref();
    }
    return get_false_ref();
}

bool is_truthy(std::shared_ptr<Object> obj) {
    if (auto b = std::dynamic_pointer_cast<Boolean>(obj)) {
        return b->value;
    } else if (auto n = std::dynamic_pointer_cast<Null>(obj)) {
        return false;
    }

    return true;
}
