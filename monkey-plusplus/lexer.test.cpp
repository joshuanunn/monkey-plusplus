// Using Catch2 unit testing library

// Let Catch2 define a main function
#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"

#include "lexer.hpp"

TEST_CASE("Test Next Token") {
    std::string input = R"(let five = 5;
let ten = 10;

let add = fn(x, y) {
  x + y;
};

let result = add(five, ten);
!-/*5;
5 < 10 > 5;
)";

    auto test_input = Lexer(input);

    Token tests[] = {
            Token{TokenType::LET, "let"},
            Token{TokenType::IDENT, "five"},
            Token{TokenType::ASSIGN, "="},
            Token{TokenType::INT, "5"},
            Token{TokenType::SEMICOLON, ";"},
            Token{TokenType::LET, "let"},
            Token{TokenType::IDENT, "ten"},
            Token{TokenType::ASSIGN, "="},
            Token{TokenType::INT, "10"},
            Token{TokenType::SEMICOLON, ";"},
            Token{TokenType::LET, "let"},
            Token{TokenType::IDENT, "add"},
            Token{TokenType::ASSIGN, "="},
            Token{TokenType::FUNCTION, "fn"},
            Token{TokenType::LPAREN, "("},
            Token{TokenType::IDENT, "x"},
            Token{TokenType::COMMA, ","},
            Token{TokenType::IDENT, "y"},
            Token{TokenType::RPAREN, ")"},
            Token{TokenType::LBRACE, "{"},
            Token{TokenType::IDENT, "x"},
            Token{TokenType::PLUS, "+"},
            Token{TokenType::IDENT, "y"},
            Token{TokenType::SEMICOLON, ";"},
            Token{TokenType::RBRACE, "}"},
            Token{TokenType::SEMICOLON, ";"},
            Token{TokenType::LET, "let"},
            Token{TokenType::IDENT, "result"},
            Token{TokenType::ASSIGN, "="},
            Token{TokenType::IDENT, "add"},
            Token{TokenType::LPAREN, "("},
            Token{TokenType::IDENT, "five"},
            Token{TokenType::COMMA, ","},
            Token{TokenType::IDENT, "ten"},
            Token{TokenType::RPAREN, ")"},
            Token{TokenType::SEMICOLON, ";"},
            Token{TokenType::BANG, "!"},
            Token{TokenType::MINUS, "-"},
            Token{TokenType::SLASH, "/"},
            Token{TokenType::ASTERISK, "*"},
            Token{TokenType::INT, "5"},
            Token{TokenType::SEMICOLON, ";"},
            Token{TokenType::INT, "5"},
            Token{TokenType::LT, "<"},
            Token{TokenType::INT, "10"},
            Token{TokenType::GT, ">"},
            Token{TokenType::INT, "5"},
            Token{TokenType::SEMICOLON, ";"},
            Token{TokenType::ENDOFFILE, ""},
    };

    for (const Token& t: tests) {
        auto tok = test_input.next_token();

        REQUIRE(tok == t);
    }
}
