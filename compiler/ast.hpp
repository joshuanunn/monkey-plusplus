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

    virtual std::shared_ptr<Node> clone() const = 0;
};

struct Statement : public Node {
    std::string token_literal() const = 0;

    std::string string() const = 0;

    virtual std::shared_ptr<Node> clone() const = 0;
};

struct Expression : public Node {
    std::string token_literal() const = 0;

    std::string string() const = 0;

    virtual std::shared_ptr<Node> clone() const = 0;
};

struct Identifier : public Expression {
    Identifier(const Token& t, const std::string& v);

    Identifier(const Identifier& other);

    Identifier(Identifier&& other) noexcept;

    Identifier& operator=(const Identifier& other);

    Identifier& operator=(Identifier&& other) noexcept;

    Token token;

    std::string value;

    std::string token_literal() const override;

    std::string string() const override;

    std::shared_ptr<Node> clone() const override;
};

struct IntegerLiteral : public Expression {
    explicit IntegerLiteral(const Token& t);

    IntegerLiteral(const IntegerLiteral& other);

    IntegerLiteral(IntegerLiteral&& other) noexcept;

    IntegerLiteral& operator=(const IntegerLiteral& other);

    IntegerLiteral& operator=(IntegerLiteral&& other) noexcept;

    Token token;

    int value;

    std::string token_literal() const override;

    std::string string() const override;

    std::shared_ptr<Node> clone() const override;
};

struct BooleanLiteral : public Expression {
    BooleanLiteral(const Token& t, bool v);

    BooleanLiteral(const BooleanLiteral& other);

    BooleanLiteral(BooleanLiteral&& other) noexcept;

    BooleanLiteral& operator=(const BooleanLiteral& other);

    BooleanLiteral& operator=(BooleanLiteral&& other) noexcept;

    Token token;

    bool value;

    std::string token_literal() const override;

    std::string string() const override;

    std::shared_ptr<Node> clone() const override;
};

struct StringLiteral : public Expression {
    StringLiteral(const Token& t, std::string v);

    StringLiteral(const StringLiteral& other);

    StringLiteral(StringLiteral&& other) noexcept;

    StringLiteral& operator=(const StringLiteral& other);

    StringLiteral& operator=(StringLiteral&& other) noexcept;

    Token token;

    std::string value;

    std::string token_literal() const override;

    std::string string() const override;

    std::shared_ptr<Node> clone() const override;
};

struct PrefixExpression : public Expression {
    PrefixExpression(const Token& t, const std::string& o);

    PrefixExpression(const PrefixExpression& other);

    PrefixExpression(PrefixExpression&& other) noexcept;

    PrefixExpression& operator=(const PrefixExpression& other);

    PrefixExpression& operator=(PrefixExpression&& other) noexcept;

    Token token;

    std::string op;

    std::shared_ptr<Expression> right;

    std::string token_literal() const override;

    std::string string() const override;

    std::shared_ptr<Node> clone() const override;
};

struct InfixExpression : public Expression {
    InfixExpression(const Token& t, const std::string& o);

    InfixExpression(const InfixExpression& other);

    InfixExpression(InfixExpression&& other) noexcept;

    InfixExpression& operator=(const InfixExpression& other);

    InfixExpression& operator=(InfixExpression&& other) noexcept;

    Token token;

    std::shared_ptr<Expression> left;

    std::string op;

    std::shared_ptr<Expression> right;

    std::string token_literal() const override;

    std::string string() const override;

    std::shared_ptr<Node> clone() const override;
};

struct BlockStatement : public Statement {
    explicit BlockStatement(const Token& t);

    BlockStatement(const BlockStatement& other);

    BlockStatement(BlockStatement&& other) noexcept;

    BlockStatement& operator=(const BlockStatement& other);

    BlockStatement& operator=(BlockStatement&& other) noexcept;

    Token token;

    std::vector<std::shared_ptr<Node>> statements;

    std::string token_literal() const;

    std::string string() const;

    std::shared_ptr<Node> clone() const override;
};

struct IfExpression : public Expression {
    explicit IfExpression(const Token& t);

    IfExpression(const IfExpression& other);

    IfExpression(IfExpression&& other) noexcept;

    IfExpression& operator=(const IfExpression& other);

    IfExpression& operator=(IfExpression&& other) noexcept;

    Token token;

    std::shared_ptr<Expression> condition;

    std::shared_ptr<BlockStatement> consequence;

    std::shared_ptr<BlockStatement> alternative;

    std::string token_literal() const override;

    std::string string() const override;

    std::shared_ptr<Node> clone() const override;
};

struct FunctionLiteral : public Expression {
    explicit FunctionLiteral(const Token& t);

    FunctionLiteral(const FunctionLiteral& other);

    FunctionLiteral(FunctionLiteral&& other) noexcept;

