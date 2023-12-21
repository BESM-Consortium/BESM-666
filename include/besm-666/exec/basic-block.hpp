#pragma once

#include <array>
#include <cstddef>
#include <functional>
#include <vector>

#include "besm-666/instruction.hpp"
#include "besm-666/util/math.hpp"

namespace besm::exec {

class BasicBlock {
public:
    static constexpr size_t kCapacity = 8;
    using InstrStorage = std::array<Instruction, kCapacity>;

    BasicBlock();

    RV64Ptr getPC() const noexcept { return pc_; }
    Instruction const *getInstructions() const noexcept {
        return instrs_.data();
    }

private:
    friend class BasicBlockRebuilder;

    InstrStorage instrs_;
    RV64Ptr pc_;
};

class BasicBlockCache {
public:
    static constexpr size_t kSetBits = 7;
    static constexpr size_t kWayBits = 3;

    static constexpr size_t kSets = (1ull << kSetBits);
    static constexpr size_t kWays = (1ull << kWayBits);

    static constexpr size_t kSetMask = kSets - 1;
    static constexpr size_t kWayMask = kWays - 1;

    BasicBlockCache();

    std::pair<bool, BasicBlock &> lookup(RV64Ptr pc);

private:
    std::array<BasicBlock, kSets * kWays> bbs_;
    std::array<size_t, kSets> lruRowers_;
};

class BasicBlockRebuilder {
public:
    BasicBlockRebuilder(BasicBlock &targetBB, size_t pc);

    bool append(Instruction const &instr) noexcept;

private:
    BasicBlock &bb_;
    size_t count_;
};

} // namespace besm::exec
