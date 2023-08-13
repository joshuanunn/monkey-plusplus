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
        return eval_program(p);
    } else if (auto r = std::dynamic_pointer_cast<ReturnStatement>(node)) {
        auto val = eval(r->return_value);
        if (is_error(val)) {
            return val;
        }
        return std::make_shared<ReturnValue>(ReturnValue{val});
    } else if (auto b = std::dynamic_pointer_cast<BlockStatement>(node)) {
        return eval_block_statement(b);
    } else if (auto e = std::dynamic_pointer_cast<ExpressionStatement>(node)) {
        return eval(e->expression);
    // Expressions
    } else if (auto i = std::dynamic_pointer_cast<IfExpression>(node)) {
        return eval_if_expression(i);
    } else if (auto pe = std::dynamic_pointer_cast<PrefixExpression>(node)) {
        auto right = eval(pe->right);
        if (is_error(right)) {
            return right;
        }
        return eval_prefix_expression(pe->op, right);
    } else if (auto ie = std::dynamic_pointer_cast<InfixExpression>(node)) {
        auto left = eval(ie->left);
        if (is_error(left)) {
            return left;
        }
        auto right = eval(ie->right);
        if (is_error(right)) {
            return right;
        }
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
    if (is_error(condition)) {
        return condition;
    }

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

std::shared_ptr<Error> new_error(std::string message) {
    return std::make_shared<Error>(Error{std::move(message)});
}

bool is_error(std::shared_ptr<Object> obj) {
    if (obj) {
        return obj->type() == ObjectType::ERROR_OBJ;
    }
    return false;
}

std::shared_ptr<Object> eval_program(const std::shared_ptr<Program> &program) {
    std::shared_ptr<Object> result;

    for (const auto &statement: program->statements) {
        result = eval(statement);

        if (auto return_value = std::dynamic_pointer_cast<ReturnValue>(result)) {
            return return_value->value;
        }

        if (auto return_value = std::dynamic_pointer_cast<Error>(result)) {
            return return_value;
        }
    }

    return result;
}

std::shared_ptr<Object> eval_block_statement(const std::shared_ptr<BlockStatement> &block) {
    std::shared_ptr<Object> result;

    for (const auto &statement: block->statements) {
        result = eval(statement);

        if (auto return_value = std::dynamic_pointer_cast<ReturnValue>(result)) {
            return return_value;
        }

        if (auto return_value = std::dynamic_pointer_cast<Error>(result)) {
            return return_value;
        }
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
        return new_error("unknown operator: " + op + objecttype_literal(right->type()));
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
        return new_error("unknown operator: -" + objecttype_literal(right->type()));
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
    } else if (left->type() != right->type()) {
        return new_error("type mismatch: " + objecttype_literal(left->type()) + " " + op + " " + objecttype_literal(right->type()));
    } else {
        return new_error("unknown operator: " + objecttype_literal(left->type()) + " " + op + " " + objecttype_literal(right->type()));
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
        return new_error("unknown operator: " + objecttype_literal(left->type()) + " " + op + " " + objecttype_literal(right->type()));
    }
}