    FunctionLiteral& operator=(const FunctionLiteral& other);

    FunctionLiteral& operator=(FunctionLiteral&& other) noexcept;

    Token token;

    std::vector<std::shared_ptr<Identifier>> parameters;

    std::shared_ptr<BlockStatement> body;

    std::string name;

    std::string token_literal() const override;

    std::string string() const override;

    std::shared_ptr<Node> clone() const override;
};

struct ArrayLiteral : public Expression {
    explicit ArrayLiteral(const Token& t);

    ArrayLiteral(const ArrayLiteral& other);

    ArrayLiteral(ArrayLiteral&& other) noexcept;

    ArrayLiteral& operator=(const ArrayLiteral& other);

    ArrayLiteral& operator=(ArrayLiteral&& other) noexcept;

    Token token;

    std::vector<std::shared_ptr<Expression>> elements;

    std::string token_literal() const override;

    std::string string() const override;

    std::shared_ptr<Node> clone() const override;
};

// Custom comparator for HashLiteral to order keys by Expression string representation
struct comp_hashliteral {
    bool operator()(std::shared_ptr<Expression> key1, std::shared_ptr<Expression> key2) const;
};

struct HashLiteral : public Expression {
    explicit HashLiteral(const Token& t);

    HashLiteral(const HashLiteral& other);

    HashLiteral(HashLiteral&& other) noexcept;

    HashLiteral& operator=(const HashLiteral& other);

    HashLiteral& operator=(HashLiteral&& other) noexcept;

    Token token;

    std::map<std::shared_ptr<Expression>, std::shared_ptr<Expression>, comp_hashliteral> pairs;

    std::string token_literal() const override;

    std::string string() const override;

    std::shared_ptr<Node> clone() const override;
};

struct IndexExpression : public Expression {
    explicit IndexExpression(const Token& t);

    IndexExpression(const IndexExpression& other);

    IndexExpression(IndexExpression&& other) noexcept;

    IndexExpression& operator=(const IndexExpression& other);

    IndexExpression& operator=(IndexExpression&& other) noexcept;

    Token token;

    std::shared_ptr<Expression> left;

    std::shared_ptr<Expression> index;

    std::string token_literal() const override;

    std::string string() const override;

    std::shared_ptr<Node> clone() const override;
};

struct CallExpression : public Expression {
    explicit CallExpression(const Token& t, std::shared_ptr<Expression> f);

    CallExpression(const CallExpression& other);

    CallExpression(CallExpression&& other) noexcept;

    CallExpression& operator=(const CallExpression& other);

    CallExpression& operator=(CallExpression&& other) noexcept;

    Token token;

    std::shared_ptr<Expression> function;

    std::vector<std::shared_ptr<Expression>> arguments;

    std::string token_literal() const override;

    std::string string() const override;

    std::shared_ptr<Node> clone() const override;
};

struct LetStatement : public Statement {
    explicit LetStatement(const Token& t);

    LetStatement(const LetStatement& other);

    LetStatement(LetStatement&& other) noexcept;

    LetStatement& operator=(const LetStatement& other);

    LetStatement& operator=(LetStatement&& other) noexcept;

    Token token;

    std::shared_ptr<Identifier> name;

    std::shared_ptr<Expression> value;

    std::string token_literal() const override;

    std::string string() const override;

    std::shared_ptr<Node> clone() const override;
};

struct ReturnStatement : public Statement {
    explicit ReturnStatement(const Token& t);

    ReturnStatement(const ReturnStatement& other);

    ReturnStatement(ReturnStatement&& other) noexcept;

    ReturnStatement& operator=(const ReturnStatement& other);

    ReturnStatement& operator=(ReturnStatement&& other) noexcept;

    Token token;

    std::shared_ptr<Expression> return_value;

    std::string token_literal() const override;

    std::string string() const override;

    std::shared_ptr<Node> clone() const override;
};

struct ExpressionStatement : public Statement {
    explicit ExpressionStatement(const Token& t);

    ExpressionStatement(const ExpressionStatement& other);

    ExpressionStatement(ExpressionStatement&& other) noexcept;

    ExpressionStatement& operator=(const ExpressionStatement& other);

    ExpressionStatement& operator=(ExpressionStatement&& other) noexcept;

    Token token;

    std::shared_ptr<Expression> expression;

    std::string token_literal() const override;

    std::string string() const override;

    std::shared_ptr<Node> clone() const override;
};

struct Program : public Node {
    Program() = default;

    Program(const Program& other);

    Program(Program&& other) noexcept;

    Program& operator=(const Program& other);

    Program& operator=(Program&& other) noexcept;

    std::vector<std::shared_ptr<Node>> statements;

    std::string token_literal() const override;

    std::string string() const override;

    std::shared_ptr<Node> clone() const override;
};

#endif //MONKEY_PLUSPLUS_AST_HPP
