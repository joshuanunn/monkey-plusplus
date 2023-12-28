#ifndef MONKEY_PLUSPLUS_VM_HPP
#define MONKEY_PLUSPLUS_VM_HPP

#include <iostream>
#include "code.hpp"
#include "compiler.hpp"
#include "object.hpp"

constexpr int STACKSIZE = 2048;
constexpr int GLOBALSSIZE = 65536;

struct VM {
    VM() = delete;

    VM(const VM& other) = delete;

    VM(VM&& other) noexcept = delete;

    VM& operator=(const VM& other) = delete;

    VM& operator=(VM&& other) noexcept = delete;

    explicit VM(std::shared_ptr<Bytecode>&& bytecode);

    explicit VM(std::shared_ptr<Bytecode>&& bytecode, std::array<std::shared_ptr<Object>, GLOBALSSIZE> s);

    std::vector<std::shared_ptr<Object>> constants;

    Instructions instructions;

    std::array<std::shared_ptr<Object>, STACKSIZE> stack;

    int sp; // Always points to the next value. Top of stack is stack[sp-1]

    std::array<std::shared_ptr<Object>, GLOBALSSIZE> globals;

    std::shared_ptr<Error> push(std::shared_ptr<Object> o);

    std::shared_ptr<Object> pop();

    std::shared_ptr<Object> last_popped_stack_elem();

    std::shared_ptr<Object> build_array(int start_index, int end_index);

    std::tuple<std::shared_ptr<Object>, std::shared_ptr<Error>> build_hash(int start_index, int end_index);

    std::shared_ptr<Error> execute_binary_operation(OpType op);

    std::shared_ptr<Error> execute_comparison(OpType op);

    std::shared_ptr<Error> execute_integer_comparison(OpType op, std::shared_ptr<Integer> left, std::shared_ptr<Integer> right);

    std::shared_ptr<Error> execute_binary_integer_operation(OpType op, std::shared_ptr<Integer> left, std::shared_ptr<Integer> right);

    std::shared_ptr<Error> execute_binary_string_operation(OpType op, std::shared_ptr<String> left, std::shared_ptr<String> right);

    std::shared_ptr<Error> execute_bang_operator();

    std::shared_ptr<Error> execute_minus_operator();

    std::shared_ptr<Error> execute_index_expression(std::shared_ptr<Object> left, std::shared_ptr<Object> index);

    std::shared_ptr<Error> execute_array_index(std::shared_ptr<Object> array, std::shared_ptr<Object> index);

    std::shared_ptr<Error> execute_hash_index(std::shared_ptr<Object> hash, std::shared_ptr<Object> index);

    std::shared_ptr<Error> run();
};

//std::shared_ptr<VM> new_vm_with_globals_store(std::shared_ptr<Bytecode>&& bytecode, std::vector<std::shared_ptr<Object>> s);

std::shared_ptr<Boolean> native_bool_to_boolean_object(bool input);

bool is_truthy(std::shared_ptr<Object> obj);

#endif //MONKEY_PLUSPLUS_VM_HPP
