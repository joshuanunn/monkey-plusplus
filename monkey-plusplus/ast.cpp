#include "ast.hpp"

std::string Statement::token_literal() const {
    return "Statement";
}

std::string Statement::string() const {
    return "Statement"; // TODO: placeholder to allow compilation
}

std::string Expression::token_literal() const {
    return "Expression";
}

std::string Expression::string() const {
    return "Expression"; // TODO: placeholder to allow compilation
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

LetStatement::LetStatement(Identifier n, std::unique_ptr<Expression> v) {
    token = Token{TokenType::LET, "let"};
    name = std::make_unique<Identifier>(std::move(n));
    value = std::move(v);
}

std::string ReturnStatement::token_literal() const {
    return token.literal;
}

std::string ReturnStatement::string() const {
    std::string out;

    out.append(token_literal() + " ");

    out.append(return_value.string());
    out.append(";");

    return out;
}

ReturnStatement::ReturnStatement(Expression v) {
    token = Token{TokenType::RETURN, "return"};
    return_value = std::move(v);
}

std::string ExpressionStatement::token_literal() const {
    return token.literal;
}

std::string ExpressionStatement::string() const {
    std::string out;

    out.append(expression.string());

    return out;
}

ExpressionStatement::ExpressionStatement(Expression v) {
    token = Token{TokenType::ILLEGAL, "expression"}; // TODO: work out how best to define this token
    expression = std::move(v);
}

std::string Identifier::token_literal() const {
    return token.literal;
}

std::string Identifier::string() const {
    return value;
}

Identifier::Identifier(Token t, std::string v) {
    token = std::move(t);
    value = std::move(v);
}

Program::Program() {
    statements = std::vector<std::unique_ptr<Node>>();
}

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
