#ifndef MONKEY_PLUSPLUS_BUILTINS_HPP
#define MONKEY_PLUSPLUS_BUILTINS_HPP

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include "evaluator.hpp"
#include "object.hpp"

std::shared_ptr<Object> __len(std::vector<std::shared_ptr<Object>> args);

std::shared_ptr<Object> __first(std::vector<std::shared_ptr<Object>> args);

std::shared_ptr<Object> __last(std::vector<std::shared_ptr<Object>> args);

std::shared_ptr<Object> __rest(std::vector<std::shared_ptr<Object>> args);

std::shared_ptr<Object> __push(std::vector<std::shared_ptr<Object>> args);

std::shared_ptr<Object> __puts(std::vector<std::shared_ptr<Object>> args);

std::shared_ptr<Builtin> get_builtin_fn(const std::string &name);

#endif //MONKEY_PLUSPLUS_BUILTINS_HPP
