// Using Catch2 unit testing library

// Let Catch2 define a main function
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "lexer.hpp"
#include "token.hpp"

TEST_CASE("Test Next Token") {
    std::string input = "=+(){},;";

    auto test_input = Lexer(input);

    Token tests[] = {
        Token{TokenType::ASSIGN, "="},
        Token{TokenType::PLUS, "+"},
        Token{TokenType::LPAREN, "("},
        Token{TokenType::RPAREN, ")"},
        Token{TokenType::LBRACE, "{"},
        Token{TokenType::RBRACE, "}"},
        Token{TokenType::COMMA, ","},
        Token{TokenType::SEMICOLON, ";"},
        Token{TokenType::ENDOFFILE, ""},
    };

    for (Token t: tests) {
        auto tok = test_input.next_token();

        REQUIRE(tok == t);
    }
}