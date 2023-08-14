#ifndef MONKEY_PLUSPLUS_EVALUATOR_HPP
#define MONKEY_PLUSPLUS_EVALUATOR_HPP

#include "ast.hpp"
#include "object.hpp"

std::shared_ptr<Null> get_null_ref();

std::shared_ptr<Object> eval(const std::shared_ptr<Node> &node, const std::shared_ptr<Environment> &env);

std::shared_ptr<Object> eval_identifier(const std::shared_ptr<Identifier> &node, const std::shared_ptr<Environment> &env);

std::shared_ptr<Object> eval_if_expression(const std::shared_ptr<IfExpression> &ie, const std::shared_ptr<Environment> &env);

bool is_truthy(const std::shared_ptr<Object> &obj);

std::shared_ptr<Error> new_error(std::string message);

bool is_error(std::shared_ptr<Object> obj);

std::shared_ptr<Object> eval_program(const std::shared_ptr<Program> &program, const std::shared_ptr<Environment> &env);

std::shared_ptr<Object> eval_block_statement(const std::shared_ptr<BlockStatement> &block, const std::shared_ptr<Environment> &env);

std::shared_ptr<Object> native_bool_to_boolean_object(bool input);

std::shared_ptr<Object> eval_prefix_expression(const std::string &op, const std::shared_ptr<Object> &right);

std::shared_ptr<Object> eval_bang_operator_expression(const std::shared_ptr<Object> &right);

std::shared_ptr<Object> eval_minus_prefix_operator_expression(const std::shared_ptr<Object> &right);

std::shared_ptr<Object> eval_infix_expression(const std::string &op, const std::shared_ptr<Object> &left, const std::shared_ptr<Object> &right);

std::shared_ptr<Object> eval_integer_infix_expression(const std::string &op, const std::shared_ptr<Object> &left, const std::shared_ptr<Object> &right);

#endif //MONKEY_PLUSPLUS_EVALUATOR_HPP
