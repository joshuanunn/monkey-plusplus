#include <iostream>
#include <memory>
#include "parser.hpp"

Parser::Parser(std::unique_ptr<Lexer> lexer) {
    l = std::move(lexer);
    e = std::vector<std::string>{};

    prefix_parse_fns = std::map<TokenType, prefix_parse_fn>{};
    infix_parse_fns = std::map<TokenType, infix_parse_fn>{};

    register_prefix(TokenType::IDENT, parse_identifier);

    // Read two tokens, so cur_token and peek_token are both set
    next_token();
    next_token();
}

void Parser::next_token() {
    cur_token = peek_token;
    peek_token = l->next_token();
}

std::shared_ptr<LetStatement> Parser::parse_let_statement() {

    if (!expect_peek(TokenType::IDENT)) {
        return nullptr;
    }

    auto stmt = std::make_shared<LetStatement>(LetStatement(
        Identifier(cur_token, cur_token.literal), std::make_unique<Expression>(Expression{})));

    if (!expect_peek(TokenType::ASSIGN)) {
        return nullptr;
    }

    // TODO: for now, skip expressions until we encounter a semi-colon
    while(!cur_token_is(TokenType::SEMICOLON)) {
        next_token();
    }

    return stmt;
}

std::shared_ptr<ReturnStatement> Parser::parse_return_statement() {

    auto stmt = std::make_shared<ReturnStatement>(ReturnStatement(std::make_unique<Expression>(Expression{})));

    next_token();

    // TODO: for now, skip expressions until we encounter a semi-colon
    while(!cur_token_is(TokenType::SEMICOLON)) {
        next_token();
    }

    return stmt;
}

std::shared_ptr<ExpressionStatement> Parser::parse_expression_statement() {

    auto stmt = std::make_shared<ExpressionStatement>(ExpressionStatement(cur_token));

    stmt->expression = std::move(parse_expression(Precedence::LOWEST));

    if (peek_token_is(TokenType::SEMICOLON)) {
        next_token();
    }

    return stmt;
}

std::shared_ptr<Statement> Parser::parse_statement() {
    switch (cur_token.type) {
        case (TokenType::LET):
            return parse_let_statement();
        case (TokenType::RETURN):
            return parse_return_statement();
        default:
            return parse_expression_statement();
    }
}

std::shared_ptr<Expression> Parser::parse_expression(Precedence precedence) {
    auto contains = prefix_parse_fns.find(cur_token.type);

    if (contains == prefix_parse_fns.end()) {
        return nullptr;
    }
    auto prefix = prefix_parse_fns[cur_token.type];

    auto left_exp = prefix(cur_token);

    return left_exp;
}

std::shared_ptr<Expression> parse_identifier(const Token &t) {
    return std::make_shared<Identifier>(Identifier(t, t.literal));
}

std::unique_ptr<Program> Parser::parse_program() {
    auto program = std::make_unique<Program>(Program());

    while (cur_token.type != TokenType::ENDOFFILE) {
        auto stmt = parse_statement();

        if (stmt != nullptr) {
            program->statements.push_back(stmt);
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

void Parser::register_prefix(TokenType token_type, prefix_parse_fn fn) {
    prefix_parse_fns[token_type] = fn;
}

void Parser::register_infix(TokenType token_type, infix_parse_fn fn) {
    infix_parse_fns[token_type] = fn;
}
