#pragma once

#include <array>

#include "besm-666/instruction.hpp"
#include "iostream"

namespace besm {

class BasicBlock {
public:
    BasicBlock(RV64Ptr startPC) : startPC_(startPC) {}

    static constexpr size_t capacity = 32;

    /**
     * \brief Puts another instruction in the base block until it is completed.
     * Basic block is completed if either the \p capacity is exceeded or a jump
     * instruction is appeared
     *
     * \param [in] instr instruction
     * \returns \p true if instruction was put successfully and it wasn't last
     * instruction in basic block
     * \returns \p false if last instruction in basic block was put or if basic
     * block is completed and no more instructions can be put
     */
    bool put(Instruction instr) {
        instrs_[sz_++] = instr;
        if (sz_ >= capacity || instr.isJump()) {
            return false;
        }
        return true;
    }
    constexpr size_t size() const { return sz_; }
    constexpr RV64Ptr startPC() const { return startPC_; }

    using It = std::array<Instruction, capacity>::iterator;
    using ConstIt = std::array<Instruction, capacity>::const_iterator;
    It begin() { return instrs_.begin(); }
    It end() { return instrs_.begin() + sz_; }
    ConstIt begin() const { return instrs_.cbegin(); }
    ConstIt end() const { return instrs_.cbegin() + sz_; }

    constexpr const Instruction &operator[](int i) const { return instrs_[i]; }
    constexpr Instruction &operator[](int i) { return instrs_[i]; }

private:
    RV64Ptr startPC_;
    std::array<Instruction, capacity> instrs_;
    size_t sz_ = 0;
};

} // namespace besm
