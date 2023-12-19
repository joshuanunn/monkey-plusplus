// Using Catch2 unit testing library

// Let Catch2 define a main function
#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"

#include "compiler.hpp"
#include "parser.hpp"

std::shared_ptr<Program> parse(const std::string& input) {
    auto l = Lexer(input);
    auto p = Parser(std::move(l));
    auto program = p.parse_program();

    return program;
}

Instructions concat_instructions(const std::vector<Instructions>& s) {
    auto out = Instructions{};

    for (const auto& ins: s) {
        out.insert(out.end(), ins.begin(), ins.end());
    }

    return out;
}

bool test_instructions(const std::vector<Instructions>& expected, const Instructions& actual) {
    auto concatted = concat_instructions(expected);

    if (actual.size() != concatted.size()) {
        std::cerr << "wrong instructions length.\nwant:\n" << concatted << "\ngot:\n" << actual << std::endl;
        return false;
    }

    int i = 0;
    for (const auto& ins: concatted) {
        if (actual.at(i++) != ins) {
            std::cerr << "wrong instruction at " << i << ".\nwant=" << concatted << "\ngot =" << actual << std::endl;
            return false;
        }
    }

    return true;
}

bool test_integer_constants(const std::vector<int>& expected, const std::vector<std::shared_ptr<Object>>& actual) {
    if (expected.size() != actual.size()) {
        std::cerr << "wrong number of constants. got=" << actual.size() << ", want=" << expected.size() << std::endl;
        return false;
    }

    for (int i = 0; i < expected.size(); i++) {
        auto integer_obj = std::dynamic_pointer_cast<Integer>(actual.at(i));
        if (!integer_obj) {
            std::cerr << "object is not Integer." << std::endl;
            return false;
        }

        if (integer_obj->value != expected.at(i)) {
            std::cerr << "object has wrong value. got=" << integer_obj->value << ", want=" << expected.at(i) << std::endl;
            return false;
        }
    }

    return true;
}

TEST_CASE("Test Integer Arithmetic") {
    std::vector<std::tuple<std::string, std::vector<int>, std::vector<Instructions>>> tests = {
        std::make_tuple("1 + 2", std::vector<int>{1, 2}, std::vector<Instructions>{
            make(OpType::OpConstant, std::vector<int>{0}),
            make(OpType::OpConstant, std::vector<int>{1}),
            make(OpType::OpAdd, std::vector<int>{}),
            make(OpType::OpPop, std::vector<int>{}),
        }),
        std::make_tuple("1; 2", std::vector<int>{1, 2}, std::vector<Instructions>{
            make(OpType::OpConstant, std::vector<int>{0}),
            make(OpType::OpPop, std::vector<int>{}),
            make(OpType::OpConstant, std::vector<int>{1}),
            make(OpType::OpPop, std::vector<int>{}),
        }),
        std::make_tuple("1 - 2", std::vector<int>{1, 2}, std::vector<Instructions>{
                make(OpType::OpConstant, std::vector<int>{0}),
                make(OpType::OpConstant, std::vector<int>{1}),
                make(OpType::OpSub, std::vector<int>{}),
                make(OpType::OpPop, std::vector<int>{}),
        }),
        std::make_tuple("1 * 2", std::vector<int>{1, 2}, std::vector<Instructions>{
                make(OpType::OpConstant, std::vector<int>{0}),
                make(OpType::OpConstant, std::vector<int>{1}),
                make(OpType::OpMul, std::vector<int>{}),
                make(OpType::OpPop, std::vector<int>{}),
        }),
        std::make_tuple("2 / 1", std::vector<int>{2, 1}, std::vector<Instructions>{
                make(OpType::OpConstant, std::vector<int>{0}),
                make(OpType::OpConstant, std::vector<int>{1}),
                make(OpType::OpDiv, std::vector<int>{}),
                make(OpType::OpPop, std::vector<int>{}),
        }),
        std::make_tuple("-1", std::vector<int>{1}, std::vector<Instructions>{
                make(OpType::OpConstant, std::vector<int>{0}),
                make(OpType::OpMinus, std::vector<int>{}),
                make(OpType::OpPop, std::vector<int>{}),
        }),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected_constants, tt_expected_instructions] = tt;

        auto program = parse(tt_input);

        auto compiler = new_compiler();

        auto err = compiler->compile(program);
        if (err) {
            std::cerr << "compiler error: " << err->message << std::endl;
        }
        REQUIRE(!err);

        auto bytecode = compiler->bytecode();

        REQUIRE(test_instructions(tt_expected_instructions, bytecode->instructions));

        REQUIRE(test_integer_constants(tt_expected_constants, bytecode->constants));
    }
}

