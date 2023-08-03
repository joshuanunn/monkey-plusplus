#include "ast.hpp"

std::string Statement::token_literal() const {
    return "<Statement base class>";
}

std::string Statement::string() const {
    return "<Statement base class>";
}

std::string Expression::token_literal() const {
    return "<Expression base class>";
}

std::string Expression::string() const {
    return "<Expression base class>";
}

std::string LetStatement::token_literal() const {
    return token.literal;
}

std::string LetStatement::string() const {
    std::string out;

    out.append(token_literal() + " ");
    out.append(name->string());
    out.append(" = ");
    out.append(value->string());
    out.append(";");

    return out;
}

LetStatement::LetStatement(const Identifier &n, std::shared_ptr<Expression> v) {
    token = Token{TokenType::LET, "let"};
    name = std::make_unique<Identifier>(n);
    value = std::move(v);
}

std::string ReturnStatement::token_literal() const {
    return token.literal;
}

std::string ReturnStatement::string() const {
    std::string out;

    out.append(token_literal() + " ");
    out.append(return_value->string());
    out.append(";");

    return out;
}

ReturnStatement::ReturnStatement(std::shared_ptr<Expression> v) {
    token = Token{TokenType::RETURN, "return"};
    return_value = std::move(v);
}

std::string ExpressionStatement::token_literal() const {
    return token.literal;
}

std::string ExpressionStatement::string() const {
    std::string out;

    out.append(expression->string());

    return out;
}

ExpressionStatement::ExpressionStatement(const Token &t) : token{t} {}

std::string Identifier::token_literal() const {
    return token.literal;
}

std::string Identifier::string() const {
    return value;
}

Identifier::Identifier(const Token &t, const std::string &v) : token{t}, value{v} {}

std::string IntegerLiteral::token_literal() const {
    return token.literal;
}

std::string IntegerLiteral::string() const {
    return std::to_string(value);
}

IntegerLiteral::IntegerLiteral(const Token &t) : token{t}, value{} {}

std::string PrefixExpression::token_literal() const {
    return token.literal;
}

std::string PrefixExpression::string() const {
    return "(" + op + right->string() + ")";
}

PrefixExpression::PrefixExpression(const Token &t, const std::string &o) : token{t}, op{o} {}

std::string InfixExpression::token_literal() const {
    return token.literal;
}

std::string InfixExpression::string() const {
    return "(" + left->string() + " " + op + " " + right->string() + ")";
}

InfixExpression::InfixExpression(const Token &t, const std::string &o) : token{t}, op{o} {}

std::string Program::token_literal() const {
    if (!statements.empty()) {
        return statements.at(0)->token_literal();
    } else {
        return "";
    }
}

std::string Program::string() const {
    std::string out;

    for (const auto &s: statements) {
        out.append(s->string());
    }

    return out;
}
