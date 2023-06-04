#ifndef MONKEY_PLUSPLUS_TOKEN_HPP
#define MONKEY_PLUSPLUS_TOKEN_HPP

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

struct Token {
    TokenType type;
    std::string literal;

    bool operator==(const Token& t) const {
        return type == t.type && literal == t.literal;
    }
};


#endif //MONKEY_PLUSPLUS_TOKEN_HPP
