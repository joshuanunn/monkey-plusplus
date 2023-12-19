// Using Catch2 unit testing library

// Let Catch2 define a main function
#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"

#include "compiler.hpp"
#include "parser.hpp"
#include "vm.hpp"

std::shared_ptr<Program> parse(const std::string& input) {
    auto l = Lexer(input);
    auto p = Parser(std::move(l));
    auto program = p.parse_program();

    return program;
}

bool test_integer_object(int expected, std::shared_ptr<Object> actual) {
    auto integer_obj = std::dynamic_pointer_cast<Integer>(actual);
    if (!integer_obj) {
        std::cerr << "object is not Integer." << std::endl;
        return false;
    }

    if (integer_obj->value != expected) {
        std::cerr << "object has wrong value. got=" << integer_obj->value << ", want=" << expected << std::endl;
        return false;
    }

    return true;
}

bool test_boolean_object(bool expected, std::shared_ptr<Object> actual) {
    auto boolean_obj = std::dynamic_pointer_cast<Boolean>(actual);
    if (!boolean_obj) {
        std::cerr << "object is not Boolean." << std::endl;
        return false;
    }

    if (boolean_obj->value != expected) {
        std::cerr << "object has wrong value. got=" << boolean_obj->value << ", want=" << expected << std::endl;
        return false;
    }

    return true;
}

TEST_CASE("Test Integer Arithmetic") {
    std::vector<std::tuple<std::string, int>> tests = {
            std::make_tuple("1", 1),
            std::make_tuple("2", 2),
            std::make_tuple("1 + 2", 3),
            std::make_tuple("1 - 2", -1),
            std::make_tuple("1 * 2", 2),
            std::make_tuple("4 / 2", 2),
            std::make_tuple("50 / 2 * 2 + 10 - 5", 55),
            std::make_tuple("5 + 5 + 5 + 5 - 10", 10),
            std::make_tuple("2 * 2 * 2 * 2 * 2", 32),
            std::make_tuple("5 * 2 + 10", 20),
            std::make_tuple("5 + 2 * 10", 25),
            std::make_tuple("5 * (2 + 10)", 60),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected] = tt;

        auto program = parse(tt_input);

        auto compiler = new_compiler();

        auto err = compiler->compile(program);
        if (err) {
            std::cerr << "compiler error: " << err->message << std::endl;
        }
        REQUIRE(!err);

        auto vm = VM(compiler->bytecode());

        err = vm.run();
        if (err) {
            std::cerr << "vm error: " << err->message << std::endl;
        }
        REQUIRE(!err);

        auto stack_elem = vm.last_popped_stack_elem();

        REQUIRE(test_integer_object(tt_expected, stack_elem));
    }
}

TEST_CASE("Test Boolean Expressions") {
    std::vector<std::tuple<std::string, bool>> tests = {
            std::make_tuple("true", true),
            std::make_tuple("false", false),
            std::make_tuple("1 < 2", true),
            std::make_tuple("1 > 2", false),
            std::make_tuple("1 < 1", false),
            std::make_tuple("1 > 1", false),
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

        auto program = parse(tt_input);

        auto compiler = new_compiler();

        auto err = compiler->compile(program);
        if (err) {
            std::cerr << "compiler error: " << err->message << std::endl;
        }
        REQUIRE(!err);

        auto vm = VM(compiler->bytecode());

        err = vm.run();
        if (err) {
            std::cerr << "vm error: " << err->message << std::endl;
        }
        REQUIRE(!err);

        auto stack_elem = vm.last_popped_stack_elem();

        REQUIRE(test_boolean_object(tt_expected, stack_elem));
    }
}
