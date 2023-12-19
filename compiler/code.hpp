#ifndef MONKEY_PLUSPLUS_CODE_HPP
#define MONKEY_PLUSPLUS_CODE_HPP

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

typedef std::vector<uint8_t> Instructions;

std::ostream& operator<<(std::ostream& out, const Instructions& ins);

typedef uint8_t Opcode;

enum class OpType : Opcode {
    OpConstant,
    OpAdd,
    OpSub,
    OpMul,
    OpDiv,
    OpPop,
    OpTrue,
    OpFalse,
    OpEqual,
    OpNotEqual,
    OpGreaterThan
};

struct Definition {
    Definition() = default;

    Definition(const Definition& other) = default;

    Definition(Definition&& other) noexcept = default;

    Definition& operator=(const Definition& other) = default;

    Definition& operator=(Definition&& other) noexcept = default;

    std::string name;

    std::vector<int> operand_widths;
};

extern std::map<OpType, std::shared_ptr<Definition>> definitions;

std::tuple<std::shared_ptr<Definition>, bool> lookup(const OpType& op);

// Convert an OpType to the underlying Opcode type
// Reproduced from: https://stackoverflow.com/questions/11421432/how-can-i-output-the-value-of-an-enum-class-in-c11
template<typename Enumeration>
constexpr std::enable_if_t<std::is_enum<Enumeration>::value,
        std::underlying_type_t<Enumeration>> as_opcode(const Enumeration value) {
    return static_cast<std::underlying_type_t<Enumeration>>(value);
}

std::string fmt_instruction(std::shared_ptr<Definition> def, std::vector<int> operands);

Instructions make(OpType op, std::vector<int> operands);

uint16_t read_uint_16(uint8_t hi, uint8_t lo);

std::tuple<std::vector<int>, int> read_operands(std::shared_ptr<Definition> def, Instructions ins);

#endif //MONKEY_PLUSPLUS_CODE_HPP
