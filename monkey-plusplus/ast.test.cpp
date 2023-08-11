// Using Catch2 unit testing library

// Let Catch2 define a main function
#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"

#include "ast.hpp"
#include "token.hpp"

TEST_CASE("Test String") {

    auto program = Program();

    auto stmt = std::make_unique<LetStatement>(LetStatement{Token{TokenType::LET, "let"}});

    stmt->name = std::make_unique<Identifier>(Identifier(Token{TokenType::IDENT, "myVar"}, "myVar"));
    stmt->value = std::make_shared<Identifier>(Identifier(Token{TokenType::IDENT, "anotherVar"}, "anotherVar"));

    program.statements.push_back(std::move(stmt));

    if (program.string() != "let myVar = anotherVar;") {
        std::cerr << "program.string wrong. got=" << program.string() << std::endl;
    }

    REQUIRE(program.string() == "let myVar = anotherVar;");
}
