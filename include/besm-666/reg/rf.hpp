#pragma once

#include <array>
#include <cassert>
#include <cstddef>

#include "besm-666/instruction.hpp"
#include "besm-666/riscv-types.hpp"

namespace besm::reg {

/// General purpose register file
class GPRF {
public:
    // x0 - x31 + PC
    static constexpr size_t Size = 33;
    static constexpr Register PC = 32;

    inline void write(Register regId, RV64DWord value);
    inline RV64DWord read(Register regId) const;

private:
    std::array<RV64DWord, GPRF::Size> registers_;
};

inline void GPRF::write(Register regId, RV64DWord value) {
    assert(regId < GPRF::Size);
    registers_[regId] = value;
}
inline RV64DWord GPRF::read(Register regId) const {
    assert(regId < GPRF::Size);
    return registers_[regId];
}

} // namespace besm::reg
