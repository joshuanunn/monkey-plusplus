#ifndef MONKEY_PLUSPLUS_EVALUATOR_HPP
#define MONKEY_PLUSPLUS_EVALUATOR_HPP

#include "ast.hpp"
#include "object.hpp"

std::shared_ptr<Object> eval(const std::shared_ptr<Node> &node);

std::shared_ptr<Object> eval_statements(const std::vector<std::shared_ptr<Node>> &stmts);

#endif //MONKEY_PLUSPLUS_EVALUATOR_HPP
