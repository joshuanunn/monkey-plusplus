#include "builtins.hpp"

std::shared_ptr<Object> __len(std::vector<std::shared_ptr<Object>> args) {
    if (args.size() != 1) {
        return new_error("wrong number of arguments. got=" + std::to_string(args.size()) + ", want=1");
    }

    if (auto array = std::dynamic_pointer_cast<Array>(args.at(0))) {
        return std::make_shared<Integer>(array->elements.size());
    } else if (auto str = std::dynamic_pointer_cast<String>(args.at(0))) {
        return std::make_shared<Integer>(str->value.size());
    }

    return new_error("argument to 'len' not supported, got " + objecttype_literal(args.at(0)->type()));
}

std::shared_ptr<Object> __first(std::vector<std::shared_ptr<Object>> args) {
    if (args.size() != 1) {
        return new_error("wrong number of arguments. got=" + std::to_string(args.size()) + ", want=1");
    }

    if (args.at(0)->type() != ObjectType::ARRAY_OBJ) {
        return new_error("argument to 'first' must be ARRAY, got " + objecttype_literal(args.at(0)->type()));
    }

    if (auto arr = std::dynamic_pointer_cast<Array>(args.at(0))) {
        if (!arr->elements.empty()) {
            return arr->elements.at(0);
        }
    }

    return get_null_ref();
}

std::shared_ptr<Object> __last(std::vector<std::shared_ptr<Object>> args) {
    if (args.size() != 1) {
        return new_error("wrong number of arguments. got=" + std::to_string(args.size()) + ", want=1");
    }

    if (args.at(0)->type() != ObjectType::ARRAY_OBJ) {
        return new_error("argument to 'last' must be ARRAY, got " + objecttype_literal(args.at(0)->type()));
    }

    if (auto arr = std::dynamic_pointer_cast<Array>(args.at(0))) {
        auto length = arr->elements.size();
        if (length > 0) {
            return arr->elements.at(length-1);
        }
    }

    return get_null_ref();
}

std::shared_ptr<Object> __rest(std::vector<std::shared_ptr<Object>> args) {
    if (args.size() != 1) {
        return new_error("wrong number of arguments. got=" + std::to_string(args.size()) + ", want=1");
    }

    if (args.at(0)->type() != ObjectType::ARRAY_OBJ) {
        return new_error("argument to 'rest' must be ARRAY, got " + objecttype_literal(args.at(0)->type()));
    }

    if (auto arr = std::dynamic_pointer_cast<Array>(args.at(0))) {
        auto length = arr->elements.size();
        if (length > 0) {
            std::vector<std::shared_ptr<Object>> new_elements;

            // Iterate over vector from second element onwards (i.e. skip first element)
            for (auto i = std::next(arr->elements.begin()); i != arr->elements.end(); ++i) {
                new_elements.push_back(*i);
            }
            auto new_array = std::make_shared<Array>(Array{});
            new_array->elements = std::move(new_elements);

            return new_array;
        }
    }

    return get_null_ref();
}

std::shared_ptr<Object> __push(std::vector<std::shared_ptr<Object>> args) {
    if (args.size() != 2) {
        return new_error("wrong number of arguments. got=" + std::to_string(args.size()) + ", want=2");
    }

    if (args.at(0)->type() != ObjectType::ARRAY_OBJ) {
        return new_error("argument to 'push' must be ARRAY, got " + objecttype_literal(args.at(0)->type()));
    }

    if (auto arr = std::dynamic_pointer_cast<Array>(args.at(0))) {
        // Copy existing array and add new element to end
        auto new_array = std::make_shared<Array>(Array(*arr));

        new_array->elements.push_back(std::move(args.at(1)));

        return new_array;
    }

    return get_null_ref();
}

std::shared_ptr<Object> __puts(std::vector<std::shared_ptr<Object>> args) {
    for (const auto &arg: args) {
        std::cout << arg->inspect() << std::endl;
    }

    return get_null_ref();
}

// Initialise all defined builtin function names in an array to fix index ordering
std::array<std::string, 6> builtins_names = {
    "len", "puts", "first", "last", "rest", "push"};

// Store all defined builtin function pointers
// IMPORTANT: array elements must match ordering in builtins_names
std::array<builtin_fn, 6> builtins = {
    __len, __puts, __first, __last, __rest, __push};

std::shared_ptr<Builtin> get_builtin_by_name(const std::string &name) {
    auto fn = std::find(builtins_names.begin(), builtins_names.end(), name);

    // If builtin function is not defined, then return nullptr
    if (fn == builtins_names.end()) {
        return nullptr;
    }

    auto index = std::distance(builtins_names.begin(), fn);

    return std::make_shared<Builtin>(Builtin{builtins[static_cast<long unsigned>(index)]});
}

std::shared_ptr<Builtin> get_builtin_by_index(int index) {
    return std::make_shared<Builtin>(Builtin{builtins[static_cast<long unsigned>(index)]});
}
