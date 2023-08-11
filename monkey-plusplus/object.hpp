#ifndef MONKEY_PLUSPLUS_OBJECT_HPP
#define MONKEY_PLUSPLUS_OBJECT_HPP

#include <string>

enum class ObjectType {
    INTEGER_OBJ,
    BOOLEAN_OBJ,
    NULL_OBJ
};

struct Object {
    virtual ~Object() = default;

    virtual ObjectType type() const = 0;

    virtual std::string inspect() const = 0;
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
