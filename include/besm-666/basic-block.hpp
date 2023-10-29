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
     * \param [in] instr
     * \returns \p true if instruction was put successfully
     * \returns \p false if basic block is completed and instruction can't be
     * put
     */
    bool put(Instruction instr) {
        if (sz_ >= capacity || instrs_[sz_ - 1].isJump()) {
            return false;
        }
        instrs_[sz_++] = instr;
        return true;
    }
    constexpr size_t size() const { return sz_; }
    constexpr RV64Ptr startPC() { return startPC_; }

    using It = std::array<Instruction, 32>::iterator;
    using ConstIt = std::array<Instruction, 32>::const_iterator;
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
