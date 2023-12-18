#include "vm.hpp"

VM::VM(std::shared_ptr<Bytecode>&& bytecode) : stack{} {
    instructions = std::move(bytecode->instructions);
    constants = std::move(bytecode->constants);

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

std::shared_ptr<Object> VM::stack_top() {
    if (sp == 0) {
        return nullptr; // TODO: should we instead return the null object?
    }
    // Value at top of stack is 1 less than stack pointer
    return stack[sp-1];
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

    return std::make_shared<Error>(Error("unsupported types for binary operation"));
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
        }
    }

    return nullptr; // TODO: should we instead return an Error?
}
