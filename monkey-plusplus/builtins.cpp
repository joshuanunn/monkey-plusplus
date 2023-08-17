#include "builtins.hpp"

std::shared_ptr<Object> len(std::vector<std::shared_ptr<Object>> args) {
    if (args.size() != 1) {
        return new_error("wrong number of arguments. got=" + std::to_string(args.size()) + ", want=1");
    }

    if (auto arg = std::dynamic_pointer_cast<String>(args.at(0))) {
        return std::make_shared<Integer>(arg->value.size());
    }

    return new_error("argument to 'len' not supported.");
}

std::map<std::string, builtin_fn> builtins = {
        {"len", len},
};

std::shared_ptr<Builtin> get_builtin_fn(const std::string &name) {
    auto fn = builtins.find(name);

    // If builtin function is not defined, then return nullptr
    if (fn == builtins.end()) {
        return nullptr;
    }

    return std::make_shared<Builtin>(Builtin{builtins[name]});
}
