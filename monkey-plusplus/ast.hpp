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

struct BooleanLiteral : public Expression {
    BooleanLiteral(const Token &t, bool v);

    Token token;
    bool value;

    std::string token_literal() const override;

    std::string string() const override;
};

struct StringLiteral : public Expression {
    StringLiteral(const Token &t, std::string v);

    Token token;
    std::string value;

    std::string token_literal() const override;

    std::string string() const override;
};

struct PrefixExpression : public Expression {
    PrefixExpression(const Token &t, const std::string &o);

    Token token;
    std::string op;
    std::shared_ptr<Expression> right;

    std::string token_literal() const override;

    std::string string() const override;
};

struct InfixExpression : public Expression {
    InfixExpression(const Token &t, const std::string &o);

    Token token;
    std::shared_ptr<Expression> left;
    std::string op;
    std::shared_ptr<Expression> right;

    std::string token_literal() const override;

    std::string string() const override;
};

struct BlockStatement : public Statement {
    BlockStatement(const Token &t);

    Token token;
    std::vector<std::shared_ptr<Node>> statements;

    std::string token_literal() const;

    std::string string() const;
};

struct IfExpression : public Expression {
    explicit IfExpression(const Token &t);

    Token token;
    std::shared_ptr<Expression> condition;
    std::shared_ptr<BlockStatement> consequence;
    std::shared_ptr<BlockStatement> alternative;

    std::string token_literal() const override;

    std::string string() const override;
};

struct FunctionLiteral : public Expression {
    explicit FunctionLiteral(const Token &t);

    Token token;
    std::vector<std::shared_ptr<Identifier>> parameters;
    std::shared_ptr<BlockStatement> body;

    std::string token_literal() const override;

    std::string string() const override;
};

struct CallExpression : public Expression {
    explicit CallExpression(const Token &t, std::shared_ptr<Expression> f);

    Token token;
    std::shared_ptr<Expression> function;
    std::vector<std::shared_ptr<Expression>> arguments;

    std::string token_literal() const override;

    std::string string() const override;
};

struct LetStatement : public Statement {
    explicit LetStatement(const Token &t);

    Token token;
    std::unique_ptr<Identifier> name;
    std::shared_ptr<Expression> value;

    std::string token_literal() const override;

    std::string string() const override;
};

struct ReturnStatement : public Statement {
    explicit ReturnStatement(const Token &t);

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

struct Program : public Node {
    std::vector<std::shared_ptr<Node>> statements;

    std::string token_literal() const;

    std::string string() const;
};

#endif //MONKEY_PLUSPLUS_AST_HPP
