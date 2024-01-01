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
    {OpType::OpCall, new_definition("OpCall", 1)},
    {OpType::OpReturnValue, new_definition("OpReturnValue")},
    {OpType::OpReturn, new_definition("OpReturn")},
    {OpType::OpSetLocal, new_definition("OpSetLocal", 1)},
    {OpType::OpGetLocal, new_definition("OpGetLocal", 1)},
    {OpType::OpGetBuiltin, new_definition("OpGetBuiltin", 1)},
    {OpType::OpClosure, new_definition("OpClosure", 2, 1)},
    {OpType::OpGetFree, new_definition("OpGetFree", 1)},
    {OpType::OpCurrentClosure, new_definition("OpCurrentClosure")},
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
        case 2:
            return def->name + " " + std::to_string(operands.at(0)) +
                             + " " + std::to_string(operands.at(1));
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
        std::cerr << "fatal error: " << def->name << " instruction not defined." << std::endl;
        throw std::exception();
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
        std::cerr << "fatal error: " << def->name << " instruction not defined." << std::endl;
        throw std::exception();
    }

    int instruction_len = std::accumulate(def->operand_widths.begin(), def->operand_widths.end(), 1);

    std::vector<uint8_t> instruction(instruction_len);
    instruction[0] = as_opcode(op);

    int offset = 1;

    auto width = def->operand_widths.at(0);

    switch (width) {
        // Store 16-bit instruction as 2 bytes (big endian byte order)
        case 2:
            instruction[offset] = static_cast<uint16_t>(operand) >> 8;
            instruction[offset + 1] = static_cast<uint16_t>(operand) & 0xFF;
            break;
        // Store 8-bit instruction directly in a single byte
        case 1:
            instruction[offset] = static_cast<uint8_t>(operand);
            break;
    }

    return instruction;
}

Instructions make(OpType op, int first_operand, int second_operand) {
    auto [def, ok] = lookup(op);
    if (!ok) {
        std::cerr << "fatal error: " << def->name << " instruction not defined." << std::endl;
        throw std::exception();
    }

    if (def->operand_widths.size() != 2) {
        std::cerr << "fatal error: unexpectedly received 2 operands for an " << def->name << " instruction." << std::endl;
        throw std::exception();
    }

    int instruction_len = std::accumulate(def->operand_widths.begin(), def->operand_widths.end(), 1);

    std::vector<uint8_t> instruction(instruction_len);
    instruction[0] = as_opcode(op);

    int offset = 1;
    int width;

    // Process first operand
    width = def->operand_widths.at(0);

    switch (width) {
        // Store 16-bit instruction as 2 bytes (big endian byte order)
        case 2:
            instruction[offset] = static_cast<uint16_t>(first_operand) >> 8;
            instruction[offset + 1] = static_cast<uint16_t>(first_operand) & 0xFF;
            offset += 2;
            break;
        // Store 8-bit instruction directly in a single byte
        case 1:
            instruction[offset] = static_cast<uint8_t>(first_operand);
            offset += 1;
            break;
    }

    // Process second operand
    width = def->operand_widths.at(1);

    switch (width) {
        // Store 16-bit instruction as 2 bytes (big endian byte order)
        case 2:
            instruction[offset] = static_cast<uint16_t>(second_operand) >> 8;
            instruction[offset + 1] = static_cast<uint16_t>(second_operand) & 0xFF;
            offset += 2;
            break;
        // Store 8-bit instruction directly in a single byte
        case 1:
            instruction[offset] = static_cast<uint8_t>(second_operand);
            offset += 1;
            break;
    }

    return instruction;
}

int read_uint_8(Instructions ins, int offset) {
    return static_cast<int>(ins[offset]);
}

int read_uint_16(Instructions ins, int offset) {
    // Interpret 2 sequential bytes (big endian) as a 16-bit int
    auto hi = static_cast<uint16_t>(ins[offset]) << 8;
    auto lo = static_cast<uint16_t>(ins[offset+1]);
    return static_cast<int>(lo | hi);
}

std::tuple<std::vector<int>, int> read_operands(std::shared_ptr<Definition> def, Instructions ins) {
    // Initialise vector<int> to be of same size as def->operands_widths
    std::vector<int> operands(def->operand_widths.size());

    int offset = 0;

    for (int i = 0; i < def->operand_widths.size(); i++) {
        auto width = def->operand_widths.at(i);

        switch (width) {
            // Read 2 byte instruction (big endian byte order)
            case 2:
                operands[i] = read_uint_16(ins, offset);
                break;
            // Read 1 byte instruction
            case 1:
                operands[i] = read_uint_8(ins, offset);
                break;
        }
        offset += width;
    }

    return std::make_tuple(operands, offset);
}
