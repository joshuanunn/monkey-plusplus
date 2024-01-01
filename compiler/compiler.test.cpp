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
        if (actual.at(i) != ins) {
            std::cerr << "wrong instruction at " << i << ".\nwant=\n" << concatted << "\ngot =\n" << actual << std::endl;
            return false;
        }
        i++;
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

bool test_string_constants(const std::vector<std::string>& expected, const std::vector<std::shared_ptr<Object>>& actual) {
    if (expected.size() != actual.size()) {
        std::cerr << "wrong number of constants. got=" << actual.size() << ", want=" << expected.size() << std::endl;
        return false;
    }

    for (int i = 0; i < expected.size(); i++) {
        auto string_obj = std::dynamic_pointer_cast<String>(actual.at(i));
        if (!string_obj) {
            std::cerr << "object is not String." << std::endl;
            return false;
        }

        if (string_obj->value != expected.at(i)) {
            std::cerr << "object has wrong value. got=" << string_obj->value << ", want=" << expected.at(i) << std::endl;
            return false;
        }
    }

    return true;
}

bool test_function_constants(const std::vector<Instructions>& expected, const std::shared_ptr<Object>& actual) {
    auto compiledfunction_obj = std::dynamic_pointer_cast<CompiledFunction>(actual);
    if (!compiledfunction_obj) {
        std::cerr << "constant is not a function." << std::endl;
        return false;
    }

    auto ok = test_instructions(expected, compiledfunction_obj->instructions);
    if (!ok) {
        std::cerr << "constant test_instructions failed." << std::endl;
        return false;
    }

    return true;
}

