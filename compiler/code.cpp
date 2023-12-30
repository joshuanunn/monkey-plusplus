#include "code.hpp"

// Initialise global Definitions
std::map<OpType, std::shared_ptr<Definition>> definitions = {
    {OpType::OpConstant, new_definition("OpConstant", 2)},
    {OpType::OpAdd, new_definition("OpAdd")},
    {OpType::OpSub, new_definition("OpSub")},
    {OpType::OpMul, new_definition("OpMul")},
    {OpType::OpDiv, new_definition("OpDiv")},
    {OpType::OpPop, new_definition("OpPop")},
    {OpType::OpTrue, new_definition("OpTrue")},
    {OpType::OpFalse, new_definition("OpFalse")},
    {OpType::OpEqual, new_definition("OpEqual")},
    {OpType::OpNotEqual, new_definition("OpNotEqual")},
    {OpType::OpGreaterThan, new_definition("OpGreaterThan")},
    {OpType::OpMinus, new_definition("OpMinus")},
    {OpType::OpBang, new_definition("OpBang")},
    {OpType::OpJumpNotTruthy, new_definition("OpJumpNotTruthy", 2)},
    {OpType::OpJump, new_definition("OpJump", 2)},
    {OpType::OpNull, new_definition("OpNull")},
    {OpType::OpSetGlobal, new_definition("OpSetGlobal", 2)},
    {OpType::OpGetGlobal, new_definition("OpGetGlobal", 2)},
    {OpType::OpArray, new_definition("OpArray", 2)},
    {OpType::OpHash, new_definition("OpHash", 2)},
    {OpType::OpIndex, new_definition("OpIndex")},
    {OpType::OpCall, new_definition("OpCall")},
    {OpType::OpReturnValue, new_definition("OpReturnValue")},
    {OpType::OpReturn, new_definition("OpReturn")},
};

Definition::Definition(std::string name) : name{name}, operand_widths{} {}

std::shared_ptr<Definition> new_definition(std::string name) {
    return std::make_shared<Definition>(Definition(name));
}

std::shared_ptr<Definition> new_definition(std::string name, int operand_width) {
    auto definition = std::make_shared<Definition>(Definition{name});
    definition->operand_widths.push_back(operand_width);
    return std::move(definition);
}

std::shared_ptr<Definition> new_definition(std::string name, int first_operand_width, int second_operand_width) {
    auto definition = std::make_shared<Definition>(Definition{name});
    definition->operand_widths.push_back(first_operand_width);
    definition->operand_widths.push_back(second_operand_width);
    return std::move(definition);
}

std::tuple<std::shared_ptr<Definition>, bool> lookup(const OpType &op)
{
    auto contains = definitions.find(op);

    // If OpType is not defined, then return false to indicate "not ok"
    if (contains == definitions.end()) {
        // TODO: should we instead define and return an "error" object with message instead of boolean?
        //std::cerr << "opcode undefined." << std::endl;
        return std::make_tuple(nullptr, false);
    }

    return std::make_tuple(definitions[op], true);
}

std::string fmt_instruction(std::shared_ptr<Definition> def, std::vector<int> operands) {
    auto operand_count = def->operand_widths.size();

    if (operands.size() != operand_count) {
        return "ERROR: operand length " + std::to_string(operands.size()) + "does not match defined" + std::to_string(operand_count) + "\n";
    }

    switch (operand_count) {
        case 0:
            return def->name;
        case 1:
            return def->name + " " + std::to_string(operands.at(0));
    }

    return "ERROR: unhandled operand_count for " + def->name + "\n";
}

std::ostream& operator<<(std::ostream& out, const Instructions& ins) {
    int i = 0;

    out.fill('0');

    while (i < ins.size()) {
        auto[def, ok] = lookup(static_cast<OpType>(ins.at(i)));
        if (!ok) {
            // TODO: if lookup defined a range of error messages, then remove hard-coded message here.
            out << std::string("ERROR: opcode undefined.") << std::endl;
            continue;
        }

        Instructions slice = Instructions(ins.begin() + i + 1, ins.end());
        auto[operands, read] = read_operands(def, slice);

        // Write instruction in this form: "0003 OpConstant 2"
        out << std::setw(4) << i << " ";
        out << fmt_instruction(def, operands) << std::endl;

        i += (1 + read);
    }
    return out;
}

Instructions make(OpType op) {
    auto [def, ok] = lookup(op);
    if (!ok) {
        return std::vector<uint8_t>{};
    }

    int instruction_len = std::accumulate(def->operand_widths.begin(), def->operand_widths.end(), 1);

    // Ensure that instruction length is correct (should have no additional operands)
    if (instruction_len != 1) {
        std::cerr << "fatal error: unexpectedly received operands for an " << def->name << " instruction." << std::endl;
        throw std::exception();
    }

    std::vector<uint8_t> instruction = {as_opcode(op)};

    return instruction;
}

Instructions make(OpType op, int operand) {
    auto [def, ok] = lookup(op);
    if (!ok) {
        return std::vector<uint8_t>{};
    }

    int instruction_len = std::accumulate(def->operand_widths.begin(), def->operand_widths.end(), 1);

    std::vector<uint8_t> instruction(instruction_len);
    instruction[0] = as_opcode(op);

    int offset = 1;

    auto width = def->operand_widths.at(0);

    switch (width) {
        case 2:
        // Store 16-bit instruction as 2 bytes (big endian byte order)
        instruction[offset] = static_cast<uint16_t>(operand) >> 8;
        instruction[offset + 1] = static_cast<uint16_t>(operand) & 0xFF;
        break;
    }

    return instruction;
}

uint16_t read_uint_16(uint8_t hi, uint8_t lo) {
    return static_cast<uint16_t>(lo) | static_cast<uint16_t>(hi) << 8;
}

std::tuple<std::vector<int>, int> read_operands(std::shared_ptr<Definition> def, Instructions ins) {
    // Initialise vector<int> to be of same size as def->operands_widths
    std::vector<int> operands(def->operand_widths.size());

    int offset = 0;

    for (int i = 0; i < def->operand_widths.size(); i++) {
        auto width = def->operand_widths.at(i);

        switch (width) {
            case 2:
                // Read 2 byte instruction into a single int (big endian byte order)
                operands[i] = static_cast<int>(read_uint_16(ins[offset], ins[offset+1]));
                break;
        }
        offset += width;
    }

    return std::make_tuple(operands, offset);
}
