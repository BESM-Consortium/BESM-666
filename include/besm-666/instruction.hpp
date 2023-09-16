#pragma once

#include "besm-666/riscv-types.hpp"

namespace besm {

using Register = uint8_t;
using Opcode = uint8_t;
using Immidiate = uint32_t;

enum class OperandType { UNUSED, REGISTER, IMMIDIATE };

struct Instruction {
    const Register rd;
    const Register rs1;
    const Register rs2;
    const Immidiate immidiate;

    /*
     * @todo #10:90m Optimize opcode and funct to one entity.
     *  Maybe it is better to create enum describing all opcode and
     *  funct combinations.
     */
    const uint8_t opcode;
    const uint8_t funct;
};

} // namespace besm
