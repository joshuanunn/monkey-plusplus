#include "ast.hpp"

std::string Statement::token_literal() {
    return "Statement";
}

std::string Expression::token_literal() {
    return "Expression";
}

std::string LetStatement::token_literal() {
    return token.literal;
}

LetStatement::LetStatement(Identifier n, Expression v) {
    token = Token{TokenType::LET, "let"};
    name = std::make_unique<Identifier>(std::move(n));
    value = std::move(v);
}

std::string Identifier::token_literal() {
    return token.literal;
}

Identifier::Identifier(Token t, std::string v) {
    token = std::move(t);
    value = std::move(v);
}

Program::Program() {
    statements = std::vector<std::unique_ptr<Node>>();
}

std::string Program::token_literal() {
    if (!statements.empty()) {
        return statements.at(0)->token_literal();
    } else {
        return "";
    }
}
