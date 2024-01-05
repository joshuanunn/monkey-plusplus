// Using Catch2 unit testing library

// Let Catch2 define a main function
#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"

#include "parser.hpp"

// Forward declaration of test helpers

bool test_parser_errors(const Parser &p);

bool test_integer_literal(std::shared_ptr<Expression> il, int value);

bool test_identifier(std::shared_ptr<Expression> exp, std::string value);

bool test_boolean_expression(std::shared_ptr<Expression> exp, bool value);

bool test_literal_expression(std::shared_ptr<Expression> exp, int value);

bool test_literal_expression(std::shared_ptr<Expression> exp, const char *value);

bool test_literal_expression(std::shared_ptr<Expression> exp, std::string value);

bool test_literal_expression(std::shared_ptr<Expression> exp, bool value);


template <class T>
bool test_let_statement(const Node &s, const std::string &name, T value) {
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

    if (!test_literal_expression(let_stmt->value, value)) {
        return false;
    }

    return true;
}

template <class T>
bool test_return_statement(const Node &s, T value) {
    if (s.token_literal() != "return") {
        std::cerr << "s->token_literal not 'return'. got=" << s.token_literal() << std::endl;
        return false;
    }

    // Can now cast Node to a derived ReturnStatement, as we are confident that it is one
    auto return_stmt = dynamic_cast<const ReturnStatement *>(&s);

    if (!test_literal_expression(return_stmt->return_value, value)) {
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
    auto bo = std::dynamic_pointer_cast<BooleanLiteral>(exp);

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
let y = true;
let foobar = y;
)";
    auto l = Lexer(input);
    auto p = Parser(std::move(l));
    auto program = p.parse_program();
    REQUIRE(test_parser_errors(p));

    if (program->statements.size() != 3) {
        std::cerr << "program->statements does not contain 3 statements. got=" << program->statements.size()
                  << std::endl;
    }
    REQUIRE(program->statements.size() == 3);

    REQUIRE(test_let_statement(*program->statements.at(0), "x", 5));
    REQUIRE(test_let_statement(*program->statements.at(1), "y", true));
    REQUIRE(test_let_statement(*program->statements.at(2), "foobar", "y"));
}

TEST_CASE("Test Return Statements") {
    std::string input = R"(
return 5;
return true;
return foobar;
)";
    auto l = Lexer(input);
    auto p = Parser(std::move(l));
    auto program = p.parse_program();
    REQUIRE(test_parser_errors(p));

    if (program->statements.size() != 3) {
        std::cerr << "program->statements does not contain 3 statements. got=" << program->statements.size()
                  << std::endl;
    }
    REQUIRE(program->statements.size() == 3);

    REQUIRE(test_return_statement(*program->statements.at(0), 5));
    REQUIRE(test_return_statement(*program->statements.at(1), true));
    REQUIRE(test_return_statement(*program->statements.at(2), "foobar"));
}

TEST_CASE("Test Identifier Expression") {
    std::string input = R"(foobar;)";

    auto l = Lexer(input);
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

    auto l = Lexer(input);
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

        auto l = Lexer(tt_input);
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

        auto l = Lexer(tt_input);
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

        auto l = Lexer(tt_input);
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

        auto l = Lexer(tt_input);
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
            std::make_tuple("a + add(b * c) + d", "((a + add((b * c))) + d)"),
            std::make_tuple("add(a, b, 1, 2 * 3, 4 + 5, add(6, 7 * 8))", "add(a, b, 1, (2 * 3), (4 + 5), add(6, (7 * 8)))"),
            std::make_tuple("add(a + b + c * d / f + g)", "add((((a + b) + ((c * d) / f)) + g))"),
            std::make_tuple("a * [1, 2, 3, 4][b * c] * d", "((a * ([1, 2, 3, 4][(b * c)])) * d)"),
            std::make_tuple("add(a * b[2], b[1], 2 * [1, 2][1])", "add((a * (b[2])), (b[1]), (2 * ([1, 2][1])))"),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected] = tt;

        auto l = Lexer(tt_input);
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

        auto l = Lexer(tt_input);
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

    auto l = Lexer(input);
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

    auto l = Lexer(input);
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

    auto l = Lexer(input);
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

        auto l = Lexer(tt_input);
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

        for (int i = 0; i < static_cast<int>(tt_expected.size()); i++) {
            REQUIRE(test_literal_expression(function->parameters.at(i), tt_expected.at(i)));
        }
    }
}

