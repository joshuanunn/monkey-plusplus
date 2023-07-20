#ifndef MONKEY_PLUSPLUS_PARSER_HPP
#define MONKEY_PLUSPLUS_PARSER_HPP

#include "ast.hpp"
#include "lexer.hpp"
#include "token.hpp"

struct Parser {
    std::unique_ptr<Lexer> l;

    Token cur_token;
    Token peek_token;

    explicit Parser(std::unique_ptr<Lexer> lexer);

    void next_token();

    std::unique_ptr<LetStatement> parse_let_statement();

    std::unique_ptr<Statement> parse_statement();

    std::unique_ptr<Program> parse_program();

    bool cur_token_is(TokenType t) const;

    bool peek_token_is(TokenType t) const;

    bool expect_peek(TokenType t);
};

#endif //MONKEY_PLUSPLUS_PARSER_HPP
