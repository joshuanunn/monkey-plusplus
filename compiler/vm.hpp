#ifndef MONKEY_PLUSPLUS_VM_HPP
#define MONKEY_PLUSPLUS_VM_HPP

#include <iostream>
#include "code.hpp"
#include "compiler.hpp"
#include "object.hpp"

constexpr int STACKSIZE = 2048;

struct VM {
    VM() = delete;

    VM(const VM& other) = delete;

    VM(VM&& other) noexcept = delete;

    VM& operator=(const VM& other) = delete;

    VM& operator=(VM&& other) noexcept = delete;

    explicit VM(std::shared_ptr<Bytecode>&& bytecode);

    std::vector<std::shared_ptr<Object>> constants;

    Instructions instructions;

    std::vector<std::shared_ptr<Object>> stack;

    int sp; // Always points to the next value. Top of stack is stack[sp-1]

    void push(std::shared_ptr<Object> o);

    std::shared_ptr<Object> stack_top();

    std::shared_ptr<Error> run();
};

#endif //MONKEY_PLUSPLUS_VM_HPP