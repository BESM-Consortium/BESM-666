#pragma once
#include "besm-666/autogen/operations-matrix.hpp"
#include "besm-666/instruction.hpp"
#include <cassert>
#include <cstdlib>
#include <stdexcept>

namespace besm::dec {

class Decoder {

private:
    static constexpr RV64UWord OPCODE_MASK = 0b01111111;
    static constexpr int FUNC3_shift = 12;
    static constexpr RV64UWord FUNC3_MASK = 0b111 << FUNC3_shift;
    static constexpr int RD_SHIFT = 7;
    static constexpr RV64UWord RD_MASK = 0b11111 << RD_SHIFT;
    static constexpr int RS1_SHIFT = 15;
    static constexpr RV64UWord RS1_MASK = 0b11111 << RS1_SHIFT;
    static constexpr int RS2_SHIFT = 20;
    static constexpr RV64UWord RS2_MASK = 0b11111 << RS2_SHIFT;

public:
    /**
     * Give the {@link Instruction} by the bytecode word.
     * @param bytecode word.
     * @return {@link Instruction} struct.
     */
    [[nodiscard]] besm::Instruction parse(RV64UWord bytecode) const;

private:
    static inline Instruction parse_R(RV64UWord bytecode, Opcode opcode,
                                      uint8_t func3);

    static inline Instruction parse_I(RV64UWord bytecode,
                                      InstructionOp operation, Opcode opcode);

    static inline Instruction parse_S(RV64UWord bytecode,
                                      InstructionOp operation);

    static inline Instruction parse_B(RV64UWord bytecode,
                                      InstructionOp operation);

    static inline Instruction parse_U(RV64UWord bytecode,
                                      InstructionOp operation);

    static inline Instruction parse_J(RV64UWord bytecode,
                                      InstructionOp operation);
};

} // namespace besm::dec
