#include "compiler.hpp"

std::shared_ptr<Error> Compiler::compile(std::shared_ptr<Node> node) {
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
        emit(OpType::OpPop, std::vector<int>{});
    // Infix Expression
    } else if (auto ie = std::dynamic_pointer_cast<InfixExpression>(node)) {
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
            emit(OpType::OpAdd, std::vector<int>{});
        } else if (ie->op == "-") {
            emit(OpType::OpSub, std::vector<int>{});
        } else if (ie->op == "*") {
            emit(OpType::OpMul, std::vector<int>{});
        } else if (ie->op == "/") {
            emit(OpType::OpDiv, std::vector<int>{});
        } else {
            return std::make_shared<Error>(Error("unknown operator " + ie->op));
        }
    // Integer
    } else if (auto il = std::dynamic_pointer_cast<IntegerLiteral>(node)) {
        auto integer = std::make_shared<Integer>(Integer{il->value});
        auto position = add_constant(integer);
        emit(OpType::OpConstant, std::vector<int>{position});
    // Boolean
    } else if (auto bl = std::dynamic_pointer_cast<BooleanLiteral>(node)) {
        if (bl->value) {
            emit(OpType::OpTrue, std::vector<int>{});
        } else {
            emit(OpType::OpFalse, std::vector<int>{});
        }
    }

    return nullptr;
}

std::shared_ptr<Compiler> new_compiler() {
    return std::make_shared<Compiler>(Compiler{});
}

std::shared_ptr<Bytecode> Compiler::bytecode() {
    return std::make_shared<Bytecode>(Bytecode{instructions, constants});
}

int Compiler::add_constant(std::shared_ptr<Object> obj) {
    constants.push_back(obj);
    return static_cast<int>(constants.size()) - 1;
}

int Compiler::emit(OpType op, std::vector<int> operands) {
    auto ins = make(op, operands);
    auto pos = add_instruction(ins);
    return pos;
}

int Compiler::add_instruction(Instructions ins) {
    auto pos_new_instruction = static_cast<int>(instructions.size());
    for (const auto& i: ins) {
        instructions.push_back(i);
    }
    return pos_new_instruction;
}

bool is_error(const std::shared_ptr<Object>& obj) {
    if (obj) {
        return obj->type() == ObjectType::ERROR_OBJ;
    }
    return false;
}
