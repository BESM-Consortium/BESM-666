#pragma once

namespace besm {

enum InstructionOp {
    INV_OP,
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
    CSRRW,  // 1110011 , 001     , I
    CSRRS,  // 1110011 , 010     , I
    CSRRC,  // 1110011 , 011     , I
    CSRRWI, // 1110011 , 101     , I
    CSRRSI, // 1110011 , 110     , I
    CSRRCI, // 1110011 , 111     , I
    SRET,
    MRET
};

}
