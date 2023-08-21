// Using Catch2 unit testing library

// Let Catch2 define a main function
#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"

#include "token.hpp"

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
