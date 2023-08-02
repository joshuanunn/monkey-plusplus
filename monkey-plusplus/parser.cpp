#include <iostream>
#include <memory>
#include "parser.hpp"

Parser::Parser(std::unique_ptr<Lexer> lexer) {
    l = std::move(lexer);
    e = std::vector<std::string>{};

    prefix_parse_fns = std::map<TokenType, prefix_parse_fn>{};
    infix_parse_fns = std::map<TokenType, infix_parse_fn>{};

    register_prefix(TokenType::IDENT, std::mem_fn(&Parser::parse_identifier));
    register_prefix(TokenType::INT, std::mem_fn(&Parser::parse_integer_literal));
    register_prefix(TokenType::BANG, std::mem_fn(&Parser::parse_prefix_expression));
    register_prefix(TokenType::MINUS,std::mem_fn(&Parser::parse_prefix_expression));

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

std::unique_ptr<ReturnStatement> Parser::parse_return_statement() {

    auto stmt = std::make_unique<ReturnStatement>(ReturnStatement(std::make_unique<Expression>(Expression{})));

    next_token();

    // TODO: for now, skip expressions until we encounter a semi-colon
    while(!cur_token_is(TokenType::SEMICOLON)) {
        next_token();
    }

    return stmt;
}

std::unique_ptr<ExpressionStatement> Parser::parse_expression_statement() {

    auto stmt = std::make_unique<ExpressionStatement>(ExpressionStatement(cur_token));

    stmt->expression = std::move(parse_expression(Precedence::LOWEST));

    if (peek_token_is(TokenType::SEMICOLON)) {
        next_token();
    }

    return stmt;
}

std::unique_ptr<Statement> Parser::parse_statement() {
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
        no_prefix_parse_fn_error(cur_token.type);
        return nullptr;
    }
    auto prefix = prefix_parse_fns[cur_token.type];

    auto left_exp = prefix(this);

    return left_exp;
}

std::shared_ptr<Expression> Parser::parse_identifier() {
    return std::make_shared<Identifier>(Identifier(cur_token, cur_token.literal));
}

std::shared_ptr<Expression> Parser::parse_integer_literal() {
    auto lit = std::make_shared<IntegerLiteral>(IntegerLiteral(cur_token));

    std::size_t pos{};
    try {
        lit->value = std::stoi(cur_token.literal, &pos);
    } catch (std::invalid_argument const &ex) {
        std::cerr << "invalid argument parsing " << ex.what() << "as int." << std::endl;
    } catch (std::out_of_range const &ex) {
        std::cerr << "integer literal " << ex.what() << " to be parsed out of range." << std::endl;
    }

    // TODO: for parse failures, we need to add the relevant error message to the parser error vector and return nullptr

    return lit;
}

std::shared_ptr<Expression> Parser::parse_prefix_expression() {
    auto expression = std::make_shared<PrefixExpression>(PrefixExpression(cur_token, cur_token.literal));

    next_token();

    expression->right = parse_expression(Precedence::PREFIX);

    return expression;
}

std::unique_ptr<Program> Parser::parse_program() {
    auto program = std::make_unique<Program>(Program());

    while (cur_token.type != TokenType::ENDOFFILE) {
        auto stmt = parse_statement();

        if (stmt) {
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

void Parser::no_prefix_parse_fn_error(TokenType t) {
    std::string msg = "no prefix parse function for " + tokentype_literal(t) + " found";
    e.push_back(msg);
}

void Parser::register_prefix(TokenType token_type, prefix_parse_fn fn) {
    prefix_parse_fns[token_type] = std::move(fn);
}

void Parser::register_infix(TokenType token_type, infix_parse_fn fn) {
    infix_parse_fns[token_type] = std::move(fn);
}
