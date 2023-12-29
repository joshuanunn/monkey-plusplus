#include "vm.hpp"

VM::VM(std::shared_ptr<Bytecode>&& bytecode) : frames{}, stack{}, globals{} {
    // Create a main frame to contain the bytecode instructions
    auto main_fn = std::make_shared<CompiledFunction>(CompiledFunction(bytecode->instructions));
    auto main_frame = new_frame(main_fn);

    // Place main frame at first frame index and set frames_index at one above this
    frames[0] = main_frame;
    frames_index = 1;

    constants = std::move(bytecode->constants);

    // Stack pointer starts at 0
    sp = 0;
}

VM::VM(std::shared_ptr<Bytecode>&& bytecode, std::array<std::shared_ptr<Object>, GLOBALSSIZE> s) : frames{}, stack{}, globals{} {
    // Create a main frame to contain the bytecode instructions
    auto main_fn = std::make_shared<CompiledFunction>(CompiledFunction(bytecode->instructions));
    auto main_frame = new_frame(main_fn);

    // Place main frame at first frame index and set frames_index at one above this
    frames[0] = main_frame;
    frames_index = 1;

    constants = std::move(bytecode->constants);

    // Copy globals from provided source to destination
    std::copy(std::begin(s), std::end(s), std::begin(globals));

    // Stack pointer starts at 0
    sp = 0;
}

std::shared_ptr<Frame> VM::current_frame() {
    return frames[frames_index-1];
}

void VM::push_frame(std::shared_ptr<Frame> f) {
    // TODO: check that frames_index does not excced MAXFRAMES

    // Push frame on to top of stack frame (frames[frames_index]) and increment stack frame pointer
    frames[frames_index++] = f;
}

std::shared_ptr<Frame> VM::pop_frame() {
    // Pop frame on top of stack frame (frames[frames_index-1]) and decrement stack frame pointer
    return frames[--frames_index];
}

std::shared_ptr<Error> VM::push(std::shared_ptr<Object> o) {
    if (sp >= STACKSIZE) {
        std::cerr << ("fatal error: stack overflow") << std::endl;
        throw std::exception();
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

    auto left_type = left->type();
    auto right_type = right->type();

    if (left_type == ObjectType::INTEGER_OBJ && right_type == ObjectType::INTEGER_OBJ) {
        return execute_binary_integer_operation(op, left, right);
    } else if (left_type == ObjectType::STRING_OBJ && right_type == ObjectType::STRING_OBJ) {
        return execute_binary_string_operation(op, left, right);
    } else {
        return new_error("unsupported types for binary operation: " +
            objecttype_literal(left->type()) + " " + objecttype_literal(right->type()));
    }
}

std::shared_ptr<Error> VM::execute_comparison(OpType op) {
    // Pop operand values from stack
    auto right = pop();
    auto left = pop();

    // Defer to execute_integer_comparison if both operands are Integers
    if (left->type() == ObjectType::INTEGER_OBJ && right->type() == ObjectType::INTEGER_OBJ) {
        return execute_integer_comparison(op, left, right);
    }

    // Otherwise cast to Boolean and compare pointers of global True/False Objects
    auto left_bool = std::dynamic_pointer_cast<Boolean>(left);
    auto right_bool = std::dynamic_pointer_cast<Boolean>(right);

    if (op == OpType::OpEqual) {
        return push(native_bool_to_boolean_object(right_bool == left_bool));
    } else if (op == OpType::OpNotEqual) {
        return push(native_bool_to_boolean_object(right_bool != left_bool));
    } else {
        return new_error("unknown operator: " + std::to_string(as_opcode(op)));
    }
}

std::shared_ptr<Error> VM::execute_integer_comparison(OpType op, std::shared_ptr<Object> left, std::shared_ptr<Object> right) {
    auto left_value = std::dynamic_pointer_cast<Integer>(left)->value;
    auto right_value = std::dynamic_pointer_cast<Integer>(right)->value;

    if (op == OpType::OpEqual) {
        return push(native_bool_to_boolean_object(right_value == left_value));
    } else if (op == OpType::OpNotEqual) {
        return push(native_bool_to_boolean_object(right_value != left_value));
    } else if (op == OpType::OpGreaterThan) {
        return push(native_bool_to_boolean_object(left_value > right_value));
    } else {
        return new_error("unknown operator: " + std::to_string(as_opcode(op)));
    }
}

std::shared_ptr<Error> VM::execute_binary_integer_operation(OpType op, std::shared_ptr<Object> left, std::shared_ptr<Object> right) {
    auto left_value = std::dynamic_pointer_cast<Integer>(left)->value;
    auto right_value = std::dynamic_pointer_cast<Integer>(right)->value;

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
        return new_error("unknown integer operator: " + std::to_string(as_opcode(op)));
    }

    // Push result back onto stack
    return push(std::make_shared<Integer>(Integer(result)));
}

std::shared_ptr<Error> VM::execute_binary_string_operation(OpType op, std::shared_ptr<Object> left, std::shared_ptr<Object> right) {
    if (op != OpType::OpAdd) {
        return new_error("unknown string operator: " + std::to_string(as_opcode(op)));
    }

    auto left_value = std::dynamic_pointer_cast<String>(left)->value;
    auto right_value = std::dynamic_pointer_cast<String>(right)->value;

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

    if (operand->type() != ObjectType::INTEGER_OBJ) {
        return new_error("unsupported type for negation: " + objecttype_literal(operand->type()));
    }

    auto value = std::dynamic_pointer_cast<Integer>(operand)->value;

    return push(std::make_shared<Integer>(Integer(-value)));
}

