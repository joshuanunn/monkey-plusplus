#include "evaluator.hpp"

// Initialise global Boolean and Null Objects to avoid unnecessary Object creation
auto GLOBAL_NULL = std::make_shared<Null>(Null{});
auto GLOBAL_TRUE = std::make_shared<Boolean>(Boolean{true});
auto GLOBAL_FALSE = std::make_shared<Boolean>(Boolean{false});

std::shared_ptr<Object> eval(const std::shared_ptr<Node> &node) {
    // Statements
    if (auto p = std::dynamic_pointer_cast<Program>(node)) {
        return eval_statements(p->statements);
    } else if (auto e = std::dynamic_pointer_cast<ExpressionStatement>(node)) {
        return eval(e->expression);
    // Expressions
    } else if (auto pe = std::dynamic_pointer_cast<PrefixExpression>(node)) {
        auto right = eval(pe->right);
        return eval_prefix_expression(pe->op, right);
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
        return GLOBAL_TRUE;
    }
    return GLOBAL_FALSE;
}

std::shared_ptr<Object> eval_prefix_expression(std::string op, std::shared_ptr<Object> right) {
    if (op == "!") {
        return eval_bang_operator_expression(right);
    } else {
        return GLOBAL_NULL;
    }
}

std::shared_ptr<Object> eval_bang_operator_expression(std::shared_ptr<Object> right) {
    if (right == GLOBAL_TRUE) {
        return GLOBAL_FALSE;
    } else if (right == GLOBAL_FALSE) {
        return GLOBAL_TRUE;
    } else if (right == GLOBAL_NULL) {
        return GLOBAL_TRUE;
    } else {
        return GLOBAL_FALSE;
    }
}