TEST_CASE("Test Boolean Expressions") {
    std::vector<std::tuple<std::string, std::vector<int>, std::vector<Instructions>>> tests = {
            std::make_tuple("true", std::vector<int>{}, std::vector<Instructions>{
                    make(OpType::OpTrue, std::vector<int>{}),
                    make(OpType::OpPop, std::vector<int>{}),
            }),
            std::make_tuple("false", std::vector<int>{}, std::vector<Instructions>{
                    make(OpType::OpFalse, std::vector<int>{}),
                    make(OpType::OpPop, std::vector<int>{}),
            }),
            std::make_tuple("1 > 2", std::vector<int>{1, 2}, std::vector<Instructions>{
                    make(OpType::OpConstant, std::vector<int>{0}),
                    make(OpType::OpConstant, std::vector<int>{1}),
                    make(OpType::OpGreaterThan, std::vector<int>{}),
                    make(OpType::OpPop, std::vector<int>{}),
            }),
            std::make_tuple("1 < 2", std::vector<int>{2, 1}, std::vector<Instructions>{
                    make(OpType::OpConstant, std::vector<int>{0}),
                    make(OpType::OpConstant, std::vector<int>{1}),
                    make(OpType::OpGreaterThan, std::vector<int>{}),
                    make(OpType::OpPop, std::vector<int>{}),
            }),
            std::make_tuple("1 == 2", std::vector<int>{1, 2}, std::vector<Instructions>{
                    make(OpType::OpConstant, std::vector<int>{0}),
                    make(OpType::OpConstant, std::vector<int>{1}),
                    make(OpType::OpEqual, std::vector<int>{}),
                    make(OpType::OpPop, std::vector<int>{}),
            }),
            std::make_tuple("1 != 2", std::vector<int>{1, 2}, std::vector<Instructions>{
                    make(OpType::OpConstant, std::vector<int>{0}),
                    make(OpType::OpConstant, std::vector<int>{1}),
                    make(OpType::OpNotEqual, std::vector<int>{}),
                    make(OpType::OpPop, std::vector<int>{}),
            }),
            std::make_tuple("true == false", std::vector<int>{}, std::vector<Instructions>{
                    make(OpType::OpTrue, std::vector<int>{}),
                    make(OpType::OpFalse, std::vector<int>{}),
                    make(OpType::OpEqual, std::vector<int>{}),
                    make(OpType::OpPop, std::vector<int>{}),
            }),
            std::make_tuple("true != false", std::vector<int>{}, std::vector<Instructions>{
                    make(OpType::OpTrue, std::vector<int>{}),
                    make(OpType::OpFalse, std::vector<int>{}),
                    make(OpType::OpNotEqual, std::vector<int>{}),
                    make(OpType::OpPop, std::vector<int>{}),
            }),
            std::make_tuple("!true", std::vector<int>{}, std::vector<Instructions>{
                    make(OpType::OpTrue, std::vector<int>{}),
                    make(OpType::OpBang, std::vector<int>{}),
                    make(OpType::OpPop, std::vector<int>{}),
            }),
    };

    for (const auto &tt: tests) {
        const auto [tt_input, tt_expected_constants, tt_expected_instructions] = tt;

        auto program = parse(tt_input);

        auto compiler = new_compiler();

        auto err = compiler->compile(program);
        if (err) {
            std::cerr << "compiler error: " << err->message << std::endl;
        }
        REQUIRE(!err);

        auto bytecode = compiler->bytecode();

        REQUIRE(test_instructions(tt_expected_instructions, bytecode->instructions));
    }
}
