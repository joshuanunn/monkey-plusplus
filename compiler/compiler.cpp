#include "compiler.hpp"

std::shared_ptr<Error> Compiler::compile(std::shared_ptr<Node> node)
{
    std::shared_ptr<Error> err;

    // Statements
    if (auto p = std::dynamic_pointer_cast<Program>(node)) {
        for (const auto& s: p->statements) {
            err = compile(s);
            if (is_error(err)) {
                return err;
            }
        }
    // Expression Statement
    } else if (auto e = std::dynamic_pointer_cast<ExpressionStatement>(node)) {
        err = compile(e->expression);
        if (is_error(err)) {
            return err;
        }
        emit(OpType::OpPop);
    // Infix Expression
    } else if (auto ie = std::dynamic_pointer_cast<InfixExpression>(node)) {
        // Initial catch for less-than operator, so code can be reordered to greater-than
        if (ie->op == "<") {
            // Extract operands in reverse (right then left)
            err = compile(ie->right);
            if (is_error(err)) {
                return err;
            }
            err = compile(ie->left);
            if (is_error(err)) {
                return err;
            }
            // Emit greater-than instruction, as operands have been read in reverse
            emit(OpType::OpGreaterThan);
            return nullptr;
        }

        // Extract left operand
        err = compile(ie->left);
        if (is_error(err)) {
            return err;
        }

        // Extract right operand
        err = compile(ie->right);
        if (is_error(err)) {
            return err;
        }

        // Extract operator
        if (ie->op == "+") {
            emit(OpType::OpAdd);
        } else if (ie->op == "-") {
            emit(OpType::OpSub);
        } else if (ie->op == "*") {
            emit(OpType::OpMul);
        } else if (ie->op == "/") {
            emit(OpType::OpDiv);
        } else if (ie->op == ">") {
            emit(OpType::OpGreaterThan);
        } else if (ie->op == "==") {
            emit(OpType::OpEqual);
        } else if (ie->op == "!=") {
            emit(OpType::OpNotEqual);
        } else {
            return new_error("unknown operator " + ie->op);
        }
    // Integer
    } else if (auto il = std::dynamic_pointer_cast<IntegerLiteral>(node)) {
        auto integer = std::make_shared<Integer>(Integer{il->value});
        auto position = add_constant(integer);
        emit(OpType::OpConstant, position);
    // Boolean
    } else if (auto bl = std::dynamic_pointer_cast<BooleanLiteral>(node)) {
        if (bl->value) {
            emit(OpType::OpTrue);
        } else {
            emit(OpType::OpFalse);
        }
    // Prefix Expression
    } else if (auto pe = std::dynamic_pointer_cast<PrefixExpression>(node)) {
        // Extract right operand (to be operated on)
        err = compile(pe->right);
        if (is_error(err)) {
            return err;
        }

        if (pe->op == "!") {
            emit(OpType::OpBang);
        } else if (pe->op == "-") {
            emit(OpType::OpMinus);
        } else {
            return new_error("unknown operator " + ie->op);
        }
    // If Expression
    } else if (auto i = std::dynamic_pointer_cast<IfExpression>(node)) {
        err = compile(i->condition);
        if (is_error(err)) {
            return err;
        }

        // Emit conditional jump instruction with placeholder value (replaced below)
        auto jump_not_truthy_pos = emit(OpType::OpJumpNotTruthy, 9999);

        err = compile(i->consequence);
        if (is_error(err)) {
            return err;
        }

        // Remove last pop instruction to keep last expression result on the stack
        if (last_instruction_is(OpType::OpPop)) {
            remove_last_pop();
        }

        // Emit jump instruction with placeholder value (replaced below)
        auto jump_pos = emit(OpType::OpJump, 9999);

        // Update placeholder value of conditional jump instruction to actual value
        auto after_consequence_pos = (int) scopes.at(scope_index).instructions.size();
        change_operand(jump_not_truthy_pos, after_consequence_pos);

        // If no alternative, emit a OpNull instruction, else compile alternative
        if (!i->alternative) {
            emit(OpType::OpNull);
        } else {
            err = compile(i->alternative);
            if (is_error(err)) {
                return err;
            }

            // Remove last pop instruction to keep last expression result on the stack
            if (last_instruction_is(OpType::OpPop)) {
                remove_last_pop();
            }
        }

        // Update placeholder value of jump instruction to actual value
        auto after_alternative_pos = (int) scopes.at(scope_index).instructions.size();
        change_operand(jump_pos, after_alternative_pos);
    // Block Statement
    } else if (auto b = std::dynamic_pointer_cast<BlockStatement>(node)) {
        for (auto const& s : b->statements) {
            err = compile(s);
            if (is_error(err)) {
                return err;
            }
        }
    // Let Statement
    } else if (auto l = std::dynamic_pointer_cast<LetStatement>(node)) {
        err = compile(l->value);
        if (is_error(err)) {
            return err;
        }
        auto symbol = symbol_table->define(l->name->value);
        emit(OpType::OpSetGlobal, symbol.index);
    // Identifier
    } else if (auto id = std::dynamic_pointer_cast<Identifier>(node)) {
        auto [symbol, ok] = symbol_table->resolve(id->value);
        if (!ok) {
            return new_error("undefined variable " + id->value);
        }
        emit(OpType::OpGetGlobal, symbol.index);
    // String Literal
    } else if (auto sl = std::dynamic_pointer_cast<StringLiteral>(node)) {
        auto str = std::make_shared<String>(String{sl->value});
        emit(OpType::OpConstant, add_constant(str));
    // Array Literal
    } else if (auto a = std::dynamic_pointer_cast<ArrayLiteral>(node)) {
        for (auto const& el: a->elements) {
            err = compile(el);
            if (is_error(err)) {
                return err;
            }
        }

        emit(OpType::OpArray, (int) a->elements.size());
    // Hash Literal
    } else if (auto hl = std::dynamic_pointer_cast<HashLiteral>(node)) {
        for (auto const& kv: hl->pairs) {
            err = compile(kv.first);
            if (is_error(err)) {
                return err;
            }
            err = compile(kv.second);
            if (is_error(err)) {
                return err;
            }
        }

        int key_value_count = hl->pairs.size() * 2;

        emit(OpType::OpHash, key_value_count);
    // Index Expression
    } else if (auto ix = std::dynamic_pointer_cast<IndexExpression>(node)) {
        err = compile(ix->left);
        if (is_error(err)) {
            return err;
        }

        err = compile(ix->index);
        if (is_error(err)) {
            return err;
        }

        emit(OpType::OpIndex);
    // Function Literals
    } else if (auto f = std::dynamic_pointer_cast<FunctionLiteral>(node)) {
        // Enter new scope to compile the function
        enter_scope();

        err = compile(f->body);
        if (is_error(err)) {
            return err;
        }

        // Replace any trailing OpPop instructions in a Functional Literal with OpReturnValue
        if (last_instruction_is(OpType::OpPop)) {
            replace_last_pop_with_return();
        }

        // Append OpReturn instruction where no trailing OpReturnValue (i.e. empty function body)
        if (!last_instruction_is(OpType::OpReturnValue)) {
            emit(OpType::OpReturn);
        }

        // Take the compiled instructions, leave scope, embed into a CompiledFunction and emit
        auto instructions = leave_scope();
        auto compiled_fn = CompiledFunction(instructions).clone();
        emit(OpType::OpConstant, add_constant(compiled_fn));
    // Return Statement
    } else if (auto r = std::dynamic_pointer_cast<ReturnStatement>(node)) {
        err = compile(r->return_value);
        if (is_error(err)) {
            return err;
        }

        emit(OpType::OpReturnValue);
    // Call Expression
    } else if (auto c = std::dynamic_pointer_cast<CallExpression>(node)) {
        err = compile(c->function);
        if (is_error(err)) {
            return err;
        }

        emit(OpType::OpCall);
    }

    return nullptr;
}

