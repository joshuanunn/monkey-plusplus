#ifndef MONKEY_PLUSPLUS_BUILTINS_HPP
#define MONKEY_PLUSPLUS_BUILTINS_HPP

#include <functional>
#include <map>
#include <memory>
#include <string>
#include "evaluator.hpp"
#include "object.hpp"

std::shared_ptr<Object> len(std::vector<std::shared_ptr<Object>> args);

std::shared_ptr<Object> first(std::vector<std::shared_ptr<Object>> args);

std::shared_ptr<Object> last(std::vector<std::shared_ptr<Object>> args);

std::shared_ptr<Object> rest(std::vector<std::shared_ptr<Object>> args);

std::shared_ptr<Object> push(std::vector<std::shared_ptr<Object>> args);

std::shared_ptr<Builtin> get_builtin_fn(const std::string &name);

#endif //MONKEY_PLUSPLUS_BUILTINS_HPP
