#include <memory>
#include "parser.hpp"

Parser::Parser(std::unique_ptr<Lexer> lexer) {
    l = std::move(lexer);
    e = std::vector<std::string>{};

    // Read two tokens, so cur_token and peek_token are both set
    next_token();
    next_token();
}

void Parser::next_token() {
    cur_token = peek_token;
    peek_token = l->next_token();
}

std::unique_ptr<LetStatement> Parser::parse_let_statement() {

    if (!expect_peek(TokenType::IDENT)) {
        return nullptr;
    }

    auto stmt = std::make_unique<LetStatement>(LetStatement(
        Identifier(cur_token, cur_token.literal), Expression{}));

    if (!expect_peek(TokenType::ASSIGN)) {
        return nullptr;
    }

    // TODO: for now, skip expressions until we encounter a semi-colon
    while(!cur_token_is(TokenType::SEMICOLON)) {
        next_token();
    }

    return stmt;
}

std::unique_ptr<Statement> Parser::parse_statement() {
    switch (cur_token.type) {
        case (TokenType::LET):
            return parse_let_statement();
        default:
            return nullptr;
    }
}

std::unique_ptr<Program> Parser::parse_program() {
    auto program = std::make_unique<Program>(Program());

    while (cur_token.type != TokenType::ENDOFFILE) {
        auto stmt = parse_statement();

        if (stmt != nullptr) {
            program->statements.push_back(std::move(stmt));
        }

        next_token();
    }

    return program;
}

std::vector<std::string> Parser::errors() const {
    return e;
}

bool Parser::cur_token_is(TokenType t) const {
    return cur_token.type == t;
}

bool Parser::peek_token_is(TokenType t) const {
    return peek_token.type == t;
}

bool Parser::expect_peek(TokenType t) {
    if (peek_token_is(t)) {
        next_token();
        return true;
    } else {
        peek_error(t);
        return false;
    }
}

void Parser::peek_error(TokenType t) {
    std::string msg = "expected next token to be " + tokentype_literal(t) + ", got " + tokentype_literal(peek_token.type) + " instead";
    e.push_back(msg);
}
