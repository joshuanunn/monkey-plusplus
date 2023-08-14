#include "environment.hpp"

std::tuple<std::shared_ptr<Object>, bool> Environment::get(const std::string &name) {
    auto contains = store.find(name);
    if (contains == store.end()) {
        return std::make_tuple(nullptr, false);
    }
    return std::make_tuple(store[name], true);
}

std::shared_ptr<Object> Environment::set(std::string name, std::shared_ptr<Object> val) {
    store[name] = val;
    return val;
}

std::shared_ptr<Environment> new_environment() {
    return std::make_shared<Environment>(Environment{});
}
