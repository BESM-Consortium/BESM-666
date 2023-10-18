#pragma once

#include "besm-666/riscv-types.hpp"

namespace besm {

constexpr const RV64UDWord VENDOR_ID = 0;
constexpr const RV64UDWord ARCH_ID = 1;
constexpr const RV64UDWord IMP_ID = 1;

enum XLEN : RV64UWord { XLEN32 = 1, XLEN64 = 2, XLEN128 = 3 };

enum class Exception {
    NO_EXCEPTION,
    ILLEGAL_INSTRUCTION,
};

} // namespace besm