std::shared_ptr<Compiler> new_compiler() {
    auto main_scope = CompilationScope{};

    return std::make_shared<Compiler>(
        Compiler{
            symbol_table: new_symbol_table(),
            scopes: std::vector<CompilationScope>{main_scope},
            scope_index: 0
        });
}

std::shared_ptr<Compiler> new_compiler_with_state(
    std::shared_ptr<SymbolTable> s, std::vector<std::shared_ptr<Object>> constants) {

    auto main_scope = CompilationScope{};

    auto compiler = std::make_shared<Compiler>(
        Compiler{
            constants: constants,
            symbol_table: s,
            scopes: std::vector<CompilationScope>{main_scope},
            scope_index: 0
    });

    return compiler;
}

std::shared_ptr<Bytecode> Compiler::bytecode() {
    // Return bytecode for current scope
    return std::make_shared<Bytecode>(
        Bytecode{scopes.at(scope_index).instructions, constants});
}

int Compiler::add_constant(std::shared_ptr<Object> obj) {
    constants.push_back(obj);
    return static_cast<int>(constants.size()) - 1;
}

int Compiler::emit(OpType op) {
    auto ins = make(op);
    auto pos = add_instruction(ins);

    set_last_instruction(op, pos);

    return pos;
}

int Compiler::emit(OpType op, int operand) {
    auto ins = make(op, operand);
    auto pos = add_instruction(ins);

    set_last_instruction(op, pos);

    return pos;
}

