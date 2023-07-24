// Using Catch2 unit testing library

// Let Catch2 define a main function
#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"

#include "parser.hpp"

bool test_let_statement(const Node &s, const std::string &name) {
    if (s.token_literal() != "let") {
        std::cerr << "s->token_literal not 'let'. got=" << s.token_literal() << std::endl;
        return false;
    }

    // Can now cast Node to a derived LetStatement, as we are confident that it is one
    auto let_stmt = dynamic_cast<const LetStatement*>(&s);

    if (let_stmt->name->value != name) {
        std::cerr << "let_stmt.name.value not '" << name << "'. got=" << let_stmt->name->value << std::endl;
        return false;
    }

    if (let_stmt->name->token_literal() != name) {
        std::cerr << "let_stmt.name.token_literal() not '" << name << "'. got=" << let_stmt->name->token_literal() << std::endl;
        return false;
    }

    return true;
}

bool test_parser_errors(const Parser &p) {
    auto errors = p.errors();

    if (errors.empty()) {
        return true;
    }

    std::cerr << "parser has " << errors.size() << " errors" << std::endl;
    for (auto &e: errors) {
        std::cerr << "parser error: " << e << std::endl;
    }
    return false;
}

TEST_CASE("Test Let Statements") {
    std::string input = R"(
let x = 5;
let y = 10;
let foobar = 838383;
)";
    auto l = std::make_unique<Lexer>(Lexer(input));
    auto p = Parser(std::move(l));

    auto program = p.parse_program();

    REQUIRE(test_parser_errors(p));

    std::vector<std::string> tests = {
        "x",
        "y",
        "foobar",
    };

    for (int i = 0; i < tests.size(); i++) {
        const auto &tt = tests.at(i);
        const auto &stmt = *program->statements.at(i);
        REQUIRE(test_let_statement(stmt, tt));
    }
}

TEST_CASE("Test Return Statements") {
    std::string input = R"(
return 5;
return 10;
return 993322;
)";
    auto l = std::make_unique<Lexer>(Lexer(input));
    auto p = Parser(std::move(l));

    auto program = p.parse_program();

    REQUIRE(test_parser_errors(p));

    if (program->statements.size() != 3) {
        std::cerr << "program->statements does not contain 3 statements. got=" << program->statements.size() << std::endl;
    }

    REQUIRE(program->statements.size() == 3);

    for (const auto &stmt: program->statements) {
        const auto &return_stmt = *stmt;

        if (return_stmt.token_literal() != "return") {
            std::cerr << "return_stmt.token_literal not 'return'. got=" << return_stmt.token_literal() << std::endl;
        }

        REQUIRE(return_stmt.token_literal() == "return");
    }
}
