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

TEST_CASE("Test Eval Integer Expression") {
    std::vector<std::tuple<std::string, int>> tests = {
            std::make_tuple("5;", 5),
            std::make_tuple("10;", 10),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected] = tt;

        auto evaluated = test_eval(tt_input);
        REQUIRE(test_integer_object(evaluated, tt_expected));
    }
}
