#ifndef MONKEY_PLUSPLUS_AST_HPP
#define MONKEY_PLUSPLUS_AST_HPP

#include <memory>
#include <string>
#include <vector>
#include "token.hpp"

struct Node {
    virtual ~Node() = default;
    virtual std::string token_literal() const = 0;
};

struct Statement : public Node {
    std::string token_literal() const override;
};

struct Expression : public Node {
    std::string token_literal() const override;
};

struct Identifier : public Expression {
    Identifier(Token t, std::string v);

    Token token;
    std::string value;

    std::string token_literal() const override;
};

struct LetStatement : public Statement {
    LetStatement(Identifier n, Expression v);

    Token token;
    std::unique_ptr<Identifier> name;
    Expression value;

    std::string token_literal() const override;
};

struct Program {
    Program();

    std::vector<std::unique_ptr<Node>> statements;

    std::string token_literal() const;
};

#endif //MONKEY_PLUSPLUS_AST_HPP
