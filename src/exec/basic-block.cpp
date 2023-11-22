#include "besm-666/exec/basic-block.hpp"
#include "besm-666/decoder/decoder.hpp"
#include <limits>
#include <iostream>

namespace besm::exec {

BasicBlock::BasicBlock() {
    pc_ = 1; // POISONED VALUE, CAN'T BE MET ON RUNTIME
    instrs_[0].operation = INV_OP;
}

BasicBlockCache::BasicBlockCache() {
    for(auto& lruRower : lruRowers_) {
        lruRower = 0;
    }
}

std::pair<bool, BasicBlock &> BasicBlockCache::lookup(RV64Ptr pc) {
    // Fix me: 2 bit offset because of insn alignment, 3 bits because
    // of bb capacity
    RV64Ptr hash = (pc & (kSetMask << 5)) >> 5;
    size_t setPos = hash * kWays;

    for(size_t i = 0; i < kWays; ++i) {
        size_t index = setPos + i;

        if(bbs_[index].getPC() == pc) {
            return std::make_pair(true, std::ref(bbs_[index]));
        }
    }

    size_t index = lruRowers_[hash] + setPos;

    lruRowers_[hash] = (lruRowers_[hash] + 1) & kWayMask;

    return std::make_pair(false, std::ref(bbs_[index]));
}

BasicBlockRebuilder::BasicBlockRebuilder(BasicBlock &targetBB, size_t pc) :
        bb_(targetBB), count_(0) {
    bb_.pc_ = pc;
}

bool BasicBlockRebuilder::append(Instruction const &instr) noexcept {
    bb_.instrs_[count_] = instr;

    ++count_;
    if(instr.isJump() || count_ == BasicBlock::kCapacity - 1) {
        bb_.instrs_[count_].operation = BB_END;
        return false;
    }

    return true;
}

} // namespace besm::exec
