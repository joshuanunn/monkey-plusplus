#ifndef MONKEY_PLUSPLUS_EVALUATOR_HPP
#define MONKEY_PLUSPLUS_EVALUATOR_HPP

#include "ast.hpp"
#include "object.hpp"

std::shared_ptr<Object> eval(const std::shared_ptr<Node> &node);

std::shared_ptr<Object> eval_statements(const std::vector<std::shared_ptr<Node>> &stmts);

std::shared_ptr<Object> native_bool_to_boolean_object(bool input);

std::shared_ptr<Object> eval_prefix_expression(std::string op, std::shared_ptr<Object> right);

std::shared_ptr<Object> eval_bang_operator_expression(std::shared_ptr<Object> right);

std::shared_ptr<Object> eval_minus_prefix_operator_expression(const std::shared_ptr<Object> &right);

#endif //MONKEY_PLUSPLUS_EVALUATOR_HPP
