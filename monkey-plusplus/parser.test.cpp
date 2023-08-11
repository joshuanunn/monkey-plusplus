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
    auto let_stmt = dynamic_cast<const LetStatement *>(&s);

    if (let_stmt->name->value != name) {
        std::cerr << "let_stmt.name.value not '" << name << "'. got=" << let_stmt->name->value << std::endl;
        return false;
    }

    if (let_stmt->name->token_literal() != name) {
        std::cerr << "let_stmt.name.token_literal() not '" << name << "'. got=" << let_stmt->name->token_literal()
                  << std::endl;
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
    // Cast Expression to an Integer, as this is what we are expecting
    auto integ = std::dynamic_pointer_cast<IntegerLiteral>(il);

    // Check that we have an Integer Literal by checking if the dynamic pointer cast fails (returns nullptr)
    if (!integ) {
        std::cerr << "il is not an IntegerLiteral." << std::endl;
        return false;
    }

    if (integ->token.type != TokenType::INT) {
        std::cerr << "ident->token.type not INT. got=" << tokentype_literal(integ->token.type) << std::endl;
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

bool test_identifier(std::shared_ptr<Expression> exp, std::string value) {
    // Cast Expression to an Identifier, as this is what we are expecting
    auto ident = std::dynamic_pointer_cast<Identifier>(exp);

    if (!ident) {
        std::cerr << "ident is nullptr, failed cast to Identifier" << std::endl;
        return false;
    }

    if (ident->token.type != TokenType::IDENT) {
        std::cerr << "ident->token.type not IDENT. got=" << tokentype_literal(ident->token.type) << std::endl;
        return false;
    }

    if (ident->value != value) {
        std::cerr << "ident->value not " << value << ". got=" << ident->value << std::endl;
        return false;
    }

    if (ident->token_literal() != value) {
        std::cerr << "ident->token_literal() not " << value << ". got=" << ident->token_literal() << std::endl;
        return false;
    }

    return true;
}

bool test_boolean_expression(std::shared_ptr<Expression> exp, bool value) {
    // Cast Expression to a Boolean, as this is what we are expecting
    auto bo = std::dynamic_pointer_cast<Boolean>(exp);

    // Check that we have an Boolean by checking if the dynamic pointer cast fails (returns nullptr)
    if (!bo) {
        std::cerr << "bo is not a Boolean." << std::endl;
        return false;
    }

    if (!((bo->token.type == TokenType::TRUE) || (bo->token.type == TokenType::FALSE))) {
        std::cerr << "ident->token.type not TRUE or FALSE. got=" << tokentype_literal(bo->token.type) << std::endl;
        return false;
    }

    if (bo->value != value) {
        std::cerr << "bo->value not " << value << ". got=" << bo->value << std::endl;
        return false;
    }

    auto str_value = value ? "true" : "false";
    if (bo->token_literal() != str_value) {
        std::cerr << "bo->token_literal not " << str_value << ". got=" << bo->token_literal() << std::endl;
        return false;
    }

    return true;
}

bool test_literal_expression(std::shared_ptr<Expression> exp, int value) {
    return test_integer_literal(exp, value);
}

bool test_literal_expression(std::shared_ptr<Expression> exp, const char *value) {
    return test_identifier(exp, value);
}

bool test_literal_expression(std::shared_ptr<Expression> exp, std::string value) {
    return test_identifier(exp, value);
}

bool test_literal_expression(std::shared_ptr<Expression> exp, bool value) {
    return test_boolean_expression(exp, value);
}

template <class T>
bool test_infix_expression(std::shared_ptr<Expression> exp, T left, std::string op, T right) {
    // Cast Expression to a InfixExpression, as it should be one
    auto op_exp = std::dynamic_pointer_cast<InfixExpression>(exp);
    if (!op_exp) {
        std::cerr << "op_exp is set to a nullptr." << std::endl;
        return false;
    }

    if (!test_literal_expression(op_exp->left, left)) {
        return false;
    }

    if (op_exp->op != op) {
        std::cerr << "op_exp->op is not '" << op << "'. got=" << op_exp->op << std::endl;
        return false;
    }

    if (!test_literal_expression(op_exp->right, right)) {
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
        std::cerr << "program->statements does not contain 3 statements. got=" << program->statements.size()
                  << std::endl;
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

    // Check Identifier in Expression Statement is correct
    std::string expected = "foobar";
    REQUIRE(test_literal_expression(expression_stmt->expression, expected));
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

    // Check IntegerLiteral in Expression Statement is correct
    REQUIRE(test_literal_expression(expression_stmt->expression, 5));
}

TEST_CASE("Test Parsing Integer Prefix Expressions") {
    std::vector<std::tuple<std::string, std::string, int>> prefix_tests = {
            std::make_tuple("!5;", "!", 5),
            std::make_tuple("-15;", "-", 15),
    };

    for (const auto &tt: prefix_tests) {
        const auto [tt_input, tt_operator, tt_value] = tt;

        auto l = std::make_unique<Lexer>(Lexer(tt_input));
        auto p = Parser(std::move(l));

        auto program = p.parse_program();

        REQUIRE(test_parser_errors(p));

        if (program->statements.size() != 1) {
            std::cerr << "program.statements does not contain 1 statements. got=" << program->statements.size()
                      << std::endl;
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

        REQUIRE(test_literal_expression(expr->right, tt_value));
    }
}

TEST_CASE("Test Parsing Boolean Prefix Expressions") {
    std::vector<std::tuple<std::string, std::string, bool>> prefix_tests = {
            std::make_tuple("!true;", "!", true),
            std::make_tuple("!false;", "!", false),
    };

    for (const auto &tt: prefix_tests) {
        const auto [tt_input, tt_operator, tt_value] = tt;

        auto l = std::make_unique<Lexer>(Lexer(tt_input));
        auto p = Parser(std::move(l));

        auto program = p.parse_program();

        REQUIRE(test_parser_errors(p));

        if (program->statements.size() != 1) {
            std::cerr << "program.statements does not contain 1 statements. got=" << program->statements.size()
                      << std::endl;
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

        REQUIRE(test_literal_expression(expr->right, tt_value));
    }
}

TEST_CASE("Test Parsing Integer Infix Expressions") {
    std::vector<std::tuple<std::string, int, std::string, int>> infix_tests = {
            std::make_tuple("5 + 5;", 5, "+", 5),
            std::make_tuple("5 - 5;", 5, "-", 5),
            std::make_tuple("5 * 5;", 5, "*", 5),
            std::make_tuple("5 / 5;", 5, "/", 5),
            std::make_tuple("5 > 5;", 5, ">", 5),
            std::make_tuple("5 < 5;", 5, "<", 5),
            std::make_tuple("5 == 5;", 5, "==", 5),
            std::make_tuple("5 != 5;", 5, "!=", 5),
    };

    for (const auto &tt: infix_tests) {
        const auto [tt_input, tt_leftvalue, tt_operator, tt_rightvalue] = tt;

        auto l = std::make_unique<Lexer>(Lexer(tt_input));
        auto p = Parser(std::move(l));

        auto program = p.parse_program();

        REQUIRE(test_parser_errors(p));

        if (program->statements.size() != 1) {
            std::cerr << "program.statements does not contain 1 statements. got=" << program->statements.size()
                      << std::endl;
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

        // Check InfixExpression in Expression Statement is correct
        REQUIRE(test_infix_expression(expression_stmt->expression, tt_leftvalue, tt_operator, tt_rightvalue));
    }
}

TEST_CASE("Test Parsing Boolean Infix Expressions") {
    std::vector<std::tuple<std::string, bool, std::string, bool>> infix_tests = {
            std::make_tuple("true == true;", true, "==", true),
            std::make_tuple("true != false;", true, "!=", false),
            std::make_tuple("false == false;", false, "==", false),
    };

    for (const auto &tt: infix_tests) {
        const auto [tt_input, tt_leftvalue, tt_operator, tt_rightvalue] = tt;

        auto l = std::make_unique<Lexer>(Lexer(tt_input));
        auto p = Parser(std::move(l));

        auto program = p.parse_program();

        REQUIRE(test_parser_errors(p));

        if (program->statements.size() != 1) {
            std::cerr << "program.statements does not contain 1 statements. got=" << program->statements.size()
                      << std::endl;
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

        // Check InfixExpression in Expression Statement is correct
        REQUIRE(test_infix_expression(expression_stmt->expression, tt_leftvalue, tt_operator, tt_rightvalue));
    }
}

TEST_CASE("Test Operator Precedence Parsing") {
    std::vector<std::tuple<std::string, std::string>> tests = {
            std::make_tuple("-a * b", "((-a) * b)"),
            std::make_tuple("!-a", "(!(-a))"),
            std::make_tuple("a + b + c", "((a + b) + c)"),
            std::make_tuple("a + b - c", "((a + b) - c)"),
            std::make_tuple("a * b * c", "((a * b) * c)"),
            std::make_tuple("a * b / c", "((a * b) / c)"),
            std::make_tuple("a + b / c", "(a + (b / c))"),
            std::make_tuple("a + b * c + d / e - f", "(((a + (b * c)) + (d / e)) - f)"),
            std::make_tuple("3 + 4; -5 * 5", "(3 + 4)((-5) * 5)"),
            std::make_tuple("5 > 4 == 3 < 4", "((5 > 4) == (3 < 4))"),
            std::make_tuple("5 < 4 != 3 > 4", "((5 < 4) != (3 > 4))"),
            std::make_tuple("3 + 4 * 5 == 3 * 1 + 4 * 5", "((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))"),
            std::make_tuple("true", "true"),
            std::make_tuple("false", "false"),
            std::make_tuple("3 > 5 == false", "((3 > 5) == false)"),
            std::make_tuple("3 < 5 == true", "((3 < 5) == true)"),
            std::make_tuple("1 + (2 + 3) + 4", "((1 + (2 + 3)) + 4)"),
            std::make_tuple("(5 + 5) * 2", "((5 + 5) * 2)"),
            std::make_tuple("2 / (5 + 5)", "(2 / (5 + 5))"),
            std::make_tuple("-(5 + 5)", "(-(5 + 5))"),
            std::make_tuple("!(true == true)", "(!(true == true))"),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected] = tt;

        auto l = std::make_unique<Lexer>(Lexer(tt_input));
        auto p = Parser(std::move(l));

        auto program = p.parse_program();

        REQUIRE(test_parser_errors(p));

        auto actual = program->string();

        if (actual != tt_expected) {
            std::cerr << "expected=" << tt_expected << ", got=" << actual << std::endl;
        }
        REQUIRE(actual == tt_expected);
    }
}

TEST_CASE("Test Boolean Expression") {
    std::vector<std::tuple<std::string, bool>> tests = {
            std::make_tuple("true;", true),
            std::make_tuple("false;", false),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected_boolean] = tt;

        auto l = std::make_unique<Lexer>(Lexer(tt_input));
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

        // Check IntegerLiteral in Expression Statement is correct
        REQUIRE(test_literal_expression(expression_stmt->expression, tt_expected_boolean));
    }
}

TEST_CASE("Test If Expression") {
    std::string input = "if (x < y) { x }";

    auto l = std::make_unique<Lexer>(Lexer(input));
    auto p = Parser(std::move(l));

    auto program = p.parse_program();

    REQUIRE(test_parser_errors(p));

    if (program->statements.size() != 1) {
        std::cerr << "program->statements does not contain 1 statements. got=" << program->statements.size() << std::endl;
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

    // Cast Expression to an IfExpression, as this is what we are expecting
    auto exp = std::dynamic_pointer_cast<IfExpression>(expression_stmt->expression);

    // Check that we have an IfExpression by checking if the dynamic pointer cast fails (returns nullptr)
    if (!exp) {
        std::cerr << "exp is not an IfExpression." << std::endl;
    }
    REQUIRE(exp);

    REQUIRE(test_infix_expression(exp->condition, "x", "<", "y"));

    // Check that consequence is a BlockStatement by checking for nullptr
    if (!exp->consequence) {
        std::cerr << "consequence is not a BlockStatement." << std::endl;
    }
    REQUIRE(exp->consequence);

    if (exp->consequence->statements.size() != 1) {
        std::cerr << "consequence is not 1 statements. got=" << exp->consequence->statements.size() << std::endl;
    }
    REQUIRE(exp->consequence->statements.size() == 1);

    stmt = exp->consequence->statements.at(0);

    // Cast Node to an ExpressionStatement, as this is what we are expecting
    auto consequence = std::dynamic_pointer_cast<ExpressionStatement>(stmt);

    // Check that we have an Expression Statement by checking if the dynamic pointer cast fails (returns nullptr)
    if (!consequence) {
        std::cerr << "exp->consequence->statements.at(0) is not an ExpressionStatement." << std::endl;
    }
    REQUIRE(consequence);

    REQUIRE(test_identifier(consequence->expression, "x"));

    if (exp->alternative) {
        std::cerr << "exp->alternative->statements was not nullptr." << std::endl;
    }
    REQUIRE(!(exp->alternative));
}

TEST_CASE("Test If Else Expression") {
    std::string input = "if (x < y) { x } else { y }";

    auto l = std::make_unique<Lexer>(Lexer(input));
    auto p = Parser(std::move(l));

    auto program = p.parse_program();

    REQUIRE(test_parser_errors(p));

    if (program->statements.size() != 1) {
        std::cerr << "program->statements does not contain 1 statements. got=" << program->statements.size() << std::endl;
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

    // Cast Expression to an IfExpression, as this is what we are expecting
    auto exp = std::dynamic_pointer_cast<IfExpression>(expression_stmt->expression);

    // Check that we have an IfExpression by checking if the dynamic pointer cast fails (returns nullptr)
    if (!exp) {
        std::cerr << "exp is not an IfExpression." << std::endl;
    }
    REQUIRE(exp);

    REQUIRE(test_infix_expression(exp->condition, "x", "<", "y"));

    // Check that consequence is a BlockStatement by checking for nullptr
    if (!exp->consequence) {
        std::cerr << "consequence is not a BlockStatement." << std::endl;
    }
    REQUIRE(exp->consequence);

    if (exp->consequence->statements.size() != 1) {
        std::cerr << "consequence is not 1 statements. got=" << exp->consequence->statements.size() << std::endl;
    }
    REQUIRE(exp->consequence->statements.size() == 1);

    stmt = exp->consequence->statements.at(0);

    // Cast Node to an ExpressionStatement, as this is what we are expecting
    auto consequence = std::dynamic_pointer_cast<ExpressionStatement>(stmt);

    // Check that we have an Expression Statement by checking if the dynamic pointer cast fails (returns nullptr)
    if (!consequence) {
        std::cerr << "exp->consequence->statements.at(0) is not an ExpressionStatement." << std::endl;
    }
    REQUIRE(consequence);

    REQUIRE(test_identifier(consequence->expression, "x"));

    // Check that alternative is a BlockStatement by checking for nullptr
    if (!exp->alternative) {
        std::cerr << "alternative is not a BlockStatement." << std::endl;
    }
    REQUIRE(exp->alternative);

    if (exp->alternative->statements.size() != 1) {
        std::cerr << "alternative is not 1 statements. got=" << exp->alternative->statements.size() << std::endl;
    }
    REQUIRE(exp->alternative->statements.size() == 1);

    stmt = exp->alternative->statements.at(0);

    // Cast Node to an ExpressionStatement, as this is what we are expecting
    auto alternative = std::dynamic_pointer_cast<ExpressionStatement>(stmt);

    // Check that we have an Expression Statement by checking if the dynamic pointer cast fails (returns nullptr)
    if (!alternative) {
        std::cerr << "exp->alternative->statements.at(0) is not an ExpressionStatement." << std::endl;
    }
    REQUIRE(alternative);

    REQUIRE(test_identifier(alternative->expression, "y"));
}

TEST_CASE("Test Function Literal Parsing") {
    std::string input = "fn(x, y) { x + y; }";

    auto l = std::make_unique<Lexer>(Lexer(input));
    auto p = Parser(std::move(l));

    auto program = p.parse_program();

    REQUIRE(test_parser_errors(p));

    if (program->statements.size() != 1) {
        std::cerr << "program->statements does not contain 1 statements. got=" << program->statements.size() << std::endl;
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

    // Cast Expression to a FunctionLiteral, as this is what we are expecting
    auto function = std::dynamic_pointer_cast<FunctionLiteral>(expression_stmt->expression);

    // Check that we have a FunctionLiteral by checking if the dynamic pointer cast fails (returns nullptr)
    if (!function) {
        std::cerr << "function is not a FunctionLiteral." << std::endl;
    }
    REQUIRE(function);

    if (function->parameters.size() != 2) {
        std::cerr << "function literal parameters wrong. want 2, got=" << function->parameters.size() << std::endl;
    }
    REQUIRE(function->parameters.size() == 2);

    REQUIRE(test_literal_expression(function->parameters.at(0), "x"));
    REQUIRE(test_literal_expression(function->parameters.at(1), "y"));

    if (function->body->statements.size() != 1) {
        std::cerr << "function->body->statements has not 1 statements. got=" << function->body->statements.size() << std::endl;
    }
    REQUIRE(function->body->statements.size() == 1);

    stmt = function->body->statements.at(0);

    // Cast Node to an ExpressionStatement, as this is what we are expecting
    auto body_stmt = std::dynamic_pointer_cast<ExpressionStatement>(stmt);

    // Check that we have an Expression Statement by checking if the dynamic pointer cast fails (returns nullptr)
    if (!body_stmt) {
        std::cerr << "function body stmt is not an ExpressionStatement." << std::endl;
    }
    REQUIRE(body_stmt);

    REQUIRE(test_infix_expression(body_stmt->expression, "x", "+", "y"));
}

TEST_CASE("Test Function Parameter Parsing") {
    std::vector<std::tuple<std::string, std::vector<std::string>>> tests = {
            std::make_tuple("fn() {};", std::vector<std::string>{}),
            std::make_tuple("fn(x) {};", std::vector<std::string>{"x"}),
            std::make_tuple("fn(x, y, z) {};", std::vector<std::string>{"x", "y", "z"}),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected] = tt;

        auto l = std::make_unique<Lexer>(Lexer(tt_input));
        auto p = Parser(std::move(l));

        auto program = p.parse_program();

        REQUIRE(test_parser_errors(p));

        auto stmt = program->statements.at(0);

        // Can now cast Node to a derived ExpressionStatement, as we are confident that it is one
        auto expression_stmt = std::dynamic_pointer_cast<ExpressionStatement>(stmt);

        // Check that we have an Expression Statement by checking if the dynamic pointer cast fails (returns nullptr)
        if (!expression_stmt) {
            std::cerr << "program.statements.at(0) is not an ExpressionStatement." << std::endl;
        }
        REQUIRE(expression_stmt);

        // Cast Expression to a FunctionLiteral, as this is what we are expecting
        auto function = std::dynamic_pointer_cast<FunctionLiteral>(expression_stmt->expression);

        // Check that we have a FunctionLiteral by checking if the dynamic pointer cast fails (returns nullptr)
        if (!function) {
            std::cerr << "function is not a FunctionLiteral." << std::endl;
        }
        REQUIRE(function);

        if (function->parameters.size() != tt_expected.size()) {
            std::cerr << "length parameters wrong. want " << tt_expected.size() << ", got=" << function->parameters.size() << std::endl;
        }
        REQUIRE(function->parameters.size() == tt_expected.size());

        for (int i = 0; i < tt_expected.size(); i++) {
            REQUIRE(test_literal_expression(function->parameters.at(i), tt_expected.at(i)));
        }
    }
}
