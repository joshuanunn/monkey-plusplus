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

if (5 < 10) {
    return true;
} else {
    return false;
}

10 == 10;
10 != 9;
"foobar"
"foo bar"
[1, 2];
{"foo": "bar"}
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
            Token{TokenType::IF, "if"},
            Token{TokenType::LPAREN, "("},
            Token{TokenType::INT, "5"},
            Token{TokenType::LT, "<"},
            Token{TokenType::INT, "10"},
            Token{TokenType::RPAREN, ")"},
            Token{TokenType::LBRACE, "{"},
            Token{TokenType::RETURN, "return"},
            Token{TokenType::TRUE, "true"},
            Token{TokenType::SEMICOLON, ";"},
            Token{TokenType::RBRACE, "}"},
            Token{TokenType::ELSE, "else"},
            Token{TokenType::LBRACE, "{"},
            Token{TokenType::RETURN, "return"},
            Token{TokenType::FALSE, "false"},
            Token{TokenType::SEMICOLON, ";"},
            Token{TokenType::RBRACE, "}"},
            Token{TokenType::INT, "10"},
            Token{TokenType::EQ, "=="},
            Token{TokenType::INT, "10"},
            Token{TokenType::SEMICOLON, ";"},
            Token{TokenType::INT, "10"},
            Token{TokenType::NOT_EQ, "!="},
            Token{TokenType::INT, "9"},
            Token{TokenType::SEMICOLON, ";"},
            Token{TokenType::STRING, "foobar"},
            Token{TokenType::STRING, "foo bar"},
            Token{TokenType::LBRACKET, "["},
            Token{TokenType::INT, "1"},
            Token{TokenType::COMMA, ","},
            Token{TokenType::INT, "2"},
            Token{TokenType::RBRACKET, "]"},
            Token{TokenType::SEMICOLON, ";"},
            Token{TokenType::LBRACE, "{"},
            Token{TokenType::STRING, "foo"},
            Token{TokenType::COLON, ":"},
            Token{TokenType::STRING, "bar"},
            Token{TokenType::RBRACE, "}"},
            Token{TokenType::ENDOFFILE, ""},
    };

    for (const Token &t: tests) {
        auto tok = test_input.next_token();
        if (!(tok == t)) {
            std::cerr << "tokentype wrong. expected='" << tokentype_literal(t.type) << "(" << t.literal << ")', got='"
                      << tokentype_literal(tok.type) << "(" << tok.literal << ")'" << std::endl;
        }
        REQUIRE(tok == t);
    }
}

TEST_CASE("Test Token Equivalence Semantics") {
    auto tok1 = Token{TokenType::IDENT, "five"};
    auto tok2 = Token{TokenType::IDENT, "ten"};
    auto tok3 = Token{TokenType::IDENT, "five"};
    auto tok4 = Token{TokenType::STRING, "five"};
    auto tok5 = Token{TokenType::STRING, "five"};

    REQUIRE(tok1 != tok2);
    REQUIRE(tok1 == tok3);
    REQUIRE(tok1 != tok4);
    REQUIRE(tok1 != tok5);
    REQUIRE(tok2 != tok3);
    REQUIRE(tok2 != tok4);
    REQUIRE(tok2 != tok5);
    REQUIRE(tok3 != tok4);
    REQUIRE(tok3 != tok5);
    REQUIRE(tok4 == tok5);

    tok2.literal = "five";
    tok4.type = TokenType::IDENT;
    tok5.type = TokenType::IDENT;

    REQUIRE(tok1 == tok2);
    REQUIRE(tok1 == tok3);
    REQUIRE(tok1 == tok4);
    REQUIRE(tok1 == tok5);
}
