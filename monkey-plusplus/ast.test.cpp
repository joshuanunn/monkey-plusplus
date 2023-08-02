// Using Catch2 unit testing library

// Let Catch2 define a main function
#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"

#include "ast.hpp"
#include "token.hpp"

TEST_CASE("Test String") {

    auto program = Program();

    program.statements.push_back(
            std::make_unique<LetStatement>(
                    LetStatement(
                            Identifier(Token{TokenType::IDENT, "myVar"}, "myVar"),
                            std::make_unique<Identifier>(
                                    Identifier(Token{TokenType::IDENT, "anotherVar"}, "anotherVar"))
                    )
            )
    );

    if (program.string() != "let myVar = anotherVar;") {
        std::cerr << "program.string wrong. got=" << program.string() << std::endl;
    }

    REQUIRE(program.string() == "let myVar = anotherVar;");
}
