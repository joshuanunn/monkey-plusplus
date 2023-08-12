#include "object.hpp"

Integer::Integer(int v) : value(v) {}

ObjectType Integer::type() const {
    return ObjectType::INTEGER_OBJ;
}

std::string Integer::inspect() const {
    return std::to_string(value);
}

Boolean::Boolean(bool v) : value(v) {}

ObjectType Boolean::type() const {
    return ObjectType::BOOLEAN_OBJ;
}

std::string Boolean::inspect() const {
    if (!value) {
        return "false";
    }
    return "true";
}

ObjectType Null::type() const {
    return ObjectType::NULL_OBJ;
}

std::string Null::inspect() const {
    return "null";
}
