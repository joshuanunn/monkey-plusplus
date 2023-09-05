#include <numeric>
#include "code.hpp"

// Initialise global Definitions
auto OP_CONSTANT = std::make_shared<Definition>(Definition{"OpConstant", std::vector<int>{2}});

std::map<OpType, std::shared_ptr<Definition>> definitions = {
        {OpType::OpConstant, OP_CONSTANT}
};

std::tuple<std::shared_ptr<Definition>, bool> lookup(const OpType& op) {
    auto contains = definitions.find(op);

    // If ident is not a defined keyword, then assumed to be a user identifier
    if (contains == definitions.end()) {
        return std::make_tuple(nullptr, false);
    }

    return std::make_tuple(definitions[op], true);
}

Instructions make(OpType op, std::vector<int> operands) {
    auto [def, ok] = lookup(op);
    if (!ok) {
        return std::vector<uint8_t>{};
    }

    int instruction_len = std::accumulate(def->operand_widths.begin(), def->operand_widths.end(), 1);

    std::vector<uint8_t> instruction(instruction_len);
    instruction[0] = as_opcode(op);

    int offset = 1;

    for (int i = 0; i < operands.size(); i++) {
        auto o = operands.at(i);
        auto width = def->operand_widths.at(i);

        switch (width) {
            case 2:
                // Store 16-bit instruction as 2 bytes (big endian byte order)
                instruction[offset] = static_cast<uint16_t>(o) >> 8;
                instruction[offset + 1] = static_cast<uint16_t>(o) & 0xFF;
                break;
        }
        offset += width;
    }

    return instruction;
}
