#include <cassert>

#include "besm-666/sim/hooks.hpp"

namespace besm::sim {

void HookManager::registerHook(HookManager::Event event, Callback callback) {
    assert(callback != nullptr);
    hooks_.insert(std::make_pair(event, callback));
}

void HookManager::doTriggerHooks(HookManager::Event event, Hart const &hart,
                                 void const *extraArg) const {
    for (auto [itr, end] = hooks_.equal_range(event); itr != end; ++itr) {
        itr->second(hart, extraArg);
    }
}

HookManager::SPtr HookManager::Create() {
    return HookManager::SPtr(new HookManager());
}

} // namespace besm::sim
