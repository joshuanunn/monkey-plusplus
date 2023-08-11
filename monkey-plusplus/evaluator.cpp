#include "evaluator.hpp"

std::shared_ptr<Object> Eval(const std::shared_ptr<Node> &node) {
    // Statements
    if (auto p = std::dynamic_pointer_cast<Program>(node)) {
        return eval_statements(p->statements);
    } else if (auto e = std::dynamic_pointer_cast<ExpressionStatement>(node)) {
        return Eval(e->expression);
    // Expressions
    } else if (auto il = std::dynamic_pointer_cast<IntegerLiteral>(node)) {
        return std::make_shared<Integer>(Integer{il->value});
    }

    return nullptr;
}

std::shared_ptr<Object> eval_statements(const std::vector<std::shared_ptr<Node>> &stmts) {
    std::shared_ptr<Object> result;

    for (const auto &statement: stmts) {
        result = Eval(statement);
    }

    return result;
}
