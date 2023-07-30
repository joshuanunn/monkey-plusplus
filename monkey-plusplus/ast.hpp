#ifndef MONKEY_PLUSPLUS_AST_HPP
#define MONKEY_PLUSPLUS_AST_HPP

#include <memory>
#include <string>
#include <vector>
#include "token.hpp"

struct Node {
    virtual ~Node() = default;
    virtual std::string token_literal() const = 0;
    virtual std::string string() const = 0;
};

struct Statement : public Node {
    std::string token_literal() const override;
    std::string string() const override;
};

struct Expression : public Node {
    std::string token_literal() const override;
    std::string string() const override;
};

struct Identifier : public Expression {
    Identifier(const Token &t, const std::string &v);

    Token token;
    std::string value;

    std::string token_literal() const override;
    std::string string() const override;
};

struct IntegerLiteral : public Expression {
    explicit IntegerLiteral(const Token &t);

    Token token;
    int value;

    std::string token_literal() const override;
    std::string string() const override;
};

struct LetStatement : public Statement {
    LetStatement(const Identifier &n, std::shared_ptr<Expression> v);

    Token token;
    std::unique_ptr<Identifier> name;
    std::shared_ptr<Expression> value;

    std::string token_literal() const override;
    std::string string() const override;
};

struct ReturnStatement : public Statement {
    explicit ReturnStatement(std::shared_ptr<Expression> v);

    Token token;
    std::shared_ptr<Expression> return_value;

    std::string token_literal() const override;
    std::string string() const override;
};

struct ExpressionStatement : public Statement {
    explicit ExpressionStatement(const Token &t);

    Token token;
    std::shared_ptr<Expression> expression;

    std::string token_literal() const override;
    std::string string() const override;
};

struct Program {
    Program();

    std::vector<std::shared_ptr<Node>> statements;

    std::string token_literal() const;
    std::string string() const;
};

#endif //MONKEY_PLUSPLUS_AST_HPP
