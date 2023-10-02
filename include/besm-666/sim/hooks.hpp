#pragma once

#include <map>
#include <memory>

#include "besm-666/sim/hart.hpp"

namespace besm::sim {

class HookManager {
public:
    enum Event { INSTRUCTION_FETCH, INSTRUCTION_DECODE, INSTRUCTION_EXECUTE };

    using SPtr = std::shared_ptr<HookManager>;
    using Callback = void (*)(sim::Hart const &hart, void const *extraArg);

    void registerHook(Event event, Callback callback);
    void triggerHooks(Event event, Hart const &hart,
                      void const *extraArg) const;

    static HookManager::SPtr Create();

private:
    HookManager() = default;
    void doTriggerHooks(HookManager::Event event, Hart const &hart,
                        void const *extraArg) const;

    std::multimap<Event, Callback> hooks_;
};

inline void HookManager::triggerHooks(HookManager::Event event,
                                      Hart const &hart,
                                      void const *extraArg) const {
#ifdef BESM666_HOOKS_ENABLED
    this->doTriggerHooks(event, hart, extraArg);
#else
    (void)event;
    (void)hart;
    (void)extraArg;
#endif
}

} // namespace besm::sim
