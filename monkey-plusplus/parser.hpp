#ifndef MONKEY_PLUSPLUS_PARSER_HPP
#define MONKEY_PLUSPLUS_PARSER_HPP

#include <functional>
#include <map>
#include "ast.hpp"
#include "lexer.hpp"
#include "token.hpp"

struct Parser;

typedef std::function<std::shared_ptr<Expression>(Parser *)> prefix_parse_fn;
typedef std::function<std::shared_ptr<Expression>(Parser *, std::shared_ptr<Expression>)> infix_parse_fn;

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
    Token cur_token;
    Token peek_token;

    explicit Parser(std::unique_ptr<Lexer> lexer);

    void next_token();

    std::unique_ptr<LetStatement> parse_let_statement();

    std::unique_ptr<ReturnStatement> parse_return_statement();

    std::unique_ptr<ExpressionStatement> parse_expression_statement();

    std::shared_ptr<BlockStatement> parse_block_statement();

    std::unique_ptr<Statement> parse_statement();

    std::shared_ptr<Expression> parse_expression(Precedence precedence);

    std::vector<std::shared_ptr<Expression>> parse_expression_list(TokenType end);

    std::shared_ptr<Expression> parse_identifier() const;

    std::shared_ptr<Expression> parse_integer_literal();

    std::vector<std::shared_ptr<Identifier>> parse_function_parameters();

    std::shared_ptr<Expression> parse_function_literal();

    std::shared_ptr<Expression> parse_boolean() const;

    std::shared_ptr<Expression> parse_string_literal() const;

    std::shared_ptr<Expression> parse_array_literal();

    std::shared_ptr<Expression> parse_prefix_expression();

    std::shared_ptr<Expression> parse_infix_expression(std::shared_ptr<Expression> left);

    std::shared_ptr<Expression> parse_call_expression(std::shared_ptr<Expression> function);

    std::shared_ptr<Expression> parse_grouped_expression();

    std::shared_ptr<Expression> parse_if_expression();

    std::unique_ptr<Program> parse_program();

    std::vector<std::string> errors() const;

    bool cur_token_is(TokenType t) const;

    bool peek_token_is(TokenType t) const;

    bool expect_peek(TokenType t);

    void peek_error(TokenType t);

    Precedence peek_precedence() const;

    Precedence cur_precedence() const;

    void no_prefix_parse_fn_error(TokenType t);

    void register_prefix(TokenType token_type, prefix_parse_fn fn);

    void register_infix(TokenType token_type, infix_parse_fn fn);

private:
    std::unique_ptr<Lexer> l;
    std::vector<std::string> e;

    std::map<TokenType, prefix_parse_fn> prefix_parse_fns;
    std::map<TokenType, infix_parse_fn> infix_parse_fns;
};

#endif //MONKEY_PLUSPLUS_PARSER_HPP
