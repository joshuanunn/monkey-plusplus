#ifndef MONKEY_PLUSPLUS_OBJECT_HPP
#define MONKEY_PLUSPLUS_OBJECT_HPP

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
    FUNCTION_OBJ
};

std::string objecttype_literal(ObjectType);

struct Object {
    virtual ~Object() = default;

    virtual ObjectType type() const = 0;

    virtual std::string inspect() const = 0;
};

struct Environment;

struct Function : public Object {
    Function(std::vector<std::shared_ptr<Identifier>> p, std::shared_ptr<BlockStatement> b, const std::shared_ptr<Environment> &e);

    std::vector<std::shared_ptr<Identifier>> parameters;
    std::shared_ptr<BlockStatement> body;
    std::shared_ptr<Environment> env;

    ObjectType type() const override;

    std::string inspect() const override;
};

struct ReturnValue : public Object {
    explicit ReturnValue(std::shared_ptr<Object> v);

    std::shared_ptr<Object> value;

    ObjectType type() const override;

    std::string inspect() const override;
};

struct Error : public Object {
    explicit Error(std::string v);

    std::string message;

    ObjectType type() const override;

    std::string inspect() const override;
};

struct Integer : public Object {
    explicit Integer(int v);

    int value;

    ObjectType type() const override;

    std::string inspect() const override;
};

struct Boolean : public Object {
    explicit Boolean(bool v);

    bool value;

    ObjectType type() const override;

    std::string inspect() const override;
};

struct Null : public Object {
    ObjectType type() const override;

    std::string inspect() const override;
};

#endif //MONKEY_PLUSPLUS_OBJECT_HPP