TEST_CASE("Test Integer Arithmetic") {
    std::vector<std::tuple<std::string, std::vector<int>, std::vector<Instructions>>> tests = {
        std::make_tuple("1 + 2", std::vector<int>{1, 2}, std::vector<Instructions>{
            make(OpType::OpConstant, 0),
            make(OpType::OpConstant, 1),
            make(OpType::OpAdd),
            make(OpType::OpPop),
        }),
        std::make_tuple("1; 2", std::vector<int>{1, 2}, std::vector<Instructions>{
            make(OpType::OpConstant, 0),
            make(OpType::OpPop),
            make(OpType::OpConstant, 1),
            make(OpType::OpPop),
        }),
        std::make_tuple("1 - 2", std::vector<int>{1, 2}, std::vector<Instructions>{
                make(OpType::OpConstant, 0),
                make(OpType::OpConstant, 1),
                make(OpType::OpSub),
                make(OpType::OpPop),
        }),
        std::make_tuple("1 * 2", std::vector<int>{1, 2}, std::vector<Instructions>{
                make(OpType::OpConstant, 0),
                make(OpType::OpConstant, 1),
                make(OpType::OpMul),
                make(OpType::OpPop),
        }),
        std::make_tuple("2 / 1", std::vector<int>{2, 1}, std::vector<Instructions>{
                make(OpType::OpConstant, 0),
                make(OpType::OpConstant, 1),
                make(OpType::OpDiv),
                make(OpType::OpPop),
        }),
        std::make_tuple("-1", std::vector<int>{1}, std::vector<Instructions>{
                make(OpType::OpConstant, 0),
                make(OpType::OpMinus),
                make(OpType::OpPop),
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
                    make(OpType::OpTrue),
                    make(OpType::OpPop),
            }),
            std::make_tuple("false", std::vector<int>{}, std::vector<Instructions>{
                    make(OpType::OpFalse),
                    make(OpType::OpPop),
            }),
            std::make_tuple("1 > 2", std::vector<int>{1, 2}, std::vector<Instructions>{
                    make(OpType::OpConstant, 0),
                    make(OpType::OpConstant, 1),
                    make(OpType::OpGreaterThan),
                    make(OpType::OpPop),
            }),
            std::make_tuple("1 < 2", std::vector<int>{2, 1}, std::vector<Instructions>{
                    make(OpType::OpConstant, 0),
                    make(OpType::OpConstant, 1),
                    make(OpType::OpGreaterThan),
                    make(OpType::OpPop),
            }),
            std::make_tuple("1 == 2", std::vector<int>{1, 2}, std::vector<Instructions>{
                    make(OpType::OpConstant, 0),
                    make(OpType::OpConstant, 1),
                    make(OpType::OpEqual),
                    make(OpType::OpPop),
            }),
            std::make_tuple("1 != 2", std::vector<int>{1, 2}, std::vector<Instructions>{
                    make(OpType::OpConstant, 0),
                    make(OpType::OpConstant, 1),
                    make(OpType::OpNotEqual),
                    make(OpType::OpPop),
            }),
            std::make_tuple("true == false", std::vector<int>{}, std::vector<Instructions>{
                    make(OpType::OpTrue),
                    make(OpType::OpFalse),
                    make(OpType::OpEqual),
                    make(OpType::OpPop),
            }),
            std::make_tuple("true != false", std::vector<int>{}, std::vector<Instructions>{
                    make(OpType::OpTrue),
                    make(OpType::OpFalse),
                    make(OpType::OpNotEqual),
                    make(OpType::OpPop),
            }),
            std::make_tuple("!true", std::vector<int>{}, std::vector<Instructions>{
                    make(OpType::OpTrue),
                    make(OpType::OpBang),
                    make(OpType::OpPop),
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

TEST_CASE("Test Conditionals") {
    std::vector<std::tuple<std::string, std::vector<int>, std::vector<Instructions>>> tests = {
            std::make_tuple("if (true) { 10 }; 3333;", std::vector<int>{10, 3333}, std::vector<Instructions>{
                    make(OpType::OpTrue),
                    make(OpType::OpJumpNotTruthy, 10),
                    make(OpType::OpConstant, 0),
                    make(OpType::OpJump, 11),
                    make(OpType::OpNull),
                    make(OpType::OpPop),
                    make(OpType::OpConstant, 1),
                    make(OpType::OpPop),
            }),
            std::make_tuple("if (true) { 10 } else { 20 }; 3333;", std::vector<int>{10, 20, 3333}, std::vector<Instructions>{
                    make(OpType::OpTrue),
                    make(OpType::OpJumpNotTruthy, 10),
                    make(OpType::OpConstant, 0),
                    make(OpType::OpJump, 13),
                    make(OpType::OpConstant, 1),
                    make(OpType::OpPop),
                    make(OpType::OpConstant, 2),
                    make(OpType::OpPop),
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

TEST_CASE("Test Global Let Statements") {
    std::vector<std::tuple<std::string, std::vector<int>, std::vector<Instructions>>> tests = {
            std::make_tuple(R"(
let one = 1;
let two = 2;
)",         std::vector<int>{1, 2}, std::vector<Instructions>{
                    make(OpType::OpConstant, 0),
                    make(OpType::OpSetGlobal, 0),
                    make(OpType::OpConstant, 1),
                    make(OpType::OpSetGlobal, 1),
            }),
            std::make_tuple(R"(
let one = 1;
one;
)",         std::vector<int>{1}, std::vector<Instructions>{
                    make(OpType::OpConstant, 0),
                    make(OpType::OpSetGlobal, 0),
                    make(OpType::OpGetGlobal, 0),
                    make(OpType::OpPop),
            }),
            std::make_tuple(R"(
let one = 1;
let two = one;
two;
)",         std::vector<int>{1}, std::vector<Instructions>{
                    make(OpType::OpConstant, 0),
                    make(OpType::OpSetGlobal, 0),
                    make(OpType::OpGetGlobal, 0),
                    make(OpType::OpSetGlobal, 1),
                    make(OpType::OpGetGlobal, 1),
                    make(OpType::OpPop),
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

TEST_CASE("Test String Expressions") {
    std::vector<std::tuple<std::string, std::vector<std::string>, std::vector<Instructions>>> tests = {
            std::make_tuple(
                R"("monkey")",
                std::vector<std::string>{"monkey"},
                std::vector<Instructions>{
                    make(OpType::OpConstant, 0),
                    make(OpType::OpPop),
            }),
            std::make_tuple(
                R"("mon" + "key")",
                std::vector<std::string>{"mon", "key"},
                std::vector<Instructions>{
                    make(OpType::OpConstant, 0),
                    make(OpType::OpConstant, 1),
                    make(OpType::OpAdd),
                    make(OpType::OpPop),
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

        REQUIRE(test_string_constants(tt_expected_constants, bytecode->constants));
    }
}

TEST_CASE("Test Array Literals") {
    std::vector<std::tuple<std::string, std::vector<int>, std::vector<Instructions>>> tests = {
            std::make_tuple("[]", std::vector<int>{},
                std::vector<Instructions>{
                    make(OpType::OpArray, 0),
                    make(OpType::OpPop),
            }),
            std::make_tuple("[1, 2, 3]", std::vector<int>{1, 2, 3},
                std::vector<Instructions>{
                    make(OpType::OpConstant, 0),
                    make(OpType::OpConstant, 1),
                    make(OpType::OpConstant, 2),
                    make(OpType::OpArray, 3),
                    make(OpType::OpPop),
            }),
            std::make_tuple("[1 + 2, 3 - 4, 5 * 6]", std::vector<int>{1, 2, 3, 4, 5, 6},
                std::vector<Instructions>{
                    make(OpType::OpConstant, 0),
                    make(OpType::OpConstant, 1),
                    make(OpType::OpAdd),
                    make(OpType::OpConstant, 2),
                    make(OpType::OpConstant, 3),
                    make(OpType::OpSub),
                    make(OpType::OpConstant, 4),
                    make(OpType::OpConstant, 5),
                    make(OpType::OpMul),
                    make(OpType::OpArray, 3),
                    make(OpType::OpPop),
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

TEST_CASE("Test Hash Literals") {
    std::vector<std::tuple<std::string, std::vector<int>, std::vector<Instructions>>> tests = {
            std::make_tuple("{}", std::vector<int>{},
                std::vector<Instructions>{
                    make(OpType::OpHash, 0),
                    make(OpType::OpPop),
            }),
            std::make_tuple("{1: 2, 3: 4, 5: 6}", std::vector<int>{1, 2, 3, 4, 5, 6},
                std::vector<Instructions>{
                    make(OpType::OpConstant, 0),
                    make(OpType::OpConstant, 1),
                    make(OpType::OpConstant, 2),
                    make(OpType::OpConstant, 3),
                    make(OpType::OpConstant, 4),
                    make(OpType::OpConstant, 5),
                    make(OpType::OpHash, 6),
                    make(OpType::OpPop),
            }),
            std::make_tuple("{1: 2 + 3, 4: 5 * 6}", std::vector<int>{1, 2, 3, 4, 5, 6},
                std::vector<Instructions>{
                    make(OpType::OpConstant, 0),
                    make(OpType::OpConstant, 1),
                    make(OpType::OpConstant, 2),
                    make(OpType::OpAdd),
                    make(OpType::OpConstant, 3),
                    make(OpType::OpConstant, 4),
                    make(OpType::OpConstant, 5),
                    make(OpType::OpMul),
                    make(OpType::OpHash, 4),
                    make(OpType::OpPop),
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

TEST_CASE("Test Index Expressions") {
    std::vector<std::tuple<std::string, std::vector<int>, std::vector<Instructions>>> tests = {
            std::make_tuple("[1, 2, 3][1 + 1]", std::vector<int>{1, 2, 3, 1, 1},
                std::vector<Instructions>{
                    make(OpType::OpConstant, 0),
                    make(OpType::OpConstant, 1),
                    make(OpType::OpConstant, 2),
                    make(OpType::OpArray, 3),
                    make(OpType::OpConstant, 3),
                    make(OpType::OpConstant, 4),
                    make(OpType::OpAdd),
                    make(OpType::OpIndex),
                    make(OpType::OpPop),
            }),
            std::make_tuple("{1: 2}[2 - 1]", std::vector<int>{1, 2, 2, 1},
                std::vector<Instructions>{
                    make(OpType::OpConstant, 0),
                    make(OpType::OpConstant, 1),
                    make(OpType::OpHash, 2),
                    make(OpType::OpConstant, 2),
                    make(OpType::OpConstant, 3),
                    make(OpType::OpSub),
                    make(OpType::OpIndex),
                    make(OpType::OpPop),
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

TEST_CASE("Test Compiler Scopes") {
    EmittedInstruction last;
    EmittedInstruction previous;

    auto compiler = new_compiler();

    if (compiler->scope_index != 0) {
        std::cerr << "scope_index wrong. got=" << compiler->scope_index << ", want=" << 0 << std::endl;
    }
    REQUIRE(compiler->scope_index == 0);

    auto global_symbol_table = compiler->symbol_table;

    compiler->emit(OpType::OpMul);

    compiler->enter_scope();

    if (compiler->scope_index != 1) {
        std::cerr << "scope_index wrong. got=" << compiler->scope_index << ", want=" << 1 << std::endl;
    }
    REQUIRE(compiler->scope_index == 1);

    compiler->emit(OpType::OpSub);

    if (compiler->scopes.at(compiler->scope_index).instructions.size() != 1) {
        std::cerr << "instructions length wrong. got="
            << compiler->scopes.at(compiler->scope_index).instructions.size() << std::endl;
    }
    REQUIRE(compiler->scopes.at(compiler->scope_index).instructions.size() == 1);

    last = compiler->scopes.at(compiler->scope_index).last_instruction;
    if (last.opcode != OpType::OpSub) {
        std::cerr << "last_instruction.opcode wrong. got=" << std::to_string(as_opcode(last.opcode))
            << ", want=" << std::to_string(as_opcode(OpType::OpSub)) << std::endl;
    }
    REQUIRE(last.opcode == OpType::OpSub);

    if (compiler->symbol_table->outer != global_symbol_table) {
        std::cerr << "compiler did not enclose symbol table" << std::endl;    
    }
    REQUIRE(compiler->symbol_table->outer == global_symbol_table);

    compiler->leave_scope();

    if (compiler->scope_index != 0) {
        std::cerr << "scope_index wrong. got=" << compiler->scope_index << ", want=" << 0 << std::endl;
    }
    REQUIRE(compiler->scope_index == 0);

    if (compiler->symbol_table != global_symbol_table) {
        std::cerr << "compiler did not restore global symbol table" << std::endl;    
    }
    REQUIRE(compiler->symbol_table == global_symbol_table);

    if (compiler->symbol_table->outer) {
        std::cerr << "compiler modified global symbol table incorrectly" << std::endl;    
    }
    REQUIRE(!compiler->symbol_table->outer);

    compiler->emit(OpType::OpAdd);

    if (compiler->scopes.at(compiler->scope_index).instructions.size() != 2) {
        std::cerr << "instructions length wrong. got="
            << compiler->scopes.at(compiler->scope_index).instructions.size() << std::endl;
    }
    REQUIRE(compiler->scopes.at(compiler->scope_index).instructions.size() == 2);

    last = compiler->scopes.at(compiler->scope_index).last_instruction;
    if (last.opcode != OpType::OpAdd) {
        std::cerr << "last_instruction.opcode wrong. got=" << std::to_string(as_opcode(last.opcode))
            << ", want=" << std::to_string(as_opcode(OpType::OpAdd)) << std::endl;
    }
    REQUIRE(last.opcode == OpType::OpAdd);

    previous = compiler->scopes.at(compiler->scope_index).previous_instruction;
    if (previous.opcode != OpType::OpMul) {
        std::cerr << "previous_instruction.opcode wrong. got=" << std::to_string(as_opcode(previous.opcode))
            << ", want=" << std::to_string(as_opcode(OpType::OpMul)) << std::endl;
    }
    REQUIRE(previous.opcode == OpType::OpMul);
}

TEST_CASE("Test Function With Return") {
    std::string input = "fn() { return 5 + 10 }";
    std::vector<int> expected_integer_constants = std::vector<int>{5, 10};
    std::vector<Instructions> expected_fn_instructions = {
        make(OpType::OpConstant, 0),
        make(OpType::OpConstant, 1),
        make(OpType::OpAdd),
        make(OpType::OpReturnValue),
    };
    std::vector<Instructions> expected_instructions = {
        make(OpType::OpClosure, 2, 0),
        make(OpType::OpPop),
    };

    auto program = parse(input);

    auto compiler = new_compiler();

    auto err = compiler->compile(program);
    if (err) {
        std::cerr << "compiler error: " << err->message << std::endl;
    }
    REQUIRE(!err);

    auto bytecode = compiler->bytecode();

    REQUIRE(test_instructions(expected_instructions, bytecode->instructions));

    // Test integer constants
    auto int_constants = std::vector<std::shared_ptr<Object>>(bytecode->constants.begin(), bytecode->constants.end() - 1);
    REQUIRE(test_integer_constants(expected_integer_constants, int_constants));

    // Test compiled function constants
    auto fn_constants = bytecode->constants.back();
    REQUIRE(test_function_constants(expected_fn_instructions, fn_constants));
}

TEST_CASE("Test Function With No Return") {
    std::string input = "fn() { 5 + 10 }";
    std::vector<int> expected_integer_constants = std::vector<int>{5, 10};
    std::vector<Instructions> expected_fn_instructions = {
        make(OpType::OpConstant, 0),
        make(OpType::OpConstant, 1),
        make(OpType::OpAdd),
        make(OpType::OpReturnValue),
    };
    std::vector<Instructions> expected_instructions = {
        make(OpType::OpClosure, 2, 0),
        make(OpType::OpPop),
    };

    auto program = parse(input);

    auto compiler = new_compiler();

    auto err = compiler->compile(program);
    if (err) {
        std::cerr << "compiler error: " << err->message << std::endl;
    }
    REQUIRE(!err);

    auto bytecode = compiler->bytecode();

    REQUIRE(test_instructions(expected_instructions, bytecode->instructions));

    // Test integer constants
    auto int_constants = std::vector<std::shared_ptr<Object>>(bytecode->constants.begin(), bytecode->constants.end() - 1);
    REQUIRE(test_integer_constants(expected_integer_constants, int_constants));

    // Test compiled function constants
    auto fn_constants = bytecode->constants.back();
    REQUIRE(test_function_constants(expected_fn_instructions, fn_constants));
}

TEST_CASE("Test Function With No Return And Two Expression Statements") {
    std::string input = "fn() { 1; 2 }";
    std::vector<int> expected_integer_constants = std::vector<int>{1, 2};
    std::vector<Instructions> expected_fn_instructions = {
        make(OpType::OpConstant, 0),
        make(OpType::OpPop),
        make(OpType::OpConstant, 1),
        make(OpType::OpReturnValue),
    };
    std::vector<Instructions> expected_instructions = {
        make(OpType::OpClosure, 2, 0),
        make(OpType::OpPop),
    };

    auto program = parse(input);

    auto compiler = new_compiler();

    auto err = compiler->compile(program);
    if (err) {
        std::cerr << "compiler error: " << err->message << std::endl;
    }
    REQUIRE(!err);

    auto bytecode = compiler->bytecode();

    REQUIRE(test_instructions(expected_instructions, bytecode->instructions));

    // Test integer constants
    auto int_constants = std::vector<std::shared_ptr<Object>>(bytecode->constants.begin(), bytecode->constants.end() - 1);
    REQUIRE(test_integer_constants(expected_integer_constants, int_constants));

    // Test compiled function constants
    auto fn_constants = bytecode->constants.back();
    REQUIRE(test_function_constants(expected_fn_instructions, fn_constants));
}

TEST_CASE("Test Function With No Return Value") {
    std::string input = "fn() { }";
    std::vector<Instructions> expected_fn_instructions = {
        make(OpType::OpReturn),
    };
    std::vector<Instructions> expected_instructions = {
        make(OpType::OpClosure, 0, 0),
        make(OpType::OpPop),
    };

    auto program = parse(input);

    auto compiler = new_compiler();

    auto err = compiler->compile(program);
    if (err) {
        std::cerr << "compiler error: " << err->message << std::endl;
    }
    REQUIRE(!err);

    auto bytecode = compiler->bytecode();

    REQUIRE(test_instructions(expected_instructions, bytecode->instructions));

    // Test compiled function constants
    auto fn_constants = bytecode->constants.back();
    REQUIRE(test_function_constants(expected_fn_instructions, fn_constants));
}

TEST_CASE("Test Function Calls") {
    std::string input = "fn() { 24 }()";
    std::vector<int> expected_integer_constants = std::vector<int>{24};
    std::vector<Instructions> expected_fn_instructions = {
        make(OpType::OpConstant, 0), // The literal "24"
        make(OpType::OpReturnValue),
    };
    std::vector<Instructions> expected_instructions = {
        make(OpType::OpClosure, 1, 0), // The compiled function
        make(OpType::OpCall, 0),
        make(OpType::OpPop),
    };

    auto program = parse(input);

    auto compiler = new_compiler();

    auto err = compiler->compile(program);
    if (err) {
        std::cerr << "compiler error: " << err->message << std::endl;
    }
    REQUIRE(!err);

    auto bytecode = compiler->bytecode();

    REQUIRE(test_instructions(expected_instructions, bytecode->instructions));

    // Test integer constants
    auto int_constants = std::vector<std::shared_ptr<Object>>(bytecode->constants.begin(), bytecode->constants.end() - 1);
    REQUIRE(test_integer_constants(expected_integer_constants, int_constants));

    // Test compiled function constants
    auto fn_constants = bytecode->constants.back();
    REQUIRE(test_function_constants(expected_fn_instructions, fn_constants));
}

TEST_CASE("Test Function Calls With Binding") {
    std::string input = R"(
let noArg = fn() { 24 };
noArg();
)";
    std::vector<int> expected_integer_constants = std::vector<int>{24};
    std::vector<Instructions> expected_fn_instructions = {
        make(OpType::OpConstant, 0), // The literal "24"
        make(OpType::OpReturnValue),
    };
    std::vector<Instructions> expected_instructions = {
        make(OpType::OpClosure, 1, 0), // The compiled function
        make(OpType::OpSetGlobal, 0),
        make(OpType::OpGetGlobal, 0),
        make(OpType::OpCall, 0),
        make(OpType::OpPop),
    };

    auto program = parse(input);

    auto compiler = new_compiler();

    auto err = compiler->compile(program);
    if (err) {
        std::cerr << "compiler error: " << err->message << std::endl;
    }
    REQUIRE(!err);

    auto bytecode = compiler->bytecode();

    REQUIRE(test_instructions(expected_instructions, bytecode->instructions));

    // Test integer constants
    auto int_constants = std::vector<std::shared_ptr<Object>>(bytecode->constants.begin(), bytecode->constants.end() - 1);
    REQUIRE(test_integer_constants(expected_integer_constants, int_constants));

    // Test compiled function constants
    auto fn_constants = bytecode->constants.back();
    REQUIRE(test_function_constants(expected_fn_instructions, fn_constants));
}

TEST_CASE("Test Let Statement Scopes 1") {
    std::string input = R"(
let num = 55;
fn() { num }
)";
    std::vector<int> expected_integer_constants = std::vector<int>{55};
    std::vector<Instructions> expected_fn_instructions = {
        make(OpType::OpGetGlobal, 0),
        make(OpType::OpReturnValue),
    };
    std::vector<Instructions> expected_instructions = {
        make(OpType::OpConstant, 0),
        make(OpType::OpSetGlobal, 0),
        make(OpType::OpClosure, 1, 0),
        make(OpType::OpPop),
    };

    auto program = parse(input);

    auto compiler = new_compiler();

    auto err = compiler->compile(program);
    if (err) {
        std::cerr << "compiler error: " << err->message << std::endl;
    }
    REQUIRE(!err);

    auto bytecode = compiler->bytecode();

    REQUIRE(test_instructions(expected_instructions, bytecode->instructions));

    // Test integer constants
    auto int_constants = std::vector<std::shared_ptr<Object>>(bytecode->constants.begin(), bytecode->constants.end() - 1);
    REQUIRE(test_integer_constants(expected_integer_constants, int_constants));

    // Test compiled function constants
    auto fn_constants = bytecode->constants.back();
    REQUIRE(test_function_constants(expected_fn_instructions, fn_constants));
}

TEST_CASE("Test Let Statement Scopes 2") {
    std::string input = R"(
fn() {
    let num = 55;
    num
}
)";
    std::vector<int> expected_integer_constants = std::vector<int>{55};
    std::vector<Instructions> expected_fn_instructions = {
        make(OpType::OpConstant, 0),
        make(OpType::OpSetLocal, 0),
        make(OpType::OpGetLocal, 0),
        make(OpType::OpReturnValue),
    };
    std::vector<Instructions> expected_instructions = {
        make(OpType::OpClosure, 1, 0),
        make(OpType::OpPop),
    };

    auto program = parse(input);

    auto compiler = new_compiler();

    auto err = compiler->compile(program);
    if (err) {
        std::cerr << "compiler error: " << err->message << std::endl;
    }
    REQUIRE(!err);

    auto bytecode = compiler->bytecode();

    REQUIRE(test_instructions(expected_instructions, bytecode->instructions));

    // Test integer constants
    auto int_constants = std::vector<std::shared_ptr<Object>>(bytecode->constants.begin(), bytecode->constants.end() - 1);
    REQUIRE(test_integer_constants(expected_integer_constants, int_constants));

    // Test compiled function constants
    auto fn_constants = bytecode->constants.back();
    REQUIRE(test_function_constants(expected_fn_instructions, fn_constants));
}

TEST_CASE("Test Let Statement Scopes 3") {
    std::string input = R"(
fn() {
    let a = 55;
    let b = 77;
    a + b
}
)";
    std::vector<int> expected_integer_constants = std::vector<int>{55, 77};
    std::vector<Instructions> expected_fn_instructions = {
        make(OpType::OpConstant, 0),
        make(OpType::OpSetLocal, 0),
        make(OpType::OpConstant, 1),
        make(OpType::OpSetLocal, 1),
        make(OpType::OpGetLocal, 0),
        make(OpType::OpGetLocal, 1),
        make(OpType::OpAdd),
        make(OpType::OpReturnValue),
    };
    std::vector<Instructions> expected_instructions = {
        make(OpType::OpClosure, 2, 0),
        make(OpType::OpPop),
    };

    auto program = parse(input);

    auto compiler = new_compiler();

    auto err = compiler->compile(program);
    if (err) {
        std::cerr << "compiler error: " << err->message << std::endl;
    }
    REQUIRE(!err);

    auto bytecode = compiler->bytecode();

    REQUIRE(test_instructions(expected_instructions, bytecode->instructions));

    // Test integer constants
    auto int_constants = std::vector<std::shared_ptr<Object>>(bytecode->constants.begin(), bytecode->constants.end() - 1);
    REQUIRE(test_integer_constants(expected_integer_constants, int_constants));

    // Test compiled function constants
    auto fn_constants = bytecode->constants.back();
    REQUIRE(test_function_constants(expected_fn_instructions, fn_constants));
}

TEST_CASE("Test Function Calls With One Argument") {
    std::string input = R"(
let oneArg = fn(a) { a };
oneArg(24);
)";
    std::vector<int> expected_integer_constants = std::vector<int>{24};
    std::vector<Instructions> expected_fn_instructions = {
        make(OpType::OpGetLocal, 0),
        make(OpType::OpReturnValue),
    };
    std::vector<Instructions> expected_instructions = {
        make(OpType::OpClosure, 0, 0),
        make(OpType::OpSetGlobal, 0),
        make(OpType::OpGetGlobal, 0),
        make(OpType::OpConstant, 1),
        make(OpType::OpCall, 1),
        make(OpType::OpPop),
    };

    auto program = parse(input);

    auto compiler = new_compiler();

    auto err = compiler->compile(program);
    if (err) {
        std::cerr << "compiler error: " << err->message << std::endl;
    }
    REQUIRE(!err);

    auto bytecode = compiler->bytecode();

    REQUIRE(test_instructions(expected_instructions, bytecode->instructions));

    // Test compiled function constants (first on stack)
    auto fn_constants = bytecode->constants.front();
    REQUIRE(test_function_constants(expected_fn_instructions, fn_constants));

    // Test integer arguments (second on stack)
    auto int_constants = std::vector<std::shared_ptr<Object>>(bytecode->constants.begin() + 1, bytecode->constants.end());
    REQUIRE(test_integer_constants(expected_integer_constants, int_constants));
}

TEST_CASE("Test Function Calls With Many Arguments") {
    std::string input = R"(
let manyArg = fn(a, b, c) { a; b; c };
manyArg(24, 25, 26);
)";
    std::vector<int> expected_integer_constants = std::vector<int>{24, 25, 26};
    std::vector<Instructions> expected_fn_instructions = {
        make(OpType::OpGetLocal, 0),
        make(OpType::OpPop),
        make(OpType::OpGetLocal, 1),
        make(OpType::OpPop),
        make(OpType::OpGetLocal, 2),
        make(OpType::OpReturnValue),
    };
    std::vector<Instructions> expected_instructions = {
        make(OpType::OpClosure, 0, 0),
        make(OpType::OpSetGlobal, 0),
        make(OpType::OpGetGlobal, 0),
        make(OpType::OpConstant, 1),
        make(OpType::OpConstant, 2),
        make(OpType::OpConstant, 3),
        make(OpType::OpCall, 3),
        make(OpType::OpPop),
    };

    auto program = parse(input);

    auto compiler = new_compiler();

    auto err = compiler->compile(program);
    if (err) {
        std::cerr << "compiler error: " << err->message << std::endl;
    }
    REQUIRE(!err);

    auto bytecode = compiler->bytecode();

    REQUIRE(test_instructions(expected_instructions, bytecode->instructions));

    // Test compiled function constants (first on stack)
    auto fn_constants = bytecode->constants.front();
    REQUIRE(test_function_constants(expected_fn_instructions, fn_constants));

    // Test integer arguments (second on stack)
    auto int_constants = std::vector<std::shared_ptr<Object>>(bytecode->constants.begin() + 1, bytecode->constants.end());
    REQUIRE(test_integer_constants(expected_integer_constants, int_constants));
}

TEST_CASE("Test Builtins") {
    std::string input = R"(
len([]);
push([], 1);
)";
    std::vector<int> expected_integer_constants = std::vector<int>{1};
    std::vector<Instructions> expected_instructions = {
        make(OpType::OpGetBuiltin, 0),
        make(OpType::OpArray, 0),
        make(OpType::OpCall, 1),
        make(OpType::OpPop),
        make(OpType::OpGetBuiltin, 5),
        make(OpType::OpArray, 0),
        make(OpType::OpConstant, 0),
        make(OpType::OpCall, 2),
        make(OpType::OpPop),
    };

    auto program = parse(input);

    auto compiler = new_compiler();

    auto err = compiler->compile(program);
    if (err) {
        std::cerr << "compiler error: " << err->message << std::endl;
    }
    REQUIRE(!err);

    auto bytecode = compiler->bytecode();

    REQUIRE(test_instructions(expected_instructions, bytecode->instructions));

    // Test integer arguments (second on stack)
    auto int_constants = std::vector<std::shared_ptr<Object>>(bytecode->constants);
    REQUIRE(test_integer_constants(expected_integer_constants, int_constants));
}

TEST_CASE("Test Builtins Within Functions") {
    std::string input = "fn() { len([]) }";
    std::vector<Instructions> expected_fn_instructions = {
        make(OpType::OpGetBuiltin, 0),
        make(OpType::OpArray, 0),
        make(OpType::OpCall, 1),
        make(OpType::OpReturnValue),
    };
    std::vector<Instructions> expected_instructions = {
        make(OpType::OpClosure, 0, 0),
        make(OpType::OpPop),
    };

    auto program = parse(input);

    auto compiler = new_compiler();

    auto err = compiler->compile(program);
    if (err) {
        std::cerr << "compiler error: " << err->message << std::endl;
    }
    REQUIRE(!err);

    auto bytecode = compiler->bytecode();

    REQUIRE(test_instructions(expected_instructions, bytecode->instructions));

    // Test compiled function constants (first on stack)
    auto fn_constants = bytecode->constants.front();
    REQUIRE(test_function_constants(expected_fn_instructions, fn_constants));
}

TEST_CASE("Test Closures") {
    std::string input = R"(
fn(a) {
    fn(b) {
        a + b
    }
}
)";
    std::vector<std::vector<Instructions>> expected_fn_constants = {
        {
            make(OpType::OpGetFree, 0),
            make(OpType::OpGetLocal, 0),
            make(OpType::OpAdd),
            make(OpType::OpReturnValue),
        },
        {
            make(OpType::OpGetLocal, 0),
            make(OpType::OpClosure, 0, 1),
            make(OpType::OpReturnValue),
        },
    };
    std::vector<Instructions> expected_instructions = {
        make(OpType::OpClosure, 1, 0),
        make(OpType::OpPop),
    };

    auto program = parse(input);

    auto compiler = new_compiler();

    auto err = compiler->compile(program);
    if (err) {
        std::cerr << "compiler error: " << err->message << std::endl;
    }
    REQUIRE(!err);

    auto bytecode = compiler->bytecode();

    REQUIRE(test_instructions(expected_instructions, bytecode->instructions));

    // Test compiled function constants
    for (int i = 0; i < expected_fn_constants.size(); i++) {
        REQUIRE(test_function_constants(expected_fn_constants.at(i), bytecode->constants.at(i)));
    }
}

TEST_CASE("Test Closures Deeply Nested") {
    std::string input = R"(
fn(a) {
    fn(b) {
        fn(c) {
            a + b + c
        }
    }
}
)";
    std::vector<std::vector<Instructions>> expected_fn_constants = {
        {
            make(OpType::OpGetFree, 0),
            make(OpType::OpGetFree, 1),
            make(OpType::OpAdd),
            make(OpType::OpGetLocal, 0),
            make(OpType::OpAdd),
            make(OpType::OpReturnValue),
        },
        {
            make(OpType::OpGetFree, 0),
            make(OpType::OpGetLocal, 0),
            make(OpType::OpClosure, 0, 2),
            make(OpType::OpReturnValue),
        },
        {
            make(OpType::OpGetLocal, 0),
            make(OpType::OpClosure, 1, 1),
            make(OpType::OpReturnValue),
        },
    };
    std::vector<Instructions> expected_instructions = {
        make(OpType::OpClosure, 2, 0),
        make(OpType::OpPop),
    };

    auto program = parse(input);

    auto compiler = new_compiler();

    auto err = compiler->compile(program);
    if (err) {
        std::cerr << "compiler error: " << err->message << std::endl;
    }
    REQUIRE(!err);

    auto bytecode = compiler->bytecode();

    REQUIRE(test_instructions(expected_instructions, bytecode->instructions));

    // Test compiled function constants (first on stack)
    for (int i = 0; i < expected_fn_constants.size(); i++) {
        REQUIRE(test_function_constants(expected_fn_constants.at(i), bytecode->constants.at(i)));
    }
}

TEST_CASE("Test Closures Deeply Nested With Local and Global Bindings") {
    std::string input = R"(
let global = 55;

fn() {
    let a = 66;

    fn() {
        let b = 77;

        fn() {
            let c = 88;

            global + a + b + c;
        }
    }
}
)";
    std::vector<int> expected_integer_constants = {55, 66, 77, 88};
    std::vector<std::vector<Instructions>> expected_fn_constants = {
        {
            make(OpType::OpConstant, 3),
            make(OpType::OpSetLocal, 0),
            make(OpType::OpGetGlobal, 0),
            make(OpType::OpGetFree, 0),
            make(OpType::OpAdd),
            make(OpType::OpGetFree, 1),
            make(OpType::OpAdd),
            make(OpType::OpGetLocal, 0),
            make(OpType::OpAdd),
            make(OpType::OpReturnValue),
        },
        {
            make(OpType::OpConstant, 2),
            make(OpType::OpSetLocal, 0),
            make(OpType::OpGetFree, 0),
            make(OpType::OpGetLocal, 0),
            make(OpType::OpClosure, 4, 2),
            make(OpType::OpReturnValue),
        },
        {
            make(OpType::OpConstant, 1),
            make(OpType::OpSetLocal, 0),
            make(OpType::OpGetLocal, 0),
            make(OpType::OpClosure, 5, 1),
            make(OpType::OpReturnValue),
        },
    };
    std::vector<Instructions> expected_instructions = {
        make(OpType::OpConstant, 0),
        make(OpType::OpSetGlobal, 0),
        make(OpType::OpClosure, 6, 0),
        make(OpType::OpPop),
    };

    auto program = parse(input);

    auto compiler = new_compiler();

    auto err = compiler->compile(program);
    if (err) {
        std::cerr << "compiler error: " << err->message << std::endl;
    }
    REQUIRE(!err);

    auto bytecode = compiler->bytecode();

    REQUIRE(test_instructions(expected_instructions, bytecode->instructions));

    // Test integer constants (first 4 on stack)
    auto first_four = std::vector<std::shared_ptr<Object>>(bytecode->constants.begin(), bytecode->constants.begin() + 4);
    REQUIRE(test_integer_constants(expected_integer_constants, first_four));

    // Test compiled function constants (last 3 on stack)
    for (int i = 0; i < expected_fn_constants.size(); i++) {
        REQUIRE(test_function_constants(expected_fn_constants.at(i), bytecode->constants.at(i + 4)));
    }
}

TEST_CASE("Test Recursive Functions") {
    std::string input = R"(
let countDown = fn(x) { countDown(x - 1); };
countDown(1);
)";
    std::vector<int> expected_integer_constants = {1, 1};
    std::vector<Instructions> expected_fn_constants = {
        make(OpType::OpCurrentClosure),
        make(OpType::OpGetLocal, 0),
        make(OpType::OpConstant, 0),
        make(OpType::OpSub),
        make(OpType::OpCall, 1),
        make(OpType::OpReturnValue),
    };
    std::vector<Instructions> expected_instructions = {
        make(OpType::OpClosure, 1, 0),
        make(OpType::OpSetGlobal, 0),
        make(OpType::OpGetGlobal, 0),
        make(OpType::OpConstant, 2),
        make(OpType::OpCall, 1),
        make(OpType::OpPop),
    };

    auto program = parse(input);

    auto compiler = new_compiler();

    auto err = compiler->compile(program);
    if (err) {
        std::cerr << "compiler error: " << err->message << std::endl;
    }
    REQUIRE(!err);

    auto bytecode = compiler->bytecode();

    REQUIRE(test_instructions(expected_instructions, bytecode->instructions));

    // Test integer constants (first and third on stack)
    std::vector<std::shared_ptr<Object>> actual_integer_constants;
    actual_integer_constants.push_back(bytecode->constants.at(0));
    actual_integer_constants.push_back(bytecode->constants.at(2));
    REQUIRE(test_integer_constants(expected_integer_constants, actual_integer_constants));

    // Test compiled function constants (second on stack)
    REQUIRE(test_function_constants(expected_fn_constants, bytecode->constants.at(1)));
}

TEST_CASE("Test Recursive Functions Defined In A Function") {
    std::string input = R"(
let wrapper = fn() {
    let countDown = fn(x) { countDown(x - 1); };
    countDown(1);
};
wrapper();
)";
    std::vector<int> expected_integer_constants = {1, 1};
    std::vector<std::vector<Instructions>> expected_fn_constants = {
        {
            make(OpType::OpCurrentClosure),
            make(OpType::OpGetLocal, 0),
            make(OpType::OpConstant, 0),
            make(OpType::OpSub),
            make(OpType::OpCall, 1),
            make(OpType::OpReturnValue),
        },
        {
            make(OpType::OpClosure, 1, 0),
            make(OpType::OpSetLocal, 0),
            make(OpType::OpGetLocal, 0),
            make(OpType::OpConstant, 2),
            make(OpType::OpCall, 1),
            make(OpType::OpReturnValue),
        },
    };
    std::vector<Instructions> expected_instructions = {
        make(OpType::OpClosure, 3, 0),
        make(OpType::OpSetGlobal, 0),
        make(OpType::OpGetGlobal, 0),
        make(OpType::OpCall, 0),
        make(OpType::OpPop),
    };

    auto program = parse(input);

    auto compiler = new_compiler();

    auto err = compiler->compile(program);
    if (err) {
        std::cerr << "compiler error: " << err->message << std::endl;
    }
    REQUIRE(!err);

    auto bytecode = compiler->bytecode();

    REQUIRE(test_instructions(expected_instructions, bytecode->instructions));

    // Test integer constants (first and third on stack)
    std::vector<std::shared_ptr<Object>> actual_integer_constants;
    actual_integer_constants.push_back(bytecode->constants.at(0));
    actual_integer_constants.push_back(bytecode->constants.at(2));
    REQUIRE(test_integer_constants(expected_integer_constants, actual_integer_constants));

    // Test compiled function constants (second and forth on stack)
    REQUIRE(test_function_constants(expected_fn_constants.at(0), bytecode->constants.at(1)));
    REQUIRE(test_function_constants(expected_fn_constants.at(1), bytecode->constants.at(3)));
}
