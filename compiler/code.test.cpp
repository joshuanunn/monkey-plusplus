// Using Catch2 unit testing library

// Let Catch2 define a main function
#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"

#include "code.hpp"

TEST_CASE("Test Make With Operand") {
    auto instruction = make(OpType::OpConstant, 65534);
    auto tt_expected = std::vector<Opcode>{as_opcode(OpType::OpConstant), 255, 254};

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

TEST_CASE("Test Make No Operands") {
    auto instruction = make(OpType::OpAdd);
    auto tt_expected = std::vector<Opcode>{as_opcode(OpType::OpAdd)};

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

TEST_CASE("Test Instructions String") {
    std::vector<Instructions> instructions = std::vector<Instructions>{
        make(OpType::OpAdd),
        make(OpType::OpConstant, 2),
        make(OpType::OpConstant, 65535),
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
    std::vector<std::tuple<OpType, int, int>> tests = {
            std::make_tuple(OpType::OpConstant, 65535, 2),
    };

    for (const auto &tt: tests) {
        const auto [tt_op, tt_operand, tt_bytes_read] = tt;

        auto instruction = make(tt_op, tt_operand);

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

        if (operands_read.at(0) != tt_operand) {
            std::cerr << "operand wrong. want=" << tt_operand << ", got=" << operands_read.at(0) << std::endl;
        }
        REQUIRE(operands_read.at(0) == tt_operand);
    }
}
