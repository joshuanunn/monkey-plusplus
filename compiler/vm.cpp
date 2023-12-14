#include "vm.hpp"

VM::VM(std::shared_ptr<Bytecode>&& bytecode) : stack{} {
    instructions = std::move(bytecode->instructions);
    constants = std::move(bytecode->constants);

    // Preallocate the stack to the default STACKSIZE
    stack.reserve(STACKSIZE);

    // Stack pointer starts at 0
    sp = 0;
}

void VM::push(std::shared_ptr<Object> o) {
    if (sp >= STACKSIZE) {
        std::cerr << "stack overflow" << std::endl;
        abort();
    }

    // Push object on to top of stack
    stack.push_back(o);
    // Increment stack pointer
    sp++;
}

std::shared_ptr<Object> VM::pop() {
    auto o = stack.at(sp-1);
    stack.pop_back();
    sp--;
    return o;
}

std::shared_ptr<Object> VM::stack_top() {
    if (sp == 0) {
        return nullptr; // TODO: should we instead return the null object?
    }
    // Value at top of stack is 1 less than stack pointer
    return stack.at(sp-1);
}

std::shared_ptr<Error> VM::run() {
    for (int ip = 0; ip < instructions.size(); ip++) {
        auto op = static_cast<OpType>(instructions.at(ip));
        if (op == OpType::OpConstant) {
            auto const_index = read_uint_16(instructions.at(ip+1), instructions.at(ip+2));
            ip += 2;

            // Add constant to VM constants
            push(constants.at(const_index));
        // OpAdd instruction sums next two values beneath it on stack and pushes result back onto stack
        } else if (op == OpType::OpAdd) {
            // Pop values to be summed
            auto right = pop();
            auto left = pop();

            // Cast to Integer objects and extract int values
            // TODO: perhaps check for dynamic pointer cast failure (i.e. not Integer)?
            auto left_value = std::dynamic_pointer_cast<Integer>(left)->value;
            auto right_value = std::dynamic_pointer_cast<Integer>(right)->value;

            // Push result back onto stack
            auto result = std::make_shared<Integer>(Integer(left_value + right_value));
            push(result);
        }
    }

    return nullptr; // TODO: should we instead return an Error?
}
