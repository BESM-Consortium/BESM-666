#pragma once

#include <array>

#include "besm-666/instruction.hpp"
#include "iostream"

namespace besm {

class BasicBlock {
public:
    BasicBlock() : startPC_(0), sz_(0) {}
    BasicBlock(RV64UDWord startPC) : startPC_(startPC), sz_(0) {}

    static constexpr size_t capacity = 33;

    /**
     * \brief Puts another instruction in the base block until it is completed.
     * Basic block is completed if either the \p capacity is exceeded or a jump
     * instruction is appeared. Also puts \p BB_END instruction at the end of
     * the basic block.
     *
     * \param [in] instr instruction
     * \returns \p true if instruction was put successfully and it wasn't last
     * instruction in basic block
     * \returns \p false if last instruction in basic block was put or if basic
     * block is completed and no more instructions can be put. In this case it
     * will put \p instr and \p BB_END instruction at the end of
     * the basic block anyway.
     */
    bool put(Instruction instr) {
        instrs_[sz_++] = instr;
        if (sz_ >= capacity - 1 || instr.isJump()) {
            instrs_[sz_++] = Instruction{.operation = BB_END};
            return false;
        }
        return true;
    }
    constexpr size_t size() const { return sz_; }
    constexpr RV64UDWord startPC() const { return startPC_; }
    void setStartPC(RV64UDWord startPC) { startPC_ = startPC; }
    void resetBB() {
        sz_ = 0;
        startPC_ = -1;
    }

    using It = std::array<Instruction, capacity>::iterator;
    using ConstIt = std::array<Instruction, capacity>::const_iterator;
    It begin() { return instrs_.begin(); }
    It end() { return instrs_.begin() + sz_; }
    ConstIt begin() const { return instrs_.cbegin(); }
    ConstIt end() const { return instrs_.cbegin() + sz_; }
    size_t nextInstr() const { return ++currentInstr_; }
    size_t currentInstr() const { return currentInstr_; }
    size_t currentInstrIncrement() const { return currentInstr_++; }
    void incrementInstr() const { currentInstr_++; }

    constexpr const Instruction &operator[](size_t i) const {
        return instrs_[i];
    }
    constexpr Instruction &operator[](size_t i) { return instrs_[i]; }

private:
    RV64UDWord startPC_;
    std::array<Instruction, capacity> instrs_;
    size_t sz_ = 0;
    mutable size_t currentInstr_ = 0;
};

} // namespace besm
