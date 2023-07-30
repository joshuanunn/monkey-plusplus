#ifndef MONKEY_PLUSPLUS_PARSER_HPP
#define MONKEY_PLUSPLUS_PARSER_HPP

#include <functional>
#include <map>
#include "ast.hpp"
#include "lexer.hpp"
#include "token.hpp"

typedef std::function<std::shared_ptr<Expression>(const Token &t)> prefix_parse_fn;
typedef std::function<std::shared_ptr<Expression>(const Token &t)> infix_parse_fn;

enum class Precedence {
    LOWEST,
    EQUALS,         // ==
    LESSGREATER,    // > or <
    SUM,            // +
    PRODUCT,        // *
    PREFIX,         // -X or !X
    CALL            // my_function(X)
};

struct Parser {
    std::unique_ptr<Lexer> l;
    std::vector<std::string> e;

    Token cur_token;
    Token peek_token;

    std::map<TokenType, prefix_parse_fn> prefix_parse_fns;
    std::map<TokenType, infix_parse_fn> infix_parse_fns;

    explicit Parser(std::unique_ptr<Lexer> lexer);

    void next_token();

    std::shared_ptr<LetStatement> parse_let_statement();

    std::shared_ptr<ReturnStatement> parse_return_statement();

    std::shared_ptr<ExpressionStatement> parse_expression_statement();

    std::shared_ptr<Statement> parse_statement();

    std::shared_ptr<Expression> parse_expression(Precedence precedence);

    std::unique_ptr<Program> parse_program();

    std::vector<std::string> errors() const;

    bool cur_token_is(TokenType t) const;

    bool peek_token_is(TokenType t) const;

    bool expect_peek(TokenType t);

    void peek_error(TokenType t);

    void register_prefix(TokenType token_type, prefix_parse_fn fn);

    void register_infix(TokenType token_type, infix_parse_fn fn);
};

std::shared_ptr<Expression> parse_identifier(const Token &t);

#endif //MONKEY_PLUSPLUS_PARSER_HPP
