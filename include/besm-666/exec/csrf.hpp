#pragma once

#include <functional>
#include <map>
#include <memory>
#include <unordered_map>
#include <variant>

#include "besm-666/exec/icsr.hpp"
#include "besm-666/riscv-types.hpp"
#include "besm-666/util/bit-magic.hpp"

#include "besm-666/exec/csrs.hpp"

namespace besm::exec {

enum PrivillegeLevel {
    PRIVILLEGE_MACHINE = 0,
    PRIVILLEGE_HYPERVISOR = 1,
    PRIVILLEGE_SUPERVISOR = 2,
    PRIVILLEGE_USER = 3
};

class CSRF {
public:
    CSRF();
    ~CSRF() = default;

    std::variant<bool, RV64UDWord> write(RV64UDWord rawId, RV64UDWord value) noexcept;
    std::variant<bool, RV64UDWord> read(RV64UDWord rawId) const noexcept;
    std::variant<bool, RV64UDWord> setBits(RV64UDWord rawId,
                                        RV64UDWord mask) noexcept;
    std::variant<bool, RV64UDWord> clearBits(RV64UDWord rawId,
                                          RV64UDWord mask) noexcept;

    MStatus mstatus;
    MEPC mepc;

    void addRegister(ICSR &reg);

    PrivillegeLevel getPrivillege() const noexcept;

private:
    static PrivillegeLevel registerPrivillege(ICSR::Id id) noexcept;
    static bool registerReadOnly(ICSR::Id id) noexcept;

    bool fitPrivillege(ICSR::Id id) const noexcept;

    PrivillegeLevel privillege_;
    std::unordered_map<ICSR::Id, std::reference_wrapper<ICSR>> registers_;
};
} // namespace besm::exec