TEST_CASE("Test Call Expression Parsing") {
    std::string input = "add(1, 2 * 3, 4 + 5);";

    auto l = Lexer(input);
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

    // Cast Expression to a CallExpression, as this is what we are expecting
    auto exp = std::dynamic_pointer_cast<CallExpression>(expression_stmt->expression);

    // Check that we have a FunctionLiteral by checking if the dynamic pointer cast fails (returns nullptr)
    if (!exp) {
        std::cerr << "stmt->expression is not a CallExpression." << std::endl;
    }
    REQUIRE(exp);

    REQUIRE(test_identifier(exp->function, "add"));

    if (exp->arguments.size() != 3) {
        std::cerr << "wrong length of arguments. want 2, got=" << exp->arguments.size() << std::endl;
    }
    REQUIRE(exp->arguments.size() == 3);

    REQUIRE(test_literal_expression(exp->arguments.at(0), 1));
    REQUIRE(test_infix_expression(exp->arguments.at(1), 2, "*", 3));
    REQUIRE(test_infix_expression(exp->arguments.at(2), 4, "+", 5));
}

TEST_CASE("Test Call Expression Parameter Parsing") {
    std::vector<std::tuple<std::string, std::string, std::vector<std::string>>> tests = {
            std::make_tuple("add();", "add", std::vector<std::string>{}),
            std::make_tuple("add(1);", "add", std::vector<std::string>{"1"}),
            std::make_tuple("add(1, 2 * 3, 4 + 5);", "add", std::vector<std::string>{"1", "(2 * 3)", "(4 + 5)"}),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected_ident, tt_expected_args] = tt;

        auto l = Lexer(tt_input);
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

        // Cast Expression to a CallExpression, as this is what we are expecting
        auto exp = std::dynamic_pointer_cast<CallExpression>(expression_stmt->expression);

        // Check that we have a CallExpression by checking if the dynamic pointer cast fails (returns nullptr)
        if (!exp) {
            std::cerr << "stmt->expression is not a CallExpression." << std::endl;
        }
        REQUIRE(exp);

        REQUIRE(test_identifier(exp->function, tt_expected_ident));

        if (exp->arguments.size() != tt_expected_args.size()) {
            std::cerr << "wrong length of arguments. want " << tt_expected_args.size() << ", got=" << exp->arguments.size() << std::endl;
        }
        REQUIRE(exp->arguments.size() == tt_expected_args.size());

        for (int i = 0; i < static_cast<int>(tt_expected_args.size()); i++) {
            if (exp->arguments.at(i)->string() != tt_expected_args.at(i)) {
                std::cerr << "argument " << i << " wrong. want =" << tt_expected_args.at(i) << ", got=" << exp->arguments.at(i)->string() << std::endl;
            }
            REQUIRE(exp->arguments.at(i)->string() == tt_expected_args.at(i));
        }
    }
}

TEST_CASE("Test String Literal Expression") {
    std::string input = R"("hello world";)";
    std::string value = "hello world";

    auto l = Lexer(input);
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

    // Can now cast ExpressionStatement to a StringLiteral
    auto literal = std::dynamic_pointer_cast<StringLiteral>(expression_stmt->expression);

    // Check that we have a StringLiteral by checking if the dynamic pointer cast fails (returns nullptr)
    if (!literal) {
        std::cerr << "exp is not a StringLiteral." << std::endl;
    }
    REQUIRE(literal);

    // Check StringLiteral in Expression Statement is correct
    if (literal->value != value) {
        std::cerr << "literal->value not " << value << ". got=" << literal->value << std::endl;
    }
    REQUIRE(literal->value == value);
}

TEST_CASE("Test Parsing Array Literals") {
    std::string input = "[1, 2 * 2, 3 + 3]";

    auto l = Lexer(input);
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

    // Cast Expression to an ArrayLiteral, as this is what we are expecting
    auto array = std::dynamic_pointer_cast<ArrayLiteral>(expression_stmt->expression);

    // Check that we have an ArrayLiteral by checking if the dynamic pointer cast fails (returns nullptr)
    if (!array) {
        std::cerr << "exp is not an ArrayLiteral." << std::endl;
    }
    REQUIRE(array);

    if (array->elements.size() != 3) {
        std::cerr << "len(array->elements) not 3. got=" << array->elements.size() << std::endl;
    }
    REQUIRE(array->elements.size() == 3);

    REQUIRE(test_literal_expression(array->elements.at(0), 1));
    REQUIRE(test_infix_expression(array->elements.at(1), 2, "*", 2));
    REQUIRE(test_infix_expression(array->elements.at(2), 3, "+", 3));
}

