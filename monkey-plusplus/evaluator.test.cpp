// Using Catch2 unit testing library

// Let Catch2 define a main function
#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"

#include "evaluator.hpp"
#include "lexer.hpp"
#include "object.hpp"
#include "parser.hpp"

std::shared_ptr<Object> test_eval (const std::string &input) {
    auto l = std::make_unique<Lexer>(Lexer(input));
    auto p = Parser(std::move(l));
    auto program = p.parse_program();

    return eval(std::move(program));
}

bool test_integer_object(std::shared_ptr<Object> obj, int expected) {
    // Cast Object to an Integer, as this is what we are expecting
    auto result = std::dynamic_pointer_cast<Integer>(obj);

    // Check that we have an Integer Object by checking if the dynamic pointer cast fails (returns nullptr)
    if (!result) {
        std::cerr << "obj is not Integer." << std::endl;
        return false;
    }

    if (result->value != expected) {
        std::cerr << "object has wrong value. got=" << result->value << ", want=" << expected << std::endl;
        return false;
    }

    return true;
}

bool test_boolean_object(std::shared_ptr<Object> obj, bool expected) {
    // Cast Object to an Boolean, as this is what we are expecting
    auto result = std::dynamic_pointer_cast<Boolean>(obj);

    // Check that we have a Boolean Object by checking if the dynamic pointer cast fails (returns nullptr)
    if (!result) {
        std::cerr << "obj is not Boolean." << std::endl;
        return false;
    }

    if (result->value != expected) {
        std::cerr << "object has wrong value. got=" << result->value << ", want=" << expected << std::endl;
        return false;
    }

    return true;
}

TEST_CASE("Test Eval Integer Expression") {
    std::vector<std::tuple<std::string, int>> tests = {
            std::make_tuple("5", 5),
            std::make_tuple("10", 10),
            std::make_tuple("-5", -5),
            std::make_tuple("-10", -10),
            std::make_tuple("5 + 5 + 5 + 5 - 10", 10),
            std::make_tuple("2 * 2 * 2 * 2 * 2", 32),
            std::make_tuple("-50 + 100 + -50", 0),
            std::make_tuple("5 * 2 + 10", 20),
            std::make_tuple("5 + 2 * 10", 25),
            std::make_tuple("20 + 2 * -10", 0),
            std::make_tuple("50 / 2 * 2 + 10", 60),
            std::make_tuple("2 * (5 + 10)", 30),
            std::make_tuple("3 * 3 * 3 + 10", 37),
            std::make_tuple("3 * (3 * 3) + 10", 37),
            std::make_tuple("(5 + 10 * 2 + 15 / 3) * 2 + -10", 50),
            std::make_tuple("40 + 2", 42),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected] = tt;

        auto evaluated = test_eval(tt_input);
        REQUIRE(test_integer_object(evaluated, tt_expected));
    }
}

TEST_CASE("Test Eval Boolean Expression") {
    std::vector<std::tuple<std::string, bool>> tests = {
            std::make_tuple("true", true),
            std::make_tuple("false", false),
            std::make_tuple("1 < 2", true),
            std::make_tuple("1 > 2", false),
            std::make_tuple("1 > 1", false),
            std::make_tuple("1 < 1", false),
            std::make_tuple("1 == 1", true),
            std::make_tuple("1 != 1", false),
            std::make_tuple("1 == 2", false),
            std::make_tuple("1 != 2", true),
            std::make_tuple("true == true", true),
            std::make_tuple("false == false", true),
            std::make_tuple("true == false", false),
            std::make_tuple("true != false", true),
            std::make_tuple("false != true", true),
            std::make_tuple("(1 < 2) == true", true),
            std::make_tuple("(1 < 2) == false", false),
            std::make_tuple("(1 > 2) == true", false),
            std::make_tuple("(1 > 2) == false", true),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected] = tt;

        auto evaluated = test_eval(tt_input);
        REQUIRE(test_boolean_object(evaluated, tt_expected));
    }
}

TEST_CASE("Test Bang Operator") {
    std::vector<std::tuple<std::string, bool>> tests = {
            std::make_tuple("!true", false),
            std::make_tuple("!false", true),
            std::make_tuple("!5", false),
            std::make_tuple("!!true", true),
            std::make_tuple("!!false", false),
            std::make_tuple("!!5", true),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected] = tt;

        auto evaluated = test_eval(tt_input);
        REQUIRE(test_boolean_object(evaluated, tt_expected));
    }
}
