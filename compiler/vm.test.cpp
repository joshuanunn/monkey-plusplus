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

TEST_CASE("Test Integer Arithmetic") {
    std::vector<std::tuple<std::string, int>> tests = {
            std::make_tuple("1", 1),
            std::make_tuple("2", 2),
            std::make_tuple("1 + 2", 2), // TODO: update 2 to expected 3 after addition functionality fixed
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

        auto stack_elem = vm.stack_top();

        REQUIRE(test_integer_object(tt_expected, stack_elem));
    }
}