TEST_CASE("Test Parsing Index Expressions") {
    std::string input = "myArray[1 + 1]";

    auto l = Lexer(input);
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

    // Cast Expression to an IndexExpression, as this is what we are expecting
    auto index_exp = std::dynamic_pointer_cast<IndexExpression>(expression_stmt->expression);

    // Check that we have an IndexExpression by checking if the dynamic pointer cast fails (returns nullptr)
    if (!index_exp) {
        std::cerr << "exp is not an IndexExpression." << std::endl;
    }
    REQUIRE(index_exp);

    REQUIRE(test_identifier(index_exp->left, "myArray"));
    REQUIRE(test_infix_expression(index_exp->index, 1, "+", 1));
}

TEST_CASE("Test Parsing Empty Hash Literal") {
    std::string input = "{}";

    auto l = Lexer(input);
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

    // Cast Expression to a HashLiteral, as this is what we are expecting
    auto hash = std::dynamic_pointer_cast<HashLiteral>(expression_stmt->expression);

    // Check that we have a HashLiteral by checking if the dynamic pointer cast fails (returns nullptr)
    if (!hash) {
        std::cerr << "stmt->expression is not a HashLiteral." << std::endl;
    }
    REQUIRE(hash);

    if (!hash->pairs.empty()) {
        std::cerr << "hash->pairs has wrong length. got=" << hash->pairs.size() << std::endl;
    }
    REQUIRE(hash->pairs.empty());
}

TEST_CASE("Test Parsing Hash Literal String Keys") {
    std::string input = R"({"one": 1, "two": 2, "three": 3})";

    std::map<std::string, int> expected = {
            {"one", 1},
            {"two", 2},
            {"three", 3},
    };

    auto l = Lexer(input);
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

    // Cast Expression to a HashLiteral, as this is what we are expecting
    auto hash = std::dynamic_pointer_cast<HashLiteral>(expression_stmt->expression);

    // Check that we have a HashLiteral by checking if the dynamic pointer cast fails (returns nullptr)
    if (!hash) {
        std::cerr << "stmt->expression is not a HashLiteral." << std::endl;
    }
    REQUIRE(hash);

    if (hash->pairs.size() != expected.size()) {
        std::cerr << "hash->pairs has wrong length. got=" << hash->pairs.size() << std::endl;
    }
    REQUIRE(hash->pairs.size() == expected.size());

    for (const auto &kv: hash->pairs) {
        const auto[key, value] = kv;

        auto literal = std::dynamic_pointer_cast<StringLiteral>(key);
        if (!literal) {
            std::cerr << "hash->pair->key is not a StringLiteral." << std::endl;
        }
        REQUIRE(literal);

        auto expected_value = expected[literal->string()];

        REQUIRE(test_integer_literal(value, expected_value));
    }
}

TEST_CASE("Test Parsing Hash Literal Boolean Keys") {
    std::string input = R"({true: 1, false: 2})";

    std::map<std::string, int> expected = {
            {"true", 1},
            {"false", 2},
    };

    auto l = Lexer(input);
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

    // Cast Expression to a HashLiteral, as this is what we are expecting
    auto hash = std::dynamic_pointer_cast<HashLiteral>(expression_stmt->expression);

    // Check that we have a HashLiteral by checking if the dynamic pointer cast fails (returns nullptr)
    if (!hash) {
        std::cerr << "stmt->expression is not a HashLiteral." << std::endl;
    }
    REQUIRE(hash);

    if (hash->pairs.size() != expected.size()) {
        std::cerr << "hash->pairs has wrong length. got=" << hash->pairs.size() << std::endl;
    }
    REQUIRE(hash->pairs.size() == expected.size());

    for (const auto &kv: hash->pairs) {
        const auto[key, value] = kv;

        auto boolean = std::dynamic_pointer_cast<BooleanLiteral>(key);
        if (!boolean) {
            std::cerr << "hash->pair->key is not a BooleanLiteral." << std::endl;
        }
        REQUIRE(boolean);

        auto expected_value = expected[boolean->string()];

        REQUIRE(test_integer_literal(value, expected_value));
    }
}

