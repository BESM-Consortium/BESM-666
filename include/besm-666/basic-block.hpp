#pragma once

#include <array>

#include "instruction.hpp"

namespace besm {

class BasicBlock {
public:
    static constexpr size_t capacity = 32;

    bool put(Instruction instr) {
        if (sz_ >= capacity || instrs_[sz_ - 1].isJump())
            return false;
        instrs_[sz_++];
        return true;
    }
    constexpr size_t size() const { return sz_; }

    using It = std::array<Instruction, 32>::iterator;
    using ConstIt = std::array<Instruction, 32>::const_iterator;
    It begin() { return instrs_.begin(); }
    It end() { return instrs_.begin() + sz_; }
    ConstIt begin() const { return instrs_.cbegin(); }
    ConstIt end() const { return instrs_.cbegin() + sz_; }

    constexpr const Instruction& operator[](int i) const { return instrs_[i]; }
    constexpr Instruction& operator[](int i) { return instrs_[i]; }
private:
    std::array<Instruction, capacity> instrs_;
    size_t sz_ = 0;
};

}
