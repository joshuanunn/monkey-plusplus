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
    COLON,
    COMMA,

    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    LBRACKET,
    RBRACKET,

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
    Token() = delete; // Default constructor not meaningful

    Token(const TokenType& t, std::string l);

    Token(const Token& other);

    Token(Token&& other) noexcept;

    Token& operator=(const Token& other);

    Token& operator=(Token&& other) noexcept;

    bool operator==(const Token&) const;

    bool operator!=(const Token&) const;

    TokenType type;

    std::string literal;
};

#endif //MONKEY_PLUSPLUS_TOKEN_HPP
