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

constexpr RV64UDWord PRIVILLEGE_MACHINE = 3;
constexpr RV64UDWord PRIVILLEGE_SUPERVISOR = 1;
constexpr RV64UDWord PRIVILLEGE_USER = 0;

class CSRF {
public:
    CSRF();
    ~CSRF() = default;

    std::variant<bool, RV64UDWord> write(RV64UDWord rawId,
                                         RV64UDWord value) noexcept;
    std::variant<bool, RV64UDWord> read(RV64UDWord rawId) const noexcept;
    std::variant<bool, RV64UDWord> setBits(RV64UDWord rawId,
                                           RV64UDWord mask) noexcept;
    std::variant<bool, RV64UDWord> clearBits(RV64UDWord rawId,
                                             RV64UDWord mask) noexcept;

    void addRegister(ICSR &reg);

    RV64UDWord getPrivillege() const noexcept { return privillege_; }
    void setPrivillege(RV64UDWord p) noexcept { privillege_ = p; }

private:
    static RV64UDWord registerPrivillege(ICSR::Id id) noexcept;
    static bool registerReadOnly(ICSR::Id id) noexcept;

    bool fitPrivillege(ICSR::Id id) const noexcept;

    RV64UDWord privillege_;
    std::unordered_map<ICSR::Id, std::reference_wrapper<ICSR>> registers_;

public:
    MStatus mstatus;
    MEPC mepc;
    MTVec mtvec;
};

} // namespace besm::exec
