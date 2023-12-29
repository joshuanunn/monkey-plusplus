// Using Catch2 unit testing library

// Let Catch2 define a main function
#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"

#include "code.hpp"

TEST_CASE("Test Make") {
    std::vector<std::tuple<OpType, std::vector<int>, std::vector<Opcode>>> tests = {
            std::make_tuple(OpType::OpConstant, std::vector<int>{65534},
                            std::vector<Opcode>{as_opcode(OpType::OpConstant), 255, 254}),
            std::make_tuple(OpType::OpAdd, std::vector<int>{},
                            std::vector<Opcode>{as_opcode(OpType::OpAdd)}),
    };

    for (const auto &tt: tests) {
        const auto [tt_op, tt_operands, tt_expected] = tt;

        auto instruction = make(tt_op, tt_operands);

        if (instruction.size() != tt_expected.size()) {
            std::cerr << "instruction has wrong length. want=" << tt_expected.size() << ", got=" << instruction.size()
                      << std::endl;
        }
        REQUIRE(instruction.size() == tt_expected.size());

        for (int i = 0; i < tt_expected.size(); i++) {
            if (instruction.at(i) != tt_expected.at(i)) {
                std::cerr << "wrong byte at pos " << i << ". want=" << tt_expected.at(i) << ", got="
                          << instruction.at(i) << std::endl;
            }
            REQUIRE(instruction.at(i) == tt_expected.at(i));
        }
    }
}

TEST_CASE("Test Instructions String") {
    std::vector<Instructions> instructions = std::vector<Instructions>{
        make(OpType::OpAdd),
        make(OpType::OpConstant, std::vector<int>{2}),
        make(OpType::OpConstant, std::vector<int>{65535}),
    };

    std::string expected = R"(0000 OpAdd
0001 OpConstant 2
0004 OpConstant 65535
)";

    auto concatted = Instructions{};
    for (const auto& ins: instructions) {
        concatted.insert(concatted.end(), ins.begin(), ins.end());
    }

    std::ostringstream out;
    out << concatted;

    if (out.str() != expected) {
        std::cerr << "instructions wrongly formatted.\nwant=" << expected << "\ngot=" << concatted << std::endl;
    }
    REQUIRE(out.str() == expected);
}

TEST_CASE("Test Read Operands") {
    std::vector<std::tuple<OpType, std::vector<int>, int>> tests = {
            std::make_tuple(OpType::OpConstant, std::vector<int>{65535}, 2),
    };

    for (const auto &tt: tests) {
        const auto [tt_op, tt_operands, tt_bytes_read] = tt;

        auto instruction = make(tt_op, tt_operands);

        auto[def, ok] = lookup(tt_op);
        if (!ok) {
            std::cerr << "definition not found." << std::endl;
        }
        REQUIRE(ok);

        Instructions slice = Instructions(instruction.begin() + 1, instruction.end());
        auto[operands_read, n] = read_operands(def, slice);

        if (n != tt_bytes_read) {
            std::cerr << "n wrong. want=" << tt_bytes_read << ", got=" << n << std::endl;
        }
        REQUIRE(n == tt_bytes_read);

        int i = 0;
        for (const auto& want: tt_operands) {
            if (operands_read.at(i) != want) {
                std::cerr << "operand wrong. want=" << want << ", got=" << operands_read.at(i) << std::endl;
            }
            REQUIRE(operands_read.at(i) == want);
            i++;
        }
    }
}
