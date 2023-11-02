#pragma once

#include <memory>

#include "besm-666/decoder/decoder.hpp"
#include "besm-666/exec/executor.hpp"
#include "besm-666/memory/mmu.hpp"
#include "besm-666/memory/phys-mem.hpp"

namespace besm::sim {

class HookManager;

class Hart : public INonCopyable {
public:
    using SPtr = std::shared_ptr<Hart>;
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

    mem::MMU::SPtr mmu_;
    dec::Decoder dec_;
    exec::Executor exec_;
    std::shared_ptr<sim::HookManager> hookManager_;

    RV64UDWord prevPC_;
    size_t instrsExecuted_;
};

} // namespace besm::sim