TEST_CASE("Test Parsing Hash Literal Integer Keys") {
    std::string input = R"({1: 1, 2: 2, 3: 3})";

    std::map<std::string, int> expected = {
            {"1", 1},
            {"2", 2},
            {"3", 3},
    };

    auto l = Lexer(input);
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

    // Cast Expression to a HashLiteral, as this is what we are expecting
    auto hash = std::dynamic_pointer_cast<HashLiteral>(expression_stmt->expression);

    // Check that we have a HashLiteral by checking if the dynamic pointer cast fails (returns nullptr)
    if (!hash) {
        std::cerr << "stmt->expression is not a HashLiteral." << std::endl;
    }
    REQUIRE(hash);

    if (hash->pairs.size() != expected.size()) {
        std::cerr << "hash->pairs has wrong length. got=" << hash->pairs.size() << std::endl;
    }
    REQUIRE(hash->pairs.size() == expected.size());

    for (const auto &kv: hash->pairs) {
        const auto[key, value] = kv;

        auto integer = std::dynamic_pointer_cast<IntegerLiteral>(key);
        if (!integer) {
            std::cerr << "hash->pair->key is not an IntegerLiteral." << std::endl;
        }
        REQUIRE(integer);

        auto expected_value = expected[integer->string()];

        REQUIRE(test_integer_literal(value, expected_value));
    }
}

TEST_CASE("Test Parsing Hash Literals With Expressions") {
    std::string input = R"({"one": 0 + 1, "two": 10 - 8, "three": 15 / 5})";

    std::map<std::string, std::tuple<int, std::string, int>> expected = {
            {"one", std::make_tuple(0, "+", 1)},
            {"two", std::make_tuple(10, "-", 8)},
            {"three", std::make_tuple(15, "/", 5)},
    };

    auto l = Lexer(input);
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

    // Cast Expression to a HashLiteral, as this is what we are expecting
    auto hash = std::dynamic_pointer_cast<HashLiteral>(expression_stmt->expression);

    // Check that we have a HashLiteral by checking if the dynamic pointer cast fails (returns nullptr)
    if (!hash) {
        std::cerr << "stmt->expression is not a HashLiteral." << std::endl;
    }
    REQUIRE(hash);

    if (hash->pairs.size() != expected.size()) {
        std::cerr << "hash->pairs has wrong length. got=" << hash->pairs.size() << std::endl;
    }
    REQUIRE(hash->pairs.size() == expected.size());

    for (const auto &kv: hash->pairs) {
        const auto[key, value] = kv;

        auto literal = std::dynamic_pointer_cast<StringLiteral>(key);
        if (!literal) {
            std::cerr << "hash->pair->key is not a StringLiteral." << std::endl;
        }
        REQUIRE(literal);

        auto[left_int, op, right_int] = expected[literal->string()];

        REQUIRE(test_infix_expression(value, left_int, op, right_int));
    }
}

TEST_CASE("Test Function Literal With Name") {
    std::string input = R"(let myFunction = fn() { };)";

    auto l = Lexer(input);
    auto p = Parser(std::move(l));
    auto program = p.parse_program();
    REQUIRE(test_parser_errors(p));

    if (program->statements.size() != 1) {
        std::cerr << "program->statements does not contain 1 statements. got=" << program->statements.size() << std::endl;
    }
    REQUIRE(program->statements.size() == 1);

    auto stmt = program->statements.at(0);

    // Can now cast Node to a derived LetStatement, as we are confident that it is one
    auto let_stmt = std::dynamic_pointer_cast<LetStatement>(stmt);

    // Check that we have a Let Statement by checking if the dynamic pointer cast fails (returns nullptr)
    if (!let_stmt) {
        std::cerr << "program.statements.at(0) is not a LetStatement." << std::endl;
    }
    REQUIRE(let_stmt);

    // Cast statement value to a FunctionLiteral, as this is what we are expecting
    auto function = std::dynamic_pointer_cast<FunctionLiteral>(let_stmt->value);

    // Check that we have a HashLiteral by checking if the dynamic pointer cast fails (returns nullptr)
    if (!function) {
        std::cerr << "stmt->value is not a FunctionLiteral." << std::endl;
    }
    REQUIRE(function);

    if (function->name != "myFunction") {
        std::cerr << "function literal name wrong. want 'myFunction', got='" << function->name << "'" << std::endl;
    }
    REQUIRE(function->name == "myFunction");
}
