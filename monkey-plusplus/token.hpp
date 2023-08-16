#ifndef MONKEY_PLUSPLUS_TOKEN_HPP
#define MONKEY_PLUSPLUS_TOKEN_HPP

#include <map>
#include <string>

enum class TokenType {
    ILLEGAL,
    ENDOFFILE,

    // Identifiers + literals
    IDENT,
    INT,
    STRING,

    // Operators
    ASSIGN,
    PLUS,
    MINUS,
    BANG,
    ASTERISK,
    SLASH,

    LT,
    GT,
    EQ,
    NOT_EQ,

    // Delimiters
    SEMICOLON,
    COMMA,

    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,

    // Keywords
    FUNCTION,
    LET,
    TRUE,
    FALSE,
    IF,
    ELSE,
    RETURN
};

std::string tokentype_literal(TokenType);

extern std::map<std::string, TokenType> keywords;

TokenType lookup_ident(const std::string &ident);

struct Token {
    TokenType type;
    std::string literal;

    bool operator==(const Token &) const;
};

#endif //MONKEY_PLUSPLUS_TOKEN_HPP
