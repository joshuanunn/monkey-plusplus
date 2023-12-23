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

bool test_null_object(std::shared_ptr<Object> actual) {
    auto result = std::dynamic_pointer_cast<Null>(actual);

    if (!result) {
        std::cerr << "obj is not Null." << std::endl;
        return false;
    }

    if (result != get_null_ref()) {
        std::cerr << "obj is not Null." << std::endl;
        return false;
    }

    return true;
}

bool test_string_object(std::string expected, std::shared_ptr<Object> actual) {
    auto string_obj = std::dynamic_pointer_cast<String>(actual);
    if (!string_obj) {
        std::cerr << "object is not String." << std::endl;
        return false;
    }

    if (string_obj->value != expected) {
        std::cerr << "object has wrong value. got=" << string_obj->value << ", want=" << expected << std::endl;
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
            std::make_tuple("-5", -5),
            std::make_tuple("-10", -10),
            std::make_tuple("-50 + 100 + -50", 0),
            std::make_tuple("(5 + 10 * 2 + 15 / 3) * 2 + -10", 50),
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
            std::make_tuple("!true", false),
            std::make_tuple("!false", true),
            std::make_tuple("!5", false),
            std::make_tuple("!!true", true),
            std::make_tuple("!!false", false),
            std::make_tuple("!!5", true),
            std::make_tuple("!(if (false) { 5; })", true),
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

TEST_CASE("Test Conditionals") {
    std::vector<std::tuple<std::string, int>> tests = {
            std::make_tuple("if (true) { 10 }", 10),
            std::make_tuple("if (true) { 10 } else { 20 }", 10),
            std::make_tuple("if (false) { 10 } else { 20 }", 20),
            std::make_tuple("if (1) { 10 }", 10),
            std::make_tuple("if (1 < 2) { 10 }", 10),
            std::make_tuple("if (1 < 2) { 10 } else { 20 }", 10),
            std::make_tuple("if (1 > 2) { 10 } else { 20 }", 20),
            std::make_tuple("if ((if (false) { 10 })) { 10 } else { 20 }", 20),
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

TEST_CASE("Test Conditionals Returning Null") {
    std::vector<std::tuple<std::string, std::shared_ptr<Object>>> tests = {
            std::make_tuple("if (1 > 2) { 10 }", get_null_ref()),
            std::make_tuple("if (false) { 10 }", get_null_ref()),
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

        REQUIRE(test_null_object(stack_elem));
    }
}

TEST_CASE("Test Global Let Statements") {
    std::vector<std::tuple<std::string, int>> tests = {
            std::make_tuple("let one = 1; one", 1),
            std::make_tuple("let one = 1; let two = 2; one + two", 3),
            std::make_tuple("let one = 1; let two = one + one; one + two", 3),
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

TEST_CASE("Test String Expressions") {
    std::vector<std::tuple<std::string, std::string>> tests = {
            std::make_tuple(R"("monkey")", "monkey"),
            std::make_tuple(R"("mon" + "key")", "monkey"),
            std::make_tuple(R"("mon" + "key" + "banana")", "monkeybanana"),
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

        REQUIRE(test_string_object(tt_expected, stack_elem));
    }
}
