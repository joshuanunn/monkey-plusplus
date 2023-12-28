#include "object.hpp"

// Initialise global Boolean and Null Objects to avoid unnecessary Object creation
auto GLOBAL_NULL = std::make_shared<Null>(Null{});
auto GLOBAL_TRUE = std::make_shared<Boolean>(Boolean{true});
auto GLOBAL_FALSE = std::make_shared<Boolean>(Boolean{false});

std::shared_ptr<Null> get_null_ref() {
    return GLOBAL_NULL;
}

std::shared_ptr<Boolean> get_true_ref() {
    return GLOBAL_TRUE;
}

std::shared_ptr<Boolean> get_false_ref() {
    return GLOBAL_FALSE;
}

Environment::Environment(const Environment& other) : store{} {
    for (const auto &s: other.store) {
        store[s.first] = std::move(s.second->clone());
    }

    if (other.outer) {
        outer = other.outer->clone();
    }
}

Environment::Environment(Environment&& other) noexcept {
    store.merge(other.store);
    outer = std::move(other.outer);

    other.outer = nullptr;
}

Environment& Environment::operator=(const Environment& other) {
    if (this == &other) return *this;

    for (const auto &s: other.store) {
        store[s.first] = std::move(s.second->clone());
    }

    if (other.outer) {
        outer = other.outer->clone();
    }

    return *this;
}

Environment& Environment::operator=(Environment&& other) noexcept {
    if (this == &other) return *this;

    store.merge(other.store);
    outer = std::move(other.outer);

    other.outer = nullptr;

    return *this;
}

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
        ok = !(contains == outer->store.end());
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

std::shared_ptr<Environment> Environment::clone() const {
    return std::make_shared<Environment>(Environment{*this});
}

std::shared_ptr<Environment> new_environment() {
    return std::make_shared<Environment>(Environment{});
}

std::shared_ptr<Environment> new_enclosed_environment(const Environment &outer) {
    auto env = new_environment();
    env->outer = outer.clone();
    return env;
}

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

Hash::Hash(const Hash& other) : pairs{} {
    for (const auto &kv: other.pairs) {
        const auto[key, value] = kv;
        auto key_copy = HashKey{key.type, key.value};
        auto value_copy = HashPair{value.key->clone(), value.value->clone()};
        pairs[key_copy] = std::move(value_copy);
    }
}

Hash::Hash(Hash&& other) noexcept {
    pairs.merge(other.pairs);
}

Hash& Hash::operator=(const Hash& other) {
    if (this == &other) return *this;

    for (const auto &kv: other.pairs) {
        const auto[key, value] = kv;
        auto key_copy = HashKey{key.type, key.value};
        auto value_copy = HashPair{value.key->clone(), value.value->clone()};
        pairs[key_copy] = std::move(value_copy);
    }

    return *this;
}

Hash& Hash::operator=(Hash&& other) noexcept {
    if (this == &other) return *this;

    pairs.merge(other.pairs);

    return *this;
}

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
                   const std::shared_ptr<Environment> &e) : parameters{std::move(p)}, body{std::move(b)} {
    env = e;
}

Function::Function(const Function& other) : parameters{} {
    for (const auto &s: other.parameters) {
        parameters.push_back(std::dynamic_pointer_cast<Identifier>(s->clone()));
    }

    body = std::dynamic_pointer_cast<BlockStatement>(other.body->clone());
    env = std::dynamic_pointer_cast<Environment>(other.env); // Note: env shouldn't be cloned here
}

Function::Function(Function&& other) noexcept {
    parameters.swap(other.parameters);
    body = std::move(other.body);
    env = std::move(other.env);

    other.body = nullptr;
    other.env = nullptr;
}

Function& Function::operator=(const Function& other) {
    if (this == &other) return *this;

    for (const auto &s: other.parameters) {
        parameters.push_back(std::dynamic_pointer_cast<Identifier>(s->clone()));
    }

    body = std::dynamic_pointer_cast<BlockStatement>(other.body->clone());
    env = std::dynamic_pointer_cast<Environment>(other.env); // Note: env shouldn't be cloned here

    return *this;
}

