#pragma once

#include <cstdint>

#include "besm-666/autogen/encoding.out.h"

namespace besm {

using RV64Char = int8_t;
using RV64UChar = uint8_t;
using RV64Short = int16_t;
using RV64UShort = uint16_t;
using RV64Int = int32_t;
using RV64UInt = uint32_t;
using RV64Long = int32_t;
using RV64ULong = uint64_t;
using RV64LongLong = int64_t;
using RV64ULongLong = uint64_t;

using RV64Ptr = uint64_t;
using RV64Size = uint64_t;
using RV64PtrDiff = int64_t;

using RV64Word = int32_t;
using RV64HWord = int16_t;
using RV64DWord = int64_t;

using RV64UWord = uint32_t;
using RV64UHWord = uint16_t;
using RV64UDWord = uint64_t;

constexpr RV64UDWord XLEN = 64;
constexpr RV64UDWord IALIGN = 32;
constexpr RV64UDWord MXLEN = 64;

enum ExceptionId {
    EXCEPTION_INSTR_ADDR_MISALIGNED = 0,
    EXCEPTION_ACCESS_FAULT = 1,
    EXCEPTION_ILLEGAL_INSTR = 2,
    EXCEPTION_BREAKPOINT = 3,
    EXCEPTION_LOAD_ADDR_MISALIGNED = 4,
    EXCEPTION_LOAD_ACCESS_FAULT = 5,
    EXCEPTION_STORE_ADDR_MISALIGNED = 6,
    EXCEPTION_STORE_ACCESS_FAULT = 7,
    EXCEPTION_ECALL_UMODE = 8,
    EXCEPTION_ECALL_SMODE = 9,
    EXCEPTION_ECALL_MMODE = 11,
    EXCEPTION_INSTR_PAGEFAULT = 12,
    EXCEPTION_LOAD_PAGEFAULT = 13,
    EXCEPTION_STORE_PAGEFAULT = 15,
};

} // namespace besm
