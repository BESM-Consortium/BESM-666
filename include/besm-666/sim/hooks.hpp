#pragma once

#include <map>
#include <memory>

#include "besm-666/exec/basic-block.hpp"
#include "besm-666/instruction.hpp"

namespace besm::sim {

class HookManager {
public:
    // Be careful, the life time of trigger[---]Hook reference values is
    // limited by the callback scope
    using BBFetchCallback = void (*)(exec::BasicBlock const &bb);
    using InstrExecutedCallback = void (*)(Instruction const &instr);

    using SPtr = std::shared_ptr<HookManager>;

    void registerBBFetchHook(BBFetchCallback callback);
    void registerInstrExecHook(InstrExecutedCallback callback);

    void triggerBBFetchHook(exec::BasicBlock const &bb) const;
    void triggerInstrExecHook(Instruction const &instr) const;

    static HookManager::SPtr Create();

private:
    enum Event {
        BB_FETCHED,
        INSTR_EXECUTED,
    };

    using Callback = void (*)(void const *extraArg);

    HookManager() = default;

    void doRegisterHook(Event event, Callback callback);
    void doTriggerHooks(HookManager::Event event, void const *extraArg) const;

    std::multimap<Event, Callback> hooks_;
};

} // namespace besm::sim
