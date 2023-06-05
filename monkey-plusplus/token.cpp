#include "token.hpp"

std::map<std::string, TokenType> keywords = {
        {"fn",  TokenType::FUNCTION},
        {"let", TokenType::LET}
};

TokenType lookup_ident(const std::string &ident) {

    auto contains = keywords.find(ident);

    // If ident is not a defined keyword, then assumed to be a user identifier
    if (contains == keywords.end()) {
        return TokenType::IDENT;
    }
    return keywords[ident];
}

bool Token::operator==(const Token &t) const {
    return type == t.type && literal == t.literal;
}
