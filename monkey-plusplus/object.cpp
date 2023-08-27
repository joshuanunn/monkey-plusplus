#include "object.hpp"

HashKey::HashKey(ObjectType t, uint64_t v) : type(t), value(v) {}

bool HashKey::operator==(const HashKey &other) const {
    return type == other.type && value == other.value;
}

bool HashKey::operator!=(const HashKey &other) const {
    return !(type == other.type && value == other.value);
}

bool HashKey::operator<(const HashKey &other) const {
    return value < other.value;
}

Hash::Hash(std::map<HashKey, HashPair> p) : pairs(std::move(p)) {}

ObjectType Hash::type() const {
    return ObjectType::HASH_OBJ;
}

std::string Hash::inspect() const {
    std::string out = "{";

    int counter = 0;
    for (const auto &kv: pairs) {
        if (counter > 0) {
            out.append(", ");
        }
        const auto[_, pair] = kv;
        out += pair.key->inspect() + ": " + pair.value->inspect();
        counter++;
    }

    out.append("}");

    return out;
}

std::shared_ptr<Object> Hash::clone() const {
    return std::make_shared<Hash>(Hash{*this});
}

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

std::shared_ptr<Object> Function::clone() const {
    return std::make_shared<Function>(Function{*this});
}

Builtin::Builtin(builtin_fn v) : builtin_function(v) {}

ObjectType Builtin::type() const {
    return ObjectType::BUILTIN_OBJ;
}

std::string Builtin::inspect() const {
    return "builtin function";
}

std::shared_ptr<Object> Builtin::clone() const {
    return std::make_shared<Builtin>(Builtin{*this});
}

Array::Array() = default;

Array::Array(const Array &a) {
    for (const auto &e: a.elements) {
        elements.push_back(e);
    }
}

ObjectType Array::type() const {
    return ObjectType::ARRAY_OBJ;
}

std::string Array::inspect() const {
    std::string out = "[";

    int counter = 0;
    for (const auto &e: elements) {
        if (counter > 0) {
            out.append(", ");
        }
        out += e->inspect();
        counter++;
    }

    out.append("]");

    return out;
}

std::shared_ptr<Object> Array::clone() const {
    return std::make_shared<Array>(Array{*this});
}

ReturnValue::ReturnValue(std::shared_ptr<Object> v) : value(v) {}

ObjectType ReturnValue::type() const {
    return ObjectType::RETURN_VALUE_OBJ;
}

std::string ReturnValue::inspect() const {
    return value->inspect();
}

std::shared_ptr<Object> ReturnValue::clone() const {
    return std::make_shared<ReturnValue>(ReturnValue{*this});
}

Error::Error(std::string v) : message(v) {}

ObjectType Error::type() const {
    return ObjectType::ERROR_OBJ;
}

std::string Error::inspect() const {
    return "Error: " + message;
}

std::shared_ptr<Object> Error::clone() const {
    return std::make_shared<Error>(Error{*this});
}

Integer::Integer(int v) : value(v) {}

ObjectType Integer::type() const {
    return ObjectType::INTEGER_OBJ;
}

std::string Integer::inspect() const {
    return std::to_string(value);
}

HashKey Integer::hash_key() const {
    return HashKey{type(), static_cast<uint64_t>(value)};
}

std::shared_ptr<Object> Integer::clone() const {
    return std::make_shared<Integer>(Integer{*this});
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

std::shared_ptr<Object> Boolean::clone() const {
    return std::make_shared<Boolean>(Boolean{*this});
}

HashKey Boolean::hash_key() const {
    return HashKey{type(), static_cast<uint64_t>(value)};
}

String::String(std::string v) : value(std::move(v)) {}

ObjectType String::type() const {
    return ObjectType::STRING_OBJ;
}

std::string String::inspect() const {
    return value;
}

std::shared_ptr<Object> String::clone() const {
    return std::make_shared<String>(String{*this});
}

// Hashing algorithm using a 64-bit FNV-1a hash in line with that used in Go (hash/fnv)
// C++ source adapted (under MIT License) from: https://github.com/SRombauts/cpp-algorithms
HashKey String::hash_key() const {
    const char* apStr = value.data();

    uint64_t hash = 14695981039346656037ULL; // 64 bit offset_basis = 14695981039346656037

    for (uint32_t idx = 0; apStr[idx] != 0; ++idx) {
        // 64 bit FNV_prime = 240 + 28 + 0xb3 = 1099511628211
        hash = 1099511628211ULL * (hash ^ static_cast<unsigned char>(apStr[idx]));
    }

    return HashKey{type(), hash};
}

ObjectType Null::type() const {
    return ObjectType::NULL_OBJ;
}

std::string Null::inspect() const {
    return "null";
}

std::shared_ptr<Object> Null::clone() const {
    return std::make_shared<Null>(Null{*this});
}

std::map<ObjectType, std::string> objecttype_literals = {
        {ObjectType::INTEGER_OBJ,"INTEGER"},
        {ObjectType::BOOLEAN_OBJ,"BOOLEAN"},
        {ObjectType::NULL_OBJ,"NULL"},
        {ObjectType::RETURN_VALUE_OBJ,"RETURN_VALUE"},
        {ObjectType::ERROR_OBJ,"ERROR"},
        {ObjectType::FUNCTION_OBJ,"FUNCTION"},
        {ObjectType::STRING_OBJ,"STRING"},
        {ObjectType::BUILTIN_OBJ,"BUILTIN"},
        {ObjectType::ARRAY_OBJ,"ARRAY"},
        {ObjectType::HASH_OBJ,"HASH"},
};

std::string objecttype_literal(ObjectType t) {
    auto contains = objecttype_literals.find(t);

    // If object is not a defined keyword, then return warning
    if (contains == objecttype_literals.end()) {
        return "OBJECT_NOT_DEFINED";
    }
    return objecttype_literals[t];
}
