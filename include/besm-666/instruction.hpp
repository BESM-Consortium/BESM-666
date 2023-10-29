#pragma once

#include "besm-666/riscv-types.hpp"
#include "besm-666/rv-instruction-op.hpp"

namespace besm {

using Opcode = uint8_t;
using Register = uint8_t;
using Immidiate = RV64UDWord;

struct Instruction {
    Register rd;
    Register rs1;
    Register rs2;
    RV64UDWord immidiate;
    InstructionOp operation;

    bool isJump() const {
        return (operation == JAL || operation == JALR || operation == BEQ ||
                operation == BNE || operation == BLT || operation == BGE ||
                operation == BLTU || operation == BGEU);
    }

    static constexpr RV64UDWord IMMIDIATE_POISON = UINT32_MAX;
};

} // namespace besm
