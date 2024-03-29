#ifndef MONKEY_PLUSPLUS_EVALUATOR_HPP
#define MONKEY_PLUSPLUS_EVALUATOR_HPP

#include "ast.hpp"
#include "builtins.hpp"
#include "object.hpp"

std::shared_ptr<Object> eval(const std::shared_ptr<Node> &node, const std::shared_ptr<Environment> &env);

std::shared_ptr<Object> eval_identifier(const std::shared_ptr<Identifier> &node, const std::shared_ptr<Environment> &env);

std::shared_ptr<Object> eval_if_expression(const std::shared_ptr<IfExpression> &ie, const std::shared_ptr<Environment> &env);

bool is_truthy(const std::shared_ptr<Object> &obj);

std::shared_ptr<Object> eval_program(const std::shared_ptr<Program> &program, const std::shared_ptr<Environment> &env);

std::shared_ptr<Object> eval_block_statement(const std::shared_ptr<BlockStatement> &block, const std::shared_ptr<Environment> &env);

std::shared_ptr<Object> native_bool_to_boolean_object(bool input);

std::shared_ptr<Object> eval_prefix_expression(const std::string &op, const std::shared_ptr<Object> &right);

std::shared_ptr<Object> eval_bang_operator_expression(const std::shared_ptr<Object> &right);

std::shared_ptr<Object> eval_minus_prefix_operator_expression(const std::shared_ptr<Object> &right);

std::shared_ptr<Object> eval_index_expression(const std::shared_ptr<Object> &left, const std::shared_ptr<Object> &index);

std::shared_ptr<Object> eval_infix_expression(const std::string &op, const std::shared_ptr<Object> &left, const std::shared_ptr<Object> &right);

std::shared_ptr<Object> eval_array_index_expression(const std::shared_ptr<Object> &array, const std::shared_ptr<Object> &index);

std::shared_ptr<Object> eval_hash_index_expression(const std::shared_ptr<Object> &hash, const std::shared_ptr<Object> &index);

std::shared_ptr<Object> eval_integer_infix_expression(const std::string &op, const std::shared_ptr<Object> &left, const std::shared_ptr<Object> &right);

std::shared_ptr<Object> eval_string_infix_expression(const std::string &op, const std::shared_ptr<Object> &left, const std::shared_ptr<Object> &right);

std::shared_ptr<Object> eval_hash_literal(const std::shared_ptr<HashLiteral> &node, const std::shared_ptr<Environment> &env);

std::vector<std::shared_ptr<Object>> eval_expressions(const std::vector<std::shared_ptr<Expression>> &exps, const std::shared_ptr<Environment> &env);

std::shared_ptr<Object> apply_function(const std::shared_ptr<Object> &fn, const std::vector<std::shared_ptr<Object>> &args);

std::shared_ptr<Environment> extend_function_env(const std::shared_ptr<Function> &fn, const std::vector<std::shared_ptr<Object>> &args);

std::shared_ptr<Object> unwrap_return_value(const std::shared_ptr<Object> &obj);

#endif //MONKEY_PLUSPLUS_EVALUATOR_HPP
