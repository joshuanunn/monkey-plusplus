#ifndef MONKEY_PLUSPLUS_OBJECT_HPP
#define MONKEY_PLUSPLUS_OBJECT_HPP

#include <functional>
#include <map>
#include <memory>
#include <string>
#include "ast.hpp"

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
    HASH_OBJ
};

std::string objecttype_literal(ObjectType);

struct Object {
    virtual ~Object() = default;

    virtual ObjectType type() const = 0;

    virtual std::string inspect() const = 0;

    virtual std::shared_ptr<Object> clone() const = 0;
};

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

    std::map<HashKey, HashPair> pairs;

    ObjectType type() const override;

    std::string inspect() const override;

    std::shared_ptr<Object> clone() const override;
};

typedef std::function<std::shared_ptr<Object>(std::vector<std::shared_ptr<Object>>)> builtin_fn;

struct Environment;

struct Function : public Object {
    Function(std::vector<std::shared_ptr<Identifier>> p, std::shared_ptr<BlockStatement> b, const std::shared_ptr<Environment> &e);

    std::vector<std::shared_ptr<Identifier>> parameters;
    std::shared_ptr<BlockStatement> body;
    std::shared_ptr<Environment> env;

    ObjectType type() const override;

    std::string inspect() const override;

    std::shared_ptr<Object> clone() const override;
};

struct Builtin : public Object {

    explicit Builtin(builtin_fn v);

    builtin_fn builtin_function;

    ObjectType type() const override;

    std::string inspect() const override;

    std::shared_ptr<Object> clone() const override;
};

struct Array : public Object {
    explicit Array();

    Array(const Array &a);

    std::vector<std::shared_ptr<Object>> elements;

    ObjectType type() const override;

    std::string inspect() const override;

    std::shared_ptr<Object> clone() const override;
};

struct ReturnValue : public Object {
    explicit ReturnValue(std::shared_ptr<Object> v);

    std::shared_ptr<Object> value;

    ObjectType type() const override;

    std::string inspect() const override;

    std::shared_ptr<Object> clone() const override;
};

struct Error : public Object {
    explicit Error(std::string v);

    std::string message;

    ObjectType type() const override;

    std::string inspect() const override;

    std::shared_ptr<Object> clone() const override;
};

struct Integer : public Object, Hashable {
    explicit Integer(int v);

    int value;

    ObjectType type() const override;

    std::string inspect() const override;

    std::shared_ptr<Object> clone() const override;

    HashKey hash_key() const;
};

struct Boolean : public Object, Hashable {
    explicit Boolean(bool v);

    bool value;

    ObjectType type() const override;

    std::string inspect() const override;

    std::shared_ptr<Object> clone() const override;

    HashKey hash_key() const;
};

struct String : public Object, Hashable {
    explicit String(std::string v);

    std::string value;

    ObjectType type() const override;

    std::string inspect() const override;

    std::shared_ptr<Object> clone() const override;

    HashKey hash_key() const;
};

struct Null : public Object {
    ObjectType type() const override;

    std::shared_ptr<Object> clone() const override;

    std::string inspect() const override;
};

#endif //MONKEY_PLUSPLUS_OBJECT_HPP
