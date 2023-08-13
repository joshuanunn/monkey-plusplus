#include "evaluator.hpp"

// Initialise global Boolean and Null Objects to avoid unnecessary Object creation
auto GLOBAL_NULL = std::make_shared<Null>(Null{});
auto GLOBAL_TRUE = std::make_shared<Boolean>(Boolean{true});
auto GLOBAL_FALSE = std::make_shared<Boolean>(Boolean{false});

std::shared_ptr<Null> get_null_ref() {
    return GLOBAL_NULL;
}

std::shared_ptr<Object> eval(const std::shared_ptr<Node> &node) {
    // Statements
    if (auto p = std::dynamic_pointer_cast<Program>(node)) {
        return eval_statements(p->statements);
    } else if (auto b = std::dynamic_pointer_cast<BlockStatement>(node)) {
        return eval_statements(b->statements);
    } else if (auto e = std::dynamic_pointer_cast<ExpressionStatement>(node)) {
        return eval(e->expression);
    // Expressions
    } else if (auto i = std::dynamic_pointer_cast<IfExpression>(node)) {
        return eval_if_expression(i);
    } else if (auto pe = std::dynamic_pointer_cast<PrefixExpression>(node)) {
        auto right = eval(pe->right);
        return eval_prefix_expression(pe->op, right);
    } else if (auto ie = std::dynamic_pointer_cast<InfixExpression>(node)) {
        auto left = eval(ie->left);
        auto right = eval(ie->right);
        return eval_infix_expression(ie->op, left, right);
    } else if (auto il = std::dynamic_pointer_cast<IntegerLiteral>(node)) {
        return std::make_shared<Integer>(Integer{il->value});
    } else if (auto bl = std::dynamic_pointer_cast<BooleanLiteral>(node)) {
        return native_bool_to_boolean_object(bl->value);
    }

    return nullptr;
}

std::shared_ptr<Object> eval_if_expression(const std::shared_ptr<IfExpression> &ie) {
    auto condition = eval(ie->condition);

    if (is_truthy(condition)) {
        return eval(ie->consequence);
    } else if (ie->alternative) {
        return eval(ie->alternative);
    } else {
        return GLOBAL_NULL;
    }
}

bool is_truthy(const std::shared_ptr<Object> &obj) {
    if (obj == GLOBAL_NULL) {
        return false;
    } else if (obj == GLOBAL_TRUE) {
        return true;
    } else if (obj == GLOBAL_FALSE) {
        return false;
    } else {
        return true;
    }
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

std::shared_ptr<Object> eval_prefix_expression(const std::string &op, const std::shared_ptr<Object> &right) {
    if (op == "!") {
        return eval_bang_operator_expression(right);
    } else if (op == "-") {
        return eval_minus_prefix_operator_expression(right);
    } else {
        return GLOBAL_NULL;
    }
}

std::shared_ptr<Object> eval_bang_operator_expression(const std::shared_ptr<Object> &right) {
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

std::shared_ptr<Object> eval_minus_prefix_operator_expression(const std::shared_ptr<Object> &right) {
    if (right->type() != ObjectType::INTEGER_OBJ) {
        return GLOBAL_NULL;
    }

    // Cast Object to Integer Object and also return GLOBAL_NULL if cast unexpectedly fails
    auto original = std::dynamic_pointer_cast<Integer>(right);
    if (!original) {
        return GLOBAL_NULL;
    }

    return std::make_shared<Integer>(Integer{-(original->value)});
}

std::shared_ptr<Object> eval_infix_expression(const std::string &op, const std::shared_ptr<Object> &left, const std::shared_ptr<Object> &right) {
    if (left->type() == ObjectType::INTEGER_OBJ && right->type() == ObjectType::INTEGER_OBJ) {
        return eval_integer_infix_expression(op, left, right);
    } else if (op == "==") {
        return native_bool_to_boolean_object(left == right);
    } else if (op == "!=") {
        return native_bool_to_boolean_object(left != right);
    } else {
        return GLOBAL_NULL;
    }
}

std::shared_ptr<Object> eval_integer_infix_expression(const std::string &op, const std::shared_ptr<Object> &left, const std::shared_ptr<Object> &right) {
    // Cast left and right Objects to Integer Objects and return GLOBAL_NULL if cast unexpectedly fails for either
    auto left_val = std::dynamic_pointer_cast<Integer>(left);
    auto right_val = std::dynamic_pointer_cast<Integer>(right);

    if (!left_val || !right_val) {
        return GLOBAL_NULL;
    }

    if (op == "+") {
        return std::make_shared<Integer>(Integer{left_val->value + right_val->value});
    } else if (op == "-") {
        return std::make_shared<Integer>(Integer{left_val->value - right_val->value});
    } else if (op == "*") {
        return std::make_shared<Integer>(Integer{left_val->value * right_val->value});
    } else if (op == "/") {
        return std::make_shared<Integer>(Integer{left_val->value / right_val->value});
    } else if (op == "<") {
        return native_bool_to_boolean_object(left_val->value < right_val->value);
    } else if (op == ">") {
        return native_bool_to_boolean_object(left_val->value > right_val->value);
    } else if (op == "==") {
        return native_bool_to_boolean_object(left_val->value == right_val->value);
    } else if (op == "!=") {
        return native_bool_to_boolean_object(left_val->value != right_val->value);
    } else {
        return GLOBAL_NULL;
    }
}
