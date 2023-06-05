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

    // Operators
    ASSIGN,
    PLUS,

    // Delimiters
    COMMA,
    SEMICOLON,

    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,

    // Keywords
    FUNCTION,
    LET
};

extern std::map<std::string, TokenType> keywords;

TokenType lookup_ident(const std::string &ident);

struct Token {
    TokenType type;
    std::string literal;

    bool operator==(const Token &) const;
};

#endif //MONKEY_PLUSPLUS_TOKEN_HPP
