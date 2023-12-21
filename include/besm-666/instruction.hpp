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

    /**
     * @todo #39:90min We can place all jump instructions consequently in
     *    InstructionOp enum and check the condition of kind 'does the current
     *    instruction fit in jump operations range'.
     */
    bool isJump() const {
        return (operation == JAL || operation == JALR || operation == BEQ ||
                operation == BNE || operation == BLT || operation == BGE ||
                operation == BLTU || operation == BGEU || operation == MRET ||
                operation == SRET || operation == ECALL);
    }

    bool isLoad() const {
        return (operation == LB || operation == LH || operation == LW ||
                operation == LBU || operation == LHU);
    }

    bool isStore() const {
        return (operation == SB || operation == SH || operation == SW);
    }

    static constexpr RV64UDWord IMMIDIATE_POISON = UINT32_MAX;
};

} // namespace besm
