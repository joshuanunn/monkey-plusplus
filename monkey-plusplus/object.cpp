#include "object.hpp"

Function::Function(std::vector<std::shared_ptr<Identifier>> p, std::shared_ptr<BlockStatement> b,
                   const std::shared_ptr<Environment> &e) : parameters{std::move(p)}, body{std::move(b)}, env{e} {}

ObjectType Function::type() const {
    return ObjectType::FUNCTION_OBJ;
}

std::string Function::inspect() const {
    std::string out = "fn(";

    int counter = 0;
    for (const auto &p: parameters) {
        if (counter > 0) {
            out.append(", ");
        }
        out += p->string();
        counter++;
    }

    out.append(") {\n" + body->string() + "\n}");

    return out;
}

ReturnValue::ReturnValue(std::shared_ptr<Object> v) : value(v) {}

ObjectType ReturnValue::type() const {
    return ObjectType::RETURN_VALUE_OBJ;
}

std::string ReturnValue::inspect() const {
    return value->inspect();
}

Error::Error(std::string v) : message(v) {}

ObjectType Error::type() const {
    return ObjectType::ERROR_OBJ;
}

std::string Error::inspect() const {
    return "Error: " + message;
}

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

std::map<ObjectType, std::string> objecttype_literals = {
        {ObjectType::INTEGER_OBJ,"INTEGER"},
        {ObjectType::BOOLEAN_OBJ,"BOOLEAN"},
        {ObjectType::NULL_OBJ,"NULL"},
        {ObjectType::RETURN_VALUE_OBJ,"RETURN_VALUE"},
        {ObjectType::ERROR_OBJ,"ERROR"},
        {ObjectType::FUNCTION_OBJ,"FUNCTION"},
};

std::string objecttype_literal(ObjectType t) {
    auto contains = objecttype_literals.find(t);

    // If object is not a defined keyword, then return warning
    if (contains == objecttype_literals.end()) {
        return "OBJECT_NOT_DEFINED";
    }
    return objecttype_literals[t];
}