Function& Function::operator=(Function&& other) noexcept {
    if (this == &other) return *this;

    parameters.swap(other.parameters);
    body = std::move(other.body);
    env = std::move(other.env);

    other.body = nullptr;
    other.env = nullptr;

    return *this;
}

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

Builtin::Builtin(const Builtin& other) {
    builtin_function = other.builtin_function;
}

Builtin::Builtin(Builtin&& other) noexcept {
    builtin_function = std::move(other.builtin_function);

    other.builtin_function = nullptr;
}

Builtin& Builtin::operator=(const Builtin& other) {
    if (this == &other) return *this;

    builtin_function = other.builtin_function;

    return *this;
}

Builtin& Builtin::operator=(Builtin&& other) noexcept {
    if (this == &other) return *this;

    builtin_function = std::move(other.builtin_function);

    other.builtin_function = nullptr;

    return *this;
}

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

Array::Array(const Array& other) : elements{} {
    for (const auto &s: other.elements) {
        elements.push_back(std::dynamic_pointer_cast<Object>(s->clone()));
    }
}

Array::Array(Array&& other) noexcept {
    elements.swap(other.elements);
}

Array& Array::operator=(const Array& other) {
    if (this == &other) return *this;

    for (const auto &s: other.elements) {
        elements.push_back(std::dynamic_pointer_cast<Object>(s->clone()));
    }

    return *this;
}

Array& Array::operator=(Array&& other) noexcept {
    if (this == &other) return *this;

    elements.swap(other.elements);

    return *this;
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

ReturnValue::ReturnValue(const ReturnValue& other) {
    // Call clone methods to force deep copy
    value = std::dynamic_pointer_cast<Object>(other.value->clone());
}

ReturnValue::ReturnValue(ReturnValue&& other) noexcept {
    value = std::move(other.value);

    other.value = nullptr;
}

ReturnValue& ReturnValue::operator=(const ReturnValue& other) {
    if (this == &other) return *this;

    // Call clone methods to force deep copy
    value = std::dynamic_pointer_cast<Object>(other.value->clone());

    return *this;
}

ReturnValue& ReturnValue::operator=(ReturnValue&& other) noexcept {
    if (this == &other) return *this;

    value = std::move(other.value);

    other.value = nullptr;

    return *this;
}

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

Error::Error(const Error& other) {
    message = std::string(other.message);
}

Error::Error(Error&& other) noexcept {
    message = std::move(other.message);

    other.message = "";
}

Error& Error::operator=(const Error& other) {
    if (this == &other) return *this;

    message = std::string(other.message);

    return *this;
}

Error& Error::operator=(Error&& other) noexcept {
    if (this == &other) return *this;

    message = std::move(other.message);

    other.message = "";

    return *this;
}

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

Integer::Integer(const Integer& other) : value{other.value} {}

Integer::Integer(Integer&& other) noexcept {
    value = other.value;

    other.value = 0;
}

Integer& Integer::operator=(const Integer& other) {
    if (this == &other) return *this;

    value = other.value;

    return *this;
}

Integer& Integer::operator=(Integer&& other) noexcept {
    if (this == &other) return *this;

    value = other.value;

    other.value = 0;

    return *this;
}

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

Boolean::Boolean(const Boolean& other) : value{other.value} {}

Boolean::Boolean(Boolean&& other) noexcept {
    value = other.value;

    other.value = false;
}

Boolean& Boolean::operator=(const Boolean& other) {
    if (this == &other) return *this;

    value = other.value;

    return *this;
}

Boolean& Boolean::operator=(Boolean&& other) noexcept {
    if (this == &other) return *this;

    value = other.value;

    other.value = false;

    return *this;
}

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

String::String(const String& other) : value{other.value} {}

String::String(String&& other) noexcept {
    value = std::move(other.value);

    other.value = "";
}

String& String::operator=(const String& other) {
    if (this == &other) return *this;

    value = other.value;

    return *this;
}

String& String::operator=(String&& other) noexcept {
    if (this == &other) return *this;

    value = std::move(other.value);

    other.value = "";

    return *this;
}

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
    return get_null_ref();
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

std::shared_ptr<Error> new_error(std::string message) {
    return std::make_shared<Error>(Error{std::move(message)});
}
