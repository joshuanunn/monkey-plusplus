#ifndef MONKEY_PLUSPLUS_ENVIRONMENT_HPP
#define MONKEY_PLUSPLUS_ENVIRONMENT_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>
#include "object.hpp"

struct Environment {
    std::map<std::string, std::shared_ptr<Object>> store;

    std::shared_ptr<Environment> outer;

    std::tuple<std::shared_ptr<Object>, bool> get(const std::string &name);

    std::shared_ptr<Object> set(std::string name, std::shared_ptr<Object> val);
};

std::shared_ptr<Environment> new_environment();

std::shared_ptr<Environment> new_enclosed_environment(const std::shared_ptr<Environment> &outer);

#endif //MONKEY_PLUSPLUS_ENVIRONMENT_HPP
