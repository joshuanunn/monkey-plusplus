#include "evaluator.hpp"

std::shared_ptr<Object> eval(const std::shared_ptr<Node> &node, const std::shared_ptr<Environment> &env) {
    // Statements
    if (auto p = std::dynamic_pointer_cast<Program>(node)) {
        return eval_program(p, env);
    } else if (auto b = std::dynamic_pointer_cast<BlockStatement>(node)) {
        return eval_block_statement(b, env);
    } else if (auto e = std::dynamic_pointer_cast<ExpressionStatement>(node)) {
        return eval(e->expression, env);
    } else if (auto r = std::dynamic_pointer_cast<ReturnStatement>(node)) {
        auto val = eval(r->return_value, env);
        if (is_error(val)) {
            return val;
        }
        return std::make_shared<ReturnValue>(ReturnValue{val});
    } else if (auto l = std::dynamic_pointer_cast<LetStatement>(node)) {
        auto val = eval(l->value, env);
        if (is_error(val)) {
            return val;
        }
        env->set(l->name->value, val);
    // Expressions
    } else if (auto il = std::dynamic_pointer_cast<IntegerLiteral>(node)) {
        return std::make_shared<Integer>(Integer{il->value});
    } else if (auto sl = std::dynamic_pointer_cast<StringLiteral>(node)) {
        return std::make_shared<String>(String{sl->value});
    } else if (auto bl = std::dynamic_pointer_cast<BooleanLiteral>(node)) {
        return native_bool_to_boolean_object(bl->value);
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
    } else if (auto i = std::dynamic_pointer_cast<IfExpression>(node)) {
        return eval_if_expression(i, env);
    } else if (auto id = std::dynamic_pointer_cast<Identifier>(node)) {
        return eval_identifier(id, env);
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
    } else if (auto a = std::dynamic_pointer_cast<ArrayLiteral>(node)) {
        auto elements = eval_expressions(a->elements, env);
        if (elements.size() == 1 && is_error(elements.at(0))) {
            return elements.at(0);
        }
        auto array = std::make_shared<Array>(Array{});
        array->elements = std::move(elements);
        return array;
    } else if (auto ix = std::dynamic_pointer_cast<IndexExpression>(node)) {
        auto left = eval(ix->left, env);
        if (is_error(left)) {
            return left;
        }
        auto index = eval(ix->index, env);
        if (is_error(index)) {
            return index;
        }
        return eval_index_expression(left, index);
    } else if (auto hl = std::dynamic_pointer_cast<HashLiteral>(node)) {
        return eval_hash_literal(hl, env);
    }

    return nullptr;
}

std::shared_ptr<Object> eval_identifier(const std::shared_ptr<Identifier> &node, const std::shared_ptr<Environment> &env) {
    auto[val, ok] = env->get(node->value);
    if (ok) {
        return val;
    }

    if (auto builtin = get_builtin_fn(node->value)) {
        return builtin;
    }

    return new_error("identifier not found: " + node->value);
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
        return get_null_ref();
    }
}

bool is_truthy(const std::shared_ptr<Object> &obj) {
    if (obj == get_null_ref()) {
        return false;
    } else if (obj == get_true_ref()) {
        return true;
    } else if (obj == get_false_ref()) {
        return false;
    } else {
        return true;
    }
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
        return get_true_ref();
    }
    return get_false_ref();
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
    if (right == get_true_ref()) {
        return get_false_ref();
    } else if (right == get_false_ref()) {
        return get_true_ref();
    } else if (right == get_null_ref()) {
        return get_true_ref();
    } else {
        return get_false_ref();
    }
}

std::shared_ptr<Object> eval_minus_prefix_operator_expression(const std::shared_ptr<Object> &right) {
    if (right->type() != ObjectType::INTEGER_OBJ) {
        return new_error("unknown operator: -" + objecttype_literal(right->type()));
    }

    // Cast Object to Integer Object and also return GLOBAL_NULL if cast unexpectedly fails
    auto original = std::dynamic_pointer_cast<Integer>(right);
    if (!original) {
        return get_null_ref();
    }

    return std::make_shared<Integer>(Integer{-(original->value)});
}

std::shared_ptr<Object> eval_index_expression(const std::shared_ptr<Object> &left, const std::shared_ptr<Object> &index) {
    if (left->type() == ObjectType::ARRAY_OBJ && index->type() == ObjectType::INTEGER_OBJ) {
        return eval_array_index_expression(left, index);
    } else if (left->type() == ObjectType::HASH_OBJ) {
        return eval_hash_index_expression(left, index);
    } else {
        return new_error("index operator not supported: " + objecttype_literal(left->type()));
    }
}

