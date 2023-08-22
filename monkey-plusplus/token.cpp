#include "token.hpp"

std::map<std::string, TokenType> keywords = {
        {"fn",     TokenType::FUNCTION},
        {"let",    TokenType::LET},
        {"true",   TokenType::TRUE},
        {"false",  TokenType::FALSE},
        {"if",     TokenType::IF},
        {"else",   TokenType::ELSE},
        {"return", TokenType::RETURN}
};

TokenType lookup_ident(const std::string &ident) {
    auto contains = keywords.find(ident);

    // If ident is not a defined keyword, then assumed to be a user identifier
    if (contains == keywords.end()) {
        return TokenType::IDENT;
    }
    return keywords[ident];
}

Token::Token(const TokenType &t, std::string l) : type{t}, literal{std::move(l)} {}

Token::Token(const Token &other) : type{other.type}, literal{std::string{other.literal}} {}

Token::Token(Token &&other) noexcept : type(other.type), literal(std::move(other.literal)) {
    other.type = TokenType::ILLEGAL;
    other.literal = "";
}

Token& Token::operator=(const Token& other) {
    if (this == &other) return *this;

    type = other.type;
    literal = other.literal;

    return *this;
}

Token& Token::operator=(Token&& other) noexcept {
    if (this == &other) return *this;

    type = other.type;
    literal = std::move(other.literal);

    other.type = TokenType::ILLEGAL;
    other.literal = "";

    return *this;
}

bool Token::operator==(const Token& t) const {
    return type == t.type && literal == t.literal;
}

bool Token::operator!=(const Token& t) const {
    return !(type == t.type && literal == t.literal);
}

std::map<TokenType, std::string> tokentype_literals = {
        {TokenType::ILLEGAL,   "ILLEGAL"},
        {TokenType::ENDOFFILE, "EOF"},
        {TokenType::IDENT,     "IDENT"},
        {TokenType::INT,       "INT"},
        {TokenType::STRING,    "STRING"},
        {TokenType::ASSIGN,    "="},
        {TokenType::PLUS,      "+"},
        {TokenType::MINUS,     "-"},
        {TokenType::BANG,      "!"},
        {TokenType::ASTERISK,  "*"},
        {TokenType::SLASH,     "/"},
        {TokenType::LT,        "<"},
        {TokenType::GT,        ">"},
        {TokenType::EQ,        "=="},
        {TokenType::NOT_EQ,    "!="},
        {TokenType::SEMICOLON, ";"},
        {TokenType::COLON,     ":"},
        {TokenType::COMMA,     ","},
        {TokenType::LPAREN,    "("},
        {TokenType::RPAREN,    ")"},
        {TokenType::LBRACE,    "{"},
        {TokenType::RBRACE,    "}"},
        {TokenType::LBRACKET,  "["},
        {TokenType::RBRACKET,  "]"},
        {TokenType::FUNCTION,  "FUNCTION"},
        {TokenType::LET,       "LET"},
        {TokenType::TRUE,      "TRUE"},
        {TokenType::FALSE,     "FALSE"},
        {TokenType::IF,        "IF"},
        {TokenType::ELSE,      "ELSE"},
        {TokenType::RETURN,    "RETURN"},
};

std::string tokentype_literal(TokenType t) {
    auto contains = tokentype_literals.find(t);

    // If ident is not a defined keyword, then return warning
    if (contains == tokentype_literals.end()) {
        return "TOKEN_NOT_DEFINED";
    }
    return tokentype_literals[t];
}
