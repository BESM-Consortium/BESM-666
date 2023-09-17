#ifndef BESM_666_RV_INSTRUCTION_OP_HPP
#define BESM_666_RV_INSTRUCTION_OP_HPP

namespace besm {

enum InstructionOp {
    // RV32I:
    LUI,
    AUIPC,
    JAL,
    JALR,
    BEQ,
    BNE,
    BLT,
    BGE,
    BLTU,
    BGEU,
    LB,
    LH,
    LW,
    LBU,
    LHU,
    SB,
    SH,
    SW,
    ADDI,
    SLTI,
    SLTIU,
    XORI,
    ORI,
    ANDI,
    //    SLLI, has rv64i alternative.
    //    SRLI, has rv64i alternative.
    //    SRAI, has rv64i alternative.
    ADD,
    SUB,
    SLL,
    SLT,
    SLTU,
    XOR,
    SRL,
    SRA,
    OR,
    AND,
    FENCE,
    FENCE_TSO,
    PAUSE,
    ECALL,
    EBREAK,
    // RV64I:
    LWU,
    LD,
    SD,
    SLLI,
    SRLI,
    SRAI,
    ADDIW,
    SLLIW,
    SRLIW,
    SRAIW,
    ADDW,
    SUBW,
    SLLW,
    SRLW,
    SRAW,
};

}
#endif // BESM_666_RV_INSTRUCTION_OP_HPP
