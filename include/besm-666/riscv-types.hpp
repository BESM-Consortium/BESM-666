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

enum ExceptionId { EXCEPTION_ILLEGAL_INSTR = 1 };

} // namespace besm