std::shared_ptr<Error> VM::execute_index_expression(std::shared_ptr<Object> left, std::shared_ptr<Object> index) {
    // Try Array indexing (expecting Array and Integer index) and Hash indexing
    if (left->type() == ObjectType::ARRAY_OBJ && index->type() == ObjectType::INTEGER_OBJ) {
        return execute_array_index(left, index);
    } else if (left->type() == ObjectType::HASH_OBJ) {
        return execute_hash_index(left, index);
    } else {
        return new_error("index operator not supported: " + objecttype_literal(left->type()));
    }
}

std::shared_ptr<Error> VM::execute_array_index(std::shared_ptr<Object> array, std::shared_ptr<Object> index) {
    auto array_obj = std::dynamic_pointer_cast<Array>(array);
    auto i = std::dynamic_pointer_cast<Integer>(index)->value;

    auto max = (int) array_obj->elements.size() - 1;

    if (i < 0 || i > max) {
        return push(get_null_ref());
    }

    return push(array_obj->elements.at(i));
}

std::shared_ptr<Error> VM::execute_hash_index(std::shared_ptr<Object> hash, std::shared_ptr<Object> index) {
    auto hash_obj = std::dynamic_pointer_cast<Hash>(hash);

    auto key = std::dynamic_pointer_cast<Hashable>(index);
    if (!key) {
        return new_error("unusable as hash key: " + objecttype_literal(index->type()));
    }

    auto contains = hash_obj->pairs.find(key->hash_key());

    if (contains == hash_obj->pairs.end()) {
        return push(get_null_ref());
    }

    auto pair = hash_obj->pairs[key->hash_key()];

    return push(pair.value);
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
            return std::make_tuple(nullptr, new_error("unusable as hash key."));
        }

        auto hashed = hash_key->hash_key();
        hashed_pairs[hashed] = HashPair{key, value};
    }

    return std::make_tuple(std::make_shared<Hash>(Hash{hashed_pairs}), nullptr);
}

std::shared_ptr<Error> VM::run() {
    int ip;
    Instructions ins;
    OpType op;

    while (current_frame()->ip < static_cast<int>(current_frame()->instructions().size())-1) {
        // Increment instruction pointer within current stack frame
        ip = ++(current_frame()->ip);

        // Fetch current instruction within current stack frame
        ins = current_frame()->instructions();
        op = static_cast<OpType>(ins.at(ip));

        if (op == OpType::OpConstant) {
            auto const_index = read_uint_16(ins.at(ip+1), ins.at(ip+2));
            current_frame()->ip += 2;

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
            auto pos = (int) read_uint_16(ins.at(ip+1), ins.at(ip+2));
            // Skip two bytes of operand associated with conditional jump
            current_frame()->ip += 2;
            // Pop stack top (condition). If not truthy then jump to target, else execute consequence
            auto condition = pop();
            if (!is_truthy(condition)) {
                // Set instruction pointer to (jump target - 1), as ip is incremented on next iteration
                current_frame()->ip = pos - 1;
            }
        } else if (op == OpType::OpJump) {
            // Read jump target into pos
            auto pos = (int) read_uint_16(ins.at(ip+1), ins.at(ip+2));
            // Set instruction pointer to (jump target - 1), as ip is incremented on next iteration
            current_frame()->ip = pos - 1;
        } else if (op == OpType::OpNull) {
            auto err = push(get_null_ref());
            if (err) {
                return err;
            }
        } else if (op == OpType::OpSetGlobal) {
            auto global_index = read_uint_16(ins.at(ip+1), ins.at(ip+2));
            current_frame()->ip += 2;

            globals[global_index] = pop();
        } else if (op == OpType::OpGetGlobal) {
            auto global_index = read_uint_16(ins.at(ip+1), ins.at(ip+2));
            current_frame()->ip += 2;

            auto err = push(globals[global_index]);
            if (err) {
                return err;
            }
        } else if (op == OpType::OpArray) {
            auto num_elements = read_uint_16(ins.at(ip+1), ins.at(ip+2));
            current_frame()->ip += 2;

            auto array = build_array(sp - num_elements, sp);
            sp -= num_elements;

            auto err = push(array);
            if (err) {
                return err;
            }
        } else if (op == OpType::OpHash) {
            auto num_elements = read_uint_16(ins.at(ip+1), ins.at(ip+2));
            current_frame()->ip += 2;

            auto [hash, err] = build_hash(sp - num_elements, sp);
            if (err) {
                return err;
            }
            sp -= num_elements;

            err = push(hash);
            if (err) {
                return err;
            }
        } else if (op == OpType::OpIndex) {
            auto index = pop();
            auto left = pop();

            auto err = execute_index_expression(left, index);
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
    if (obj->type() == ObjectType::BOOLEAN_OBJ) {
        return std::dynamic_pointer_cast<Boolean>(obj)->value;
    } else if (obj == get_null_ref()) {
        return false;
    }

    return true;
}