int Compiler::add_instruction(Instructions ins) {
    auto pos_new_instruction = static_cast<int>(scopes.at(scope_index).instructions.size());

    // Append new Instructions to end of instructions in current scope
    scopes.at(scope_index).instructions.insert(
        scopes.at(scope_index).instructions.end(), ins.begin(), ins.end());

    return pos_new_instruction;
}

void Compiler::set_last_instruction(OpType op, int pos) {
    auto previous = scopes.at(scope_index).last_instruction;
    auto last = EmittedInstruction{op, pos};

    scopes.at(scope_index).previous_instruction = previous;
    scopes.at(scope_index).last_instruction = last;
}

bool Compiler::last_instruction_is(OpType op) const {
    if (scopes.at(scope_index).instructions.empty()) {
        return false;
    }

    return scopes.at(scope_index).last_instruction.opcode == op;
}

void Compiler::remove_last_pop() {
    // Remove last instruction (OpPop) from instructions
    scopes.at(scope_index).instructions.pop_back();
    scopes.at(scope_index).last_instruction = scopes.at(scope_index).previous_instruction;
}

void Compiler::replace_last_pop_with_return() {
    // Replace last instruction (OpPop) in instructions with an OpReturnValue
    auto last_pos = scopes.at(scope_index).last_instruction.position;
    replace_instruction(last_pos, make(OpType::OpReturnValue));

    // Correct last_instruction
    scopes.at(scope_index).last_instruction.opcode = OpType::OpReturnValue;
}

void Compiler::replace_instruction(int pos, Instructions new_instruction) {
    for (int i = 0; i < new_instruction.size(); i++) {
        scopes.at(scope_index).instructions.at(pos + i) = new_instruction.at(i);
    }
}

void Compiler::change_operand(int op_pos, int first_operand) {
    auto op = OpType(scopes.at(scope_index).instructions.at(op_pos));
    auto new_instruction = make(op, first_operand);

    replace_instruction(op_pos, new_instruction);
}

void Compiler::enter_scope() {
    scopes.push_back(CompilationScope{});
    scope_index++;
}

Instructions Compiler::leave_scope() {
    auto instructions = scopes.at(scope_index).instructions;

    scopes.pop_back();
    scope_index--;

    return instructions;
}
