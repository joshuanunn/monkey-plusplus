#include "environment.hpp"

std::tuple<std::shared_ptr<Object>, bool> Environment::get(const std::string &name) {
    std::shared_ptr<Object> obj;
    bool ok;

    auto contains = store.find(name);
    ok = !(contains == store.end());
    if (ok) {
        obj = store[name];
    }

    if (!ok && outer) {
        contains = outer->store.find(name);
        ok = !(contains == store.end());
        if (ok) {
            obj = outer->store[name];
        }
    }

    return std::make_tuple(obj, ok);
}

std::shared_ptr<Object> Environment::set(std::string name, std::shared_ptr<Object> val) {
    store[name] = val;
    return val;
}

std::shared_ptr<Environment> new_environment() {
    return std::make_shared<Environment>(Environment{});
}

std::shared_ptr<Environment> new_enclosed_environment(const std::shared_ptr<Environment> &outer) {
    auto env = new_environment();
    env->outer = outer;
    return env;
}
