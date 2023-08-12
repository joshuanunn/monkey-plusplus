#include "evaluator.hpp"

// Initialise global Boolean Objects to avoid unnecessary Object creation
auto TRUE = std::make_shared<Boolean>(Boolean{true});
auto FALSE = std::make_shared<Boolean>(Boolean{false});

std::shared_ptr<Object> eval(const std::shared_ptr<Node> &node) {
    // Statements
    if (auto p = std::dynamic_pointer_cast<Program>(node)) {
        return eval_statements(p->statements);
    } else if (auto e = std::dynamic_pointer_cast<ExpressionStatement>(node)) {
        return eval(e->expression);
    // Expressions
    } else if (auto il = std::dynamic_pointer_cast<IntegerLiteral>(node)) {
        return std::make_shared<Integer>(Integer{il->value});
    } else if (auto bl = std::dynamic_pointer_cast<BooleanLiteral>(node)) {
        return native_bool_to_boolean_object(bl->value);
    }

    return nullptr;
}

std::shared_ptr<Object> eval_statements(const std::vector<std::shared_ptr<Node>> &stmts) {
    std::shared_ptr<Object> result;

    for (const auto &statement: stmts) {
        result = eval(statement);
    }

    return result;
}

std::shared_ptr<Object> native_bool_to_boolean_object(bool input) {
    if (input) {
        return TRUE;
    }
    return FALSE;
}
