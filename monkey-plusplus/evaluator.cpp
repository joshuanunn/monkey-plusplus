#include "environment.hpp"
#include "evaluator.hpp"

// Initialise global Boolean and Null Objects to avoid unnecessary Object creation
auto GLOBAL_NULL = std::make_shared<Null>(Null{});
auto GLOBAL_TRUE = std::make_shared<Boolean>(Boolean{true});
auto GLOBAL_FALSE = std::make_shared<Boolean>(Boolean{false});

std::shared_ptr<Null> get_null_ref() {
    return GLOBAL_NULL;
}

std::shared_ptr<Object> eval(const std::shared_ptr<Node> &node, const std::shared_ptr<Environment> &env) {
    // Statements
    if (auto p = std::dynamic_pointer_cast<Program>(node)) {
        return eval_program(p, env);
    } else if (auto l = std::dynamic_pointer_cast<LetStatement>(node)) {
        auto val = eval(l->value, env);
        if (is_error(val)) {
            return val;
        }
        env->set(l->name->value, val);
    } else if (auto r = std::dynamic_pointer_cast<ReturnStatement>(node)) {
        auto val = eval(r->return_value, env);
        if (is_error(val)) {
            return val;
        }
        return std::make_shared<ReturnValue>(ReturnValue{val});
    } else if (auto b = std::dynamic_pointer_cast<BlockStatement>(node)) {
        return eval_block_statement(b, env);
    } else if (auto e = std::dynamic_pointer_cast<ExpressionStatement>(node)) {
        return eval(e->expression, env);
    // Expressions
    } else if (auto f = std::dynamic_pointer_cast<FunctionLiteral>(node)) {
        auto params = f->parameters;
        auto body = f->body;
        return std::make_shared<Function>(Function{params, body, env});
    } else if (auto c = std::dynamic_pointer_cast<CallExpression>(node)) {
        auto function = eval(c->function, env);
        if (is_error(function)) {
            return function;
        }
        auto args = eval_expressions(c->arguments, env);
        if (args.size() == 1 && is_error(args.at(0))) {
            return args.at(0);
        }
        return apply_function(function, args);
    } else if (auto id = std::dynamic_pointer_cast<Identifier>(node)) {
        return eval_identifier(id, env);
    } else if (auto i = std::dynamic_pointer_cast<IfExpression>(node)) {
        return eval_if_expression(i, env);
    } else if (auto pe = std::dynamic_pointer_cast<PrefixExpression>(node)) {
        auto right = eval(pe->right, env);
        if (is_error(right)) {
            return right;
        }
        return eval_prefix_expression(pe->op, right);
    } else if (auto ie = std::dynamic_pointer_cast<InfixExpression>(node)) {
        auto left = eval(ie->left, env);
        if (is_error(left)) {
            return left;
        }
        auto right = eval(ie->right, env);
        if (is_error(right)) {
            return right;
        }
        return eval_infix_expression(ie->op, left, right);
    } else if (auto il = std::dynamic_pointer_cast<IntegerLiteral>(node)) {
        return std::make_shared<Integer>(Integer{il->value});
    } else if (auto bl = std::dynamic_pointer_cast<BooleanLiteral>(node)) {
        return native_bool_to_boolean_object(bl->value);
    } else if (auto sl = std::dynamic_pointer_cast<StringLiteral>(node)) {
        return std::make_shared<String>(String{sl->value});
    }

    return nullptr;
}

std::shared_ptr<Object> eval_identifier(const std::shared_ptr<Identifier> &node, const std::shared_ptr<Environment> &env) {
    auto[val, ok] = env->get(node->value);
    if (!ok) {
        return new_error("identifier not found: " + node->value);
    }

    return val;
}

std::shared_ptr<Object> eval_if_expression(const std::shared_ptr<IfExpression> &ie, const std::shared_ptr<Environment> &env) {
    auto condition = eval(ie->condition, env);
    if (is_error(condition)) {
        return condition;
    }

    if (is_truthy(condition)) {
        return eval(ie->consequence, env);
    } else if (ie->alternative) {
        return eval(ie->alternative, env);
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

std::shared_ptr<Object> eval_program(const std::shared_ptr<Program> &program, const std::shared_ptr<Environment> &env) {
    std::shared_ptr<Object> result;

    for (const auto &statement: program->statements) {
        result = eval(statement, env);

        if (auto return_value = std::dynamic_pointer_cast<ReturnValue>(result)) {
            return return_value->value;
        }

        if (auto return_value = std::dynamic_pointer_cast<Error>(result)) {
            return return_value;
        }
    }

    return result;
}

std::shared_ptr<Object> eval_block_statement(const std::shared_ptr<BlockStatement> &block, const std::shared_ptr<Environment> &env) {
    std::shared_ptr<Object> result;

    for (const auto &statement: block->statements) {
        result = eval(statement, env);

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

std::vector<std::shared_ptr<Object>> eval_expressions(const std::vector<std::shared_ptr<Expression>> &exps, const std::shared_ptr<Environment> &env) {

    std::vector<std::shared_ptr<Object>> result;

    for (const auto &e: exps) {
        auto evaluated = eval(e, env);
        if (is_error(evaluated)) {
            return std::vector<std::shared_ptr<Object>>{evaluated};
        }
        result.push_back(evaluated);
    }

    return result;
}

std::shared_ptr<Object> apply_function(const std::shared_ptr<Object> &fn, const std::vector<std::shared_ptr<Object>> &args) {
    auto function = std::dynamic_pointer_cast<Function>(fn);

    if (!function) {
        return new_error("not a function.");
    }

    auto entended_env = extend_function_env(function, args);
    auto evaluated = eval(function->body, entended_env);

    return unwrap_return_value(evaluated);
}

std::shared_ptr<Environment> extend_function_env(const std::shared_ptr<Function> &fn, const std::vector<std::shared_ptr<Object>> &args) {
    auto env = new_enclosed_environment(fn->env);

    for (int i = 0; i < fn->parameters.size(); i++) {
        env->set(fn->parameters.at(i)->value, args.at(i));
    }

    return env;
}

std::shared_ptr<Object> unwrap_return_value(const std::shared_ptr<Object> &obj) {
    auto return_value = std::dynamic_pointer_cast<ReturnValue>(obj);

    if (return_value) {
        return return_value->value;
    }

    return obj;
}
