#ifndef MONKEY_PLUSPLUS_CODE_HPP
#define MONKEY_PLUSPLUS_CODE_HPP

#include <cstdint>
#include <exception>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
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
    OpGreaterThan,
    OpMinus,
    OpBang,
    OpJumpNotTruthy,
    OpJump,
    OpNull,
    OpSetGlobal,
    OpGetGlobal,
    OpArray,
    OpHash,
    OpIndex,
    OpCall,
    OpReturnValue,
    OpReturn,
    OpSetLocal,
    OpGetLocal,
    OpGetBuiltin
};

struct Definition {
    Definition() = delete;

    Definition(std::string name);

    Definition(const Definition& other) = default;

    Definition(Definition&& other) noexcept = default;

    Definition& operator=(const Definition& other) = default;

    Definition& operator=(Definition&& other) noexcept = default;

    std::string name;

    std::vector<int> operand_widths;
};

extern std::map<OpType, std::shared_ptr<Definition>> definitions;

std::shared_ptr<Definition> new_definition(std::string name);

std::shared_ptr<Definition> new_definition(std::string name, int operand_width);

std::shared_ptr<Definition> new_definition(std::string name, int first_operand_width, int second_operand_width);

std::tuple<std::shared_ptr<Definition>, bool> lookup(const OpType& op);

// Convert an OpType to the underlying Opcode type
// Reproduced from: https://stackoverflow.com/questions/11421432/how-can-i-output-the-value-of-an-enum-class-in-c11
template<typename Enumeration>
constexpr std::enable_if_t<std::is_enum<Enumeration>::value,
        std::underlying_type_t<Enumeration>> as_opcode(const Enumeration value) {
    return static_cast<std::underlying_type_t<Enumeration>>(value);
}

std::string fmt_instruction(std::shared_ptr<Definition> def, std::vector<int> operands);

Instructions make(OpType op);

Instructions make(OpType op, int operand);

int read_uint_8(Instructions ins, int offset);

int read_uint_16(Instructions ins, int offset);

std::tuple<std::vector<int>, int> read_operands(std::shared_ptr<Definition> def, Instructions ins);

#endif //MONKEY_PLUSPLUS_CODE_HPP
