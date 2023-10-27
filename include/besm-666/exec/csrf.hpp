#pragma once

#include <map>
#include <memory>
#include <unordered_map>
#include <utility>

#include "besm-666/exec/icsr.hpp"
#include "besm-666/riscv-types.hpp"
#include "besm-666/util/bit-magic.hpp"

#include "besm-666/exec/csrs.hpp"

namespace besm::exec {

class CSRF {
public:
    CSRF();
    ~CSRF() = default;

    std::pair<bool, RV64UDWord> write(RV64UDWord id, RV64UDWord value) noexcept;
    std::pair<bool, RV64UDWord> read(RV64UDWord id) const noexcept;
    std::pair<bool, RV64UDWord> swap(RV64UDWord id, RV64UDWord value) noexcept;
    std::pair<bool, RV64UDWord> setBits(RV64UDWord id,
                                        RV64UDWord mask) noexcept;
    std::pair<bool, RV64UDWord> clearBits(RV64UDWord id,
                                          RV64UDWord mask) noexcept;

    MStatus mstatus;

private:
    void addRegister(ICSR &reg);

    std::map<ICSR::Id, std::reference_wrapper<ICSR>> registers_;
};
} // namespace besm::exec
