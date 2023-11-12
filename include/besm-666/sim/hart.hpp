#pragma once

#include "besm-666/basic-block.hpp"
#include "besm-666/decoder/decoder.hpp"
#include "besm-666/exec/executor.hpp"
#include "besm-666/memory/mmu.hpp"
#include "besm-666/memory/phys-mem.hpp"
#include "besm-666/util/assotiative-cache.hpp"

#include <memory>

namespace besm::sim {

template <typename PayloadType, typename TagType>
TagType BBTag(PayloadType const &payload) {
    return payload.startPC();
}

template <typename TagType, typename HashType>
HashType BBHash(TagType const &tag) {
    // Can be used: tag, std::hash<TagType>{}(tag), (tag & 0x7F8) >> 3.
    // (tag & 0x7F8) >> 3 repeats hash every 128 byte aka num of set.

    return tag; // straight mapping
}

class HookManager;

class Hart : public INonCopyable {
public:
    using SPtr = std::shared_ptr<Hart>;
    using BBCache =
        besm::util::Cache<BasicBlock, RV64UDWord, RV64UDWord, BBTag, BBHash>;
    static SPtr Create(std::shared_ptr<mem::PhysMem> const &pMem,
                       std::shared_ptr<HookManager> const &hookManager);

    exec::GPRF const &getGPRF() const { return exec_.getState(); }
    exec::CSRF const &getCSRF() const { return exec_.getCSRF(); }
    mem::MMU const &getMMU() const { return *mmu_; }
    size_t getInstrsExecuted() const { return instrsExecuted_; }

    void runCycle();
    bool finished() const;

    void run();

private:
    explicit Hart(std::shared_ptr<mem::PhysMem> const &pMem,
                  std::shared_ptr<HookManager> hookManager);

    BBCache cache_;
    mem::MMU::SPtr mmu_;
    dec::Decoder dec_;
    exec::Executor exec_;
    std::shared_ptr<sim::HookManager> hookManager_;

    RV64UDWord prevPC_;
    size_t instrsExecuted_;
};

} // namespace besm::sim
