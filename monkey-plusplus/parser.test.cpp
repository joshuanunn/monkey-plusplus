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

bool test_integer_literal(std::shared_ptr<Expression> il, int value) {
    // Can now cast Expression to an Identifier, as we are confident that it is one
    auto integ = std::dynamic_pointer_cast<IntegerLiteral>(il);

    // Check that we have an Integer Literal by checking if the dynamic pointer cast fails (returns nullptr)
    if (integ == nullptr) {
        std::cerr << "il is not an IntegerLiteral." << std::endl;
        return false;
    }

    if (integ->value != value) {
        std::cerr << "integ->value not " << value << ". got=" << integ->value << std::endl;
        return false;
    }

    if (integ->token_literal() != std::to_string(value)) {
        std::cerr << "integ->token_literal not " << value << ". got=" << integ->token_literal() << std::endl;
        return false;
    }

    return true;
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

TEST_CASE("Test Identifier Expression") {
    std::string input = R"(foobar;)";

    auto l = std::make_unique<Lexer>(Lexer(input));
    auto p = Parser(std::move(l));

    auto program = p.parse_program();

    REQUIRE(test_parser_errors(p));

    if (program->statements.size() != 1) {
        std::cerr << "program has not enough statements. got=" << program->statements.size() << std::endl;
    }
    REQUIRE(program->statements.size() == 1);

    auto stmt = program->statements.at(0);

    // Can now cast Node to a derived ExpressionStatement, as we are confident that it is one
    auto expression_stmt = std::dynamic_pointer_cast<ExpressionStatement>(stmt);

    // Check that we have an Expression Statement by checking if the dynamic pointer cast fails (returns nullptr)
    if (expression_stmt == nullptr) {
        std::cerr << "program.statements.at(0) is not an ExpressionStatement." << std::endl;
    }
    REQUIRE(expression_stmt != nullptr);

    // Can now cast Expression to an Identifier, as we are confident that it is one
    auto ident = std::dynamic_pointer_cast<Identifier>(expression_stmt->expression);

    if (ident->token.type != TokenType::IDENT) {
        std::cerr << "ident->token.type not IDENT. got=" << tokentype_literal(ident->token.type) << std::endl;
    }
    REQUIRE(ident->token.type == TokenType::IDENT);

    if (ident->value != "foobar") {
        std::cerr << "ident->value not foobar. got=" << ident->value << std::endl;
    }
    REQUIRE(ident->value == "foobar");

    if (ident->token_literal() != "foobar") {
        std::cerr << "ident->token_literal() not foobar. got=" << ident->token_literal() << std::endl;
    }
    REQUIRE(ident->token_literal() == "foobar");
}

TEST_CASE("Test Integer Literal Expression") {
    std::string input = R"(5;)";

    auto l = std::make_unique<Lexer>(Lexer(input));
    auto p = Parser(std::move(l));

    auto program = p.parse_program();

    REQUIRE(test_parser_errors(p));

    if (program->statements.size() != 1) {
        std::cerr << "program has not enough statements. got=" << program->statements.size() << std::endl;
    }
    REQUIRE(program->statements.size() == 1);

    auto stmt = program->statements.at(0);

    // Can now cast Node to a derived ExpressionStatement, as we are confident that it is one
    auto expression_stmt = std::dynamic_pointer_cast<ExpressionStatement>(stmt);

    // Check that we have an Expression Statement by checking if the dynamic pointer cast fails (returns nullptr)
    if (!expression_stmt) {
        std::cerr << "program.statements.at(0) is not an ExpressionStatement." << std::endl;
    }
    REQUIRE(expression_stmt);

    // Can now cast Expression to an Identifier, as we are confident that it is one
    auto ident = std::dynamic_pointer_cast<IntegerLiteral>(expression_stmt->expression);
    if (!ident) {
        std::cerr << "ident is set to a nullptr." << std::endl;
    }
    REQUIRE(ident);

    if (ident->token.type != TokenType::INT) {
        std::cerr << "ident->token.type not IDENT. got=" << tokentype_literal(ident->token.type) << std::endl;
    }
    REQUIRE(ident->token.type == TokenType::INT);

    if (ident->value != 5) {
        std::cerr << "ident->value not 5. got=" << ident->value << std::endl;
    }
    REQUIRE(ident->value == 5);

    if (ident->token_literal() != "5") {
        std::cerr << "ident->token_literal() not 5. got=" << ident->token_literal() << std::endl;
    }
    REQUIRE(ident->token_literal() == "5");
}

TEST_CASE("Test Parsing Prefix Expressions") {
    std::vector<std::tuple<std::string, std::string, int>> prefix_tests = {
       std::make_tuple("!5;", "!", 5),
       std::make_tuple("-15;", "-", 15),
    };

    for (const auto &tt: prefix_tests) {
        const auto[tt_input, tt_operator, tt_value] = tt;

        auto l = std::make_unique<Lexer>(Lexer(tt_input));
        auto p = Parser(std::move(l));

        auto program = p.parse_program();

        REQUIRE(test_parser_errors(p));

        if (program->statements.size() != 1) {
            std::cerr << "program.statements does not contain 1 statements. got=" << program->statements.size() << std::endl;
        }
        REQUIRE(program->statements.size() == 1);

        auto stmt = program->statements.at(0);

        // Can now cast Node to a derived ExpressionStatement, as we are confident that it is one
        auto expression_stmt = std::dynamic_pointer_cast<ExpressionStatement>(stmt);

        // Check that we have an Expression Statement by checking if the dynamic pointer cast fails (returns nullptr)
        if (!expression_stmt) {
            std::cerr << "program.statements.at(0) is not an ExpressionStatement." << std::endl;
        }
        REQUIRE(expression_stmt);

        // Can now cast Expression to a PrefixExpression, as we are confident that it is one
        auto expr = std::dynamic_pointer_cast<PrefixExpression>(expression_stmt->expression);
        if (!expr) {
            std::cerr << "expr is set to a nullptr." << std::endl;
        }
        REQUIRE(expr);

        if (expr->op != tt_operator) {
            std::cerr << "exp->operator is not '" << tt_operator << "'. got=" << expr->op << std::endl;
        }
        REQUIRE(expr->op == tt_operator);

        REQUIRE(test_integer_literal(expr->right, tt_value));
    }
}
