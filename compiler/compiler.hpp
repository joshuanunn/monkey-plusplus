#ifndef MONKEY_PLUSPLUS_COMPILER_HPP
#define MONKEY_PLUSPLUS_COMPILER_HPP

#include "ast.hpp"
#include "code.hpp"
#include "object.hpp"

struct Bytecode {
    Bytecode() = default;

    Bytecode(const Bytecode& other) = default;

    Bytecode(Bytecode&& other) noexcept = default;

    Bytecode& operator=(const Bytecode& other) = default;

    Bytecode& operator=(Bytecode&& other) noexcept = default;

    Instructions instructions;

    std::vector<std::shared_ptr<Object>> constants;
};

struct Compiler {
    Compiler() = default;

    Compiler(const Compiler& other) = default;

    Compiler(Compiler&& other) noexcept = default;

    Compiler& operator=(const Compiler& other) = default;

    Compiler& operator=(Compiler&& other) noexcept = default;

    Instructions instructions;

    std::vector<std::shared_ptr<Object>> constants;

    std::shared_ptr<Error> compile(std::shared_ptr<Node> node);

    int add_constant(std::shared_ptr<Object> obj);

    int emit(OpType op, std::vector<int> operands);

    int add_instruction(Instructions ins);

    std::shared_ptr<Bytecode> bytecode();
};

std::shared_ptr<Compiler> new_compiler();

// TODO: this is copied from interpreter/evaluator, if linked at some point then can remove
bool is_error(const std::shared_ptr<Object>& obj);

#endif //MONKEY_PLUSPLUS_COMPILER_HPP
