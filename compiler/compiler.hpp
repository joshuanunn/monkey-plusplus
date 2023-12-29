#ifndef MONKEY_PLUSPLUS_COMPILER_HPP
#define MONKEY_PLUSPLUS_COMPILER_HPP

#include "ast.hpp"
#include "code.hpp"
#include "object.hpp"
#include "symbol_table.hpp"

struct Bytecode {
    Bytecode() = default;

    Bytecode(const Bytecode& other) = default;

    Bytecode(Bytecode&& other) noexcept = default;

    Bytecode& operator=(const Bytecode& other) = default;

    Bytecode& operator=(Bytecode&& other) noexcept = default;

    Instructions instructions;

    std::vector<std::shared_ptr<Object>> constants;
};

struct EmittedInstruction {
    OpType opcode;
    int position;
};

struct CompilationScope {
    Instructions instructions;
    EmittedInstruction last_instruction;
    EmittedInstruction previous_instruction;
};

struct Compiler {
    Compiler() = default;

    Compiler(const Compiler& other) = default;

    Compiler(Compiler&& other) noexcept = default;

    Compiler& operator=(const Compiler& other) = default;

    Compiler& operator=(Compiler&& other) noexcept = default;

    std::vector<std::shared_ptr<Object>> constants;

    std::shared_ptr<SymbolTable> symbol_table;

    std::vector<CompilationScope> scopes;

    int scope_index;

    std::shared_ptr<Error> compile(std::shared_ptr<Node> node);

    int add_constant(std::shared_ptr<Object> obj);

    int emit(OpType op);

    int emit(OpType op, std::vector<int> operands);

    int add_instruction(Instructions ins);

    void set_last_instruction(OpType op, int pos);

    bool last_instruction_is(OpType op) const;

    void remove_last_pop();

    void replace_last_pop_with_return();

    void replace_instruction(int pos, Instructions new_instruction);

    void change_operand(int op_pos, int operand);

    void enter_scope();

    Instructions leave_scope();

    std::shared_ptr<Bytecode> bytecode();
};

std::shared_ptr<Compiler> new_compiler();

std::shared_ptr<Compiler> new_compiler_with_state(
    std::shared_ptr<SymbolTable> s, std::vector<std::shared_ptr<Object>> constants);

#endif //MONKEY_PLUSPLUS_COMPILER_HPP
