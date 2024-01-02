#ifndef MONKEY_PLUSPLUS_OBJECT_HPP
#define MONKEY_PLUSPLUS_OBJECT_HPP

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "ast.hpp"
#include "code.hpp"

enum class ObjectType {
    INTEGER_OBJ,
    BOOLEAN_OBJ,
    NULL_OBJ,
    RETURN_VALUE_OBJ,
    ERROR_OBJ,
    FUNCTION_OBJ,
    STRING_OBJ,
    BUILTIN_OBJ,
    ARRAY_OBJ,
    HASH_OBJ,
    COMPILED_FUNCTION_OBJ,
    CLOSURE_OBJ
};

std::string objecttype_literal(ObjectType);

struct Object {
    virtual ~Object() = default;

    virtual ObjectType type() const = 0;

    virtual std::string inspect() const = 0;

    virtual std::shared_ptr<Object> clone() const = 0;
};

struct Environment {
    Environment() = default;

    Environment(const Environment& other);

    Environment(Environment&& other) noexcept;

    Environment& operator=(const Environment& other);

    Environment& operator=(Environment&& other) noexcept;

    std::map<std::string, std::shared_ptr<Object>> store;

    std::shared_ptr<Environment> outer;

    std::tuple<std::shared_ptr<Object>, bool> get(const std::string &name);

    std::shared_ptr<Object> set(std::string name, std::shared_ptr<Object> val);

    std::shared_ptr<Environment> clone() const;
};

std::shared_ptr<Environment> new_environment();

std::shared_ptr<Environment> new_enclosed_environment(const Environment &outer);

struct HashKey {
    HashKey(ObjectType t, uint64_t v);

    ObjectType type;

    uint64_t value;

    bool operator==(const HashKey &other) const;

    bool operator!=(const HashKey &other) const;

    bool operator<(const HashKey &other) const;
};

struct HashPair {
    std::shared_ptr<Object> key;

    std::shared_ptr<Object> value;
};

struct Hashable {
    virtual ~Hashable() = default;

    virtual HashKey hash_key() const = 0;
};

struct Hash : public Object {
    Hash(std::map<HashKey, HashPair> p);

    Hash(const Hash& other);

    Hash(Hash&& other) noexcept;

    Hash& operator=(const Hash& other);

    Hash& operator=(Hash&& other) noexcept;

    std::map<HashKey, HashPair> pairs;

    ObjectType type() const override;

    std::string inspect() const override;

    std::shared_ptr<Object> clone() const override;
};

typedef std::function<std::shared_ptr<Object>(std::vector<std::shared_ptr<Object>>)> builtin_fn;

struct Function : public Object {
    Function(std::vector<std::shared_ptr<Identifier>> p, std::shared_ptr<BlockStatement> b, const std::shared_ptr<Environment> &e);

    Function(const Function& other);

    Function(Function&& other) noexcept;

    Function& operator=(const Function& other);

    Function& operator=(Function&& other) noexcept;

    std::vector<std::shared_ptr<Identifier>> parameters;

    std::shared_ptr<BlockStatement> body;

    std::shared_ptr<Environment> env;

    ObjectType type() const override;

    std::string inspect() const override;

    std::shared_ptr<Object> clone() const override;
};

struct Builtin : public Object {

    explicit Builtin(builtin_fn v);

    Builtin(const Builtin& other);

    Builtin(Builtin&& other) noexcept;

    Builtin& operator=(const Builtin& other);

    Builtin& operator=(Builtin&& other) noexcept;

    builtin_fn builtin_function;

    ObjectType type() const override;

    std::string inspect() const override;

    std::shared_ptr<Object> clone() const override;
};

struct Array : public Object {
    explicit Array();

    Array(const Array& other);

    Array(Array&& other) noexcept;

    Array& operator=(const Array& other);

    Array& operator=(Array&& other) noexcept;

    std::vector<std::shared_ptr<Object>> elements;

