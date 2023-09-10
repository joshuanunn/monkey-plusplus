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

TEST_CASE("Test Token Copy Construction") {
    auto tok1 = Token{TokenType::IDENT, "five"};

    // Get object memory locations before copy
    auto address_tok1 = std::addressof(tok1);
    auto address_tok1_type = std::addressof(tok1.type);
    auto address_tok1_literal = std::addressof(tok1.literal);

    // Copy token
    Token tok2{tok1};

    // Assert that tok1 and tok2 are equivalent after copy
    REQUIRE(tok1 == tok2);

    // Assert that tok1 and tok2 are are independent after copy
    REQUIRE(std::addressof(tok1) != std::addressof(tok2));
    REQUIRE(std::addressof(tok1.type) != std::addressof(tok2.type));
    REQUIRE(std::addressof(tok1.literal) != std::addressof(tok2.literal));

    // Assert that both token and member objects are unchanged after copy
    REQUIRE(std::addressof(tok1) == address_tok1);
    REQUIRE(std::addressof(tok1.type) == address_tok1_type);
    REQUIRE(std::addressof(tok1.literal) == address_tok1_literal);
}

TEST_CASE("Test Token Copy Assignment") {
    auto tok1 = Token{TokenType::IDENT, "five"};
    auto tok2 = Token{TokenType::LET, "let"};

    // Get object memory locations before copy
    auto address_tok1 = std::addressof(tok1);
    auto address_tok1_type = std::addressof(tok1.type);
    auto address_tok1_literal = std::addressof(tok1.literal);
    auto address_tok2 = std::addressof(tok2);
    auto address_tok2_type = std::addressof(tok2.type);
    auto address_tok2_literal = std::addressof(tok2.literal);

    // Assert that tok1 and tok2 are not equivalent before copy
    REQUIRE(tok1 != tok2);

    // Copy token
    tok2 = tok1;

    // Assert that tok1 and tok2 are equivalent after copy
    REQUIRE(tok1 == tok2);

    // Assert that tok1 and tok2 are are independent after copy
    REQUIRE(std::addressof(tok1) != std::addressof(tok2));
    REQUIRE(std::addressof(tok1.type) != std::addressof(tok2.type));
    REQUIRE(std::addressof(tok1.literal) != std::addressof(tok2.literal));

    // Assert that both token and member objects are unchanged after copy
    REQUIRE(std::addressof(tok1) == address_tok1);
    REQUIRE(std::addressof(tok1.type) == address_tok1_type);
    REQUIRE(std::addressof(tok1.literal) == address_tok1_literal);
    REQUIRE(std::addressof(tok2) == address_tok2);
    REQUIRE(std::addressof(tok2.type) == address_tok2_type);
    REQUIRE(std::addressof(tok2.literal) == address_tok2_literal);
}

TEST_CASE("Test Token Move Construction") {
    // No checking of member object addresses, as the enum and string members behave as copy on move
    auto tok1 = Token{TokenType::IDENT, "five"};

    // Get tok1 values before move
    auto tok1_type = TokenType{tok1.type};
    auto tok1_literal = std::string{tok1.literal};

    // Move token
    Token tok2{std::move(tok1)};

    // Assert that tok2 members are equivalent to previous values of tok1 after move
    REQUIRE(tok2.type == tok1_type);
    REQUIRE(tok2.literal == tok1_literal);
}

TEST_CASE("Test Token Move Assignment") {
    // No checking of member object addresses, as the enum and string members behave as copy on move
    auto tok1 = Token{TokenType::IDENT, "five"};
    auto tok2 = Token{TokenType::LET, "let"};

    // Get tok1 values before move
    auto tok1_type = TokenType{tok1.type};
    auto tok1_literal = std::string{tok1.literal};

    // Assert that tok1 and tok2 are not equivalent before copy
    REQUIRE(tok1 != tok2);

    // Move token
    tok2 = std::move(tok1);

    // Assert that tok2 members are equivalent to previous values of tok1 after move
    REQUIRE(tok2.type == tok1_type);
    REQUIRE(tok2.literal == tok1_literal);
}