std::shared_ptr<Object> eval_infix_expression(const std::string &op, const std::shared_ptr<Object> &left, const std::shared_ptr<Object> &right) {
    if (left->type() == ObjectType::INTEGER_OBJ && right->type() == ObjectType::INTEGER_OBJ) {
        return eval_integer_infix_expression(op, left, right);
    } else if (left->type() == ObjectType::STRING_OBJ && right->type() == ObjectType::STRING_OBJ) {
        return eval_string_infix_expression(op, left, right);
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

std::shared_ptr<Object> eval_array_index_expression(const std::shared_ptr<Object> &array, const std::shared_ptr<Object> &index) {
    // Cast left and right Objects to Integer Objects and return GLOBAL_NULL if cast unexpectedly fails for either
    auto array_object = std::dynamic_pointer_cast<Array>(array);
    auto idx = std::dynamic_pointer_cast<Integer>(index);

    if (!array_object || !idx) {
        return get_null_ref();
    }

    auto index_value = idx->value;
    auto max_value = array_object->elements.size() - 1;

    if (index_value < 0 || index_value > max_value) {
        return get_null_ref();
    }

    return array_object->elements.at(index_value);
}

std::shared_ptr<Object> eval_hash_index_expression(const std::shared_ptr<Object> &hash, const std::shared_ptr<Object> &index) {
    auto hash_object = std::dynamic_pointer_cast<Hash>(hash);

    auto key = std::dynamic_pointer_cast<Hashable>(index);
    if (!key) {
        return new_error("unusable as hash key: " + objecttype_literal(index->type()));
    }

    auto contains = hash_object->pairs.find(key->hash_key());

    if (contains == hash_object->pairs.end()) {
        return get_null_ref();
    }

    auto pair = hash_object->pairs[key->hash_key()];

    return pair.value;
}

std::shared_ptr<Object> eval_integer_infix_expression(const std::string &op, const std::shared_ptr<Object> &left, const std::shared_ptr<Object> &right) {
    // Cast left and right Objects to Integer Objects and return GLOBAL_NULL if cast unexpectedly fails for either
    auto left_val = std::dynamic_pointer_cast<Integer>(left);
    auto right_val = std::dynamic_pointer_cast<Integer>(right);

    if (!left_val || !right_val) {
        return get_null_ref();
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

std::shared_ptr<Object> eval_string_infix_expression(const std::string &op, const std::shared_ptr<Object> &left, const std::shared_ptr<Object> &right) {
    // Cast left and right Objects to String Objects and return GLOBAL_NULL if cast unexpectedly fails for either
    auto left_val = std::dynamic_pointer_cast<String>(left);
    auto right_val = std::dynamic_pointer_cast<String>(right);

    if (!left_val || !right_val) {
        return get_null_ref();
    }

    if (op != "+") {
        return new_error("unknown operator: " + objecttype_literal(left->type()) + " " + op + " " + objecttype_literal(right->type()));
    }

    return std::make_shared<String>(String{left_val->value + right_val->value});
}

std::shared_ptr<Object> eval_hash_literal(const std::shared_ptr<HashLiteral> &node, const std::shared_ptr<Environment> &env) {
    std::map<HashKey, HashPair> pairs;

    for (const auto &kv: node->pairs) {
        const auto[key_node, value_node] = kv;

        auto key = eval(key_node, env);
        if (is_error(key)) {
            return key;
        }

        auto hash_key = std::dynamic_pointer_cast<Hashable>(key);
        if (!hash_key) {
            return new_error("unusable as hash key.");
        }

        auto value = eval(value_node, env);
        if (is_error(value)) {
            return value;
        }

        auto hashed = hash_key->hash_key();
        pairs[hashed] = HashPair{key, value};
    }

    return std::make_shared<Hash>(Hash{pairs});
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
    if (auto function = std::dynamic_pointer_cast<Function>(fn)) {
        auto extended_env = extend_function_env(function, args);
        auto evaluated = eval(function->body, extended_env);
        return unwrap_return_value(evaluated);
    }

    if (auto builtin = std::dynamic_pointer_cast<Builtin>(fn)) {
        return builtin->builtin_function(args);
    }

    return new_error("not a function.");
}

std::shared_ptr<Environment> extend_function_env(const std::shared_ptr<Function> &fn, const std::vector<std::shared_ptr<Object>> &args) {
    auto env = new_enclosed_environment(*fn->env);

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
