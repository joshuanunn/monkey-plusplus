#ifndef MONKEY_PLUSPLUS_BUILTINS_HPP
#define MONKEY_PLUSPLUS_BUILTINS_HPP

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "object.hpp"

// Store all defined builtin function names in an array to fix index ordering
extern std::array<std::string, 6> builtins_names;

std::shared_ptr<Object> __len(std::vector<std::shared_ptr<Object>> args);

std::shared_ptr<Object> __first(std::vector<std::shared_ptr<Object>> args);

std::shared_ptr<Object> __last(std::vector<std::shared_ptr<Object>> args);

std::shared_ptr<Object> __rest(std::vector<std::shared_ptr<Object>> args);

std::shared_ptr<Object> __push(std::vector<std::shared_ptr<Object>> args);

std::shared_ptr<Object> __puts(std::vector<std::shared_ptr<Object>> args);

std::shared_ptr<Builtin> get_builtin_by_name(const std::string &name);

std::shared_ptr<Builtin> get_builtin_by_index(int index);

#endif //MONKEY_PLUSPLUS_BUILTINS_HPP
