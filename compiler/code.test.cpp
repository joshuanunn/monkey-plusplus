// Using Catch2 unit testing library

// Let Catch2 define a main function
#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"

#include "code.hpp"

TEST_CASE("Test Make") {
    std::vector<std::tuple<OpType, std::vector<int>, std::vector<Opcode>>> tests = {
            std::make_tuple(OpType::OpConstant, std::vector<int>{65534},
                            std::vector<Opcode>{as_opcode(OpType::OpConstant), 255, 254}),
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
