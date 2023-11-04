#include <cassert>

#include "besm-666/sim/hooks.hpp"

namespace besm::sim {

void HookManager::registerBBFetchHook(BBFetchCallback callback) {
    this->doRegisterHook(BB_FETCHED, reinterpret_cast<Callback>(callback));
}
void HookManager::registerInstrExecHook(InstrExecutedCallback callback) {
    this->doRegisterHook(INSTR_EXECUTED, reinterpret_cast<Callback>(callback));
}

void HookManager::triggerBBFetchHook(BasicBlock const &bb) const {
    this->doTriggerHooks(BB_FETCHED, &bb);
}
void HookManager::triggerInstrExecHook(Instruction const &instr) const {
    this->doTriggerHooks(INSTR_EXECUTED, &instr);
}

void HookManager::doRegisterHook(HookManager::Event event, Callback callback) {
    assert(callback != nullptr);
    hooks_.insert(std::make_pair(event, callback));
}

void HookManager::doTriggerHooks(HookManager::Event event,
                                 void const *extraArg) const {
    for (auto [itr, end] = hooks_.equal_range(event); itr != end; ++itr) {
        switch (event) {
        case BB_FETCHED:
            reinterpret_cast<BBFetchCallback>(itr->second)(
                *reinterpret_cast<BasicBlock const *>(extraArg));
            break;

        case INSTR_EXECUTED:
            reinterpret_cast<InstrExecutedCallback>(itr->second)(
                *reinterpret_cast<Instruction const *>(extraArg));
            break;
        }
    }
}

HookManager::SPtr HookManager::Create() {
    return HookManager::SPtr(new HookManager());
}

} // namespace besm::sim