    ObjectType type() const override;

    std::string inspect() const override;

    std::shared_ptr<Object> clone() const override;
};

struct ReturnValue : public Object {
    explicit ReturnValue(std::shared_ptr<Object> v);

    ReturnValue(const ReturnValue& other);

    ReturnValue(ReturnValue&& other) noexcept;

    ReturnValue& operator=(const ReturnValue& other);

    ReturnValue& operator=(ReturnValue&& other) noexcept;

    std::shared_ptr<Object> value;

    ObjectType type() const override;

    std::string inspect() const override;

    std::shared_ptr<Object> clone() const override;
};

struct Error : public Object {
    explicit Error(std::string v);

    Error(const Error& other);

    Error(Error&& other) noexcept;

    Error& operator=(const Error& other);

    Error& operator=(Error&& other) noexcept;

    std::string message;

    ObjectType type() const override;

    std::string inspect() const override;

    std::shared_ptr<Object> clone() const override;
};

struct Integer : public Object, Hashable {
    explicit Integer(int v);

    Integer(const Integer& other);

    Integer(Integer&& other) noexcept;

    Integer& operator=(const Integer& other);

    Integer& operator=(Integer&& other) noexcept;

    int value;

    ObjectType type() const override;

    std::string inspect() const override;

    std::shared_ptr<Object> clone() const override;

    HashKey hash_key() const override;
};

struct Boolean : public Object, Hashable {
    explicit Boolean(bool v);

    Boolean(const Boolean& other);

    Boolean(Boolean&& other) noexcept;

    Boolean& operator=(const Boolean& other);

    Boolean& operator=(Boolean&& other) noexcept;

    bool value;

    ObjectType type() const override;

    std::string inspect() const override;

    std::shared_ptr<Object> clone() const override;

    HashKey hash_key() const override;
};

std::shared_ptr<Boolean> get_true_ref();

std::shared_ptr<Boolean> get_false_ref();

struct String : public Object, Hashable {
    explicit String(std::string v);

    String(const String& other);

    String(String&& other) noexcept;

    String& operator=(const String& other);

    String& operator=(String&& other) noexcept;

    std::string value;

    ObjectType type() const override;

    std::string inspect() const override;

    std::shared_ptr<Object> clone() const override;

    HashKey hash_key() const override;
};

struct Null : public Object {
    Null() = default;

    Null(const Null& other) = delete;

    Null(Null&& other) noexcept = default;

    Null& operator=(const Null& other) = delete;

    Null& operator=(Null&& other) noexcept = delete;

    ObjectType type() const override;

    std::shared_ptr<Object> clone() const override;

    std::string inspect() const override;
};

struct CompiledFunction : public Object {
    CompiledFunction(const Instructions& instructions);

    CompiledFunction(const CompiledFunction& other);

    CompiledFunction(CompiledFunction&& other) noexcept;

    CompiledFunction& operator=(const CompiledFunction& other);

    CompiledFunction& operator=(CompiledFunction&& other) noexcept;

    Instructions instructions;

    int num_locals;

    int num_parameters;

    ObjectType type() const override;

    std::string inspect() const override;

    std::shared_ptr<Object> clone() const override;
};

struct Closure : public Object {
    Closure() = default;

    Closure(std::shared_ptr<CompiledFunction> fn);

    Closure(const Closure& other);

    Closure(Closure&& other) noexcept;

    Closure& operator=(const Closure& other);

    Closure& operator=(Closure&& other) noexcept;

    std::shared_ptr<CompiledFunction> fn;

    std::vector<std::shared_ptr<Object>> free;

    ObjectType type() const override;

    std::string inspect() const override;

    std::shared_ptr<Object> clone() const override;
};

std::shared_ptr<Null> get_null_ref();

std::shared_ptr<Error> new_error(std::string message);

bool is_error(const std::shared_ptr<Object>& obj);

#endif //MONKEY_PLUSPLUS_OBJECT_HPP
