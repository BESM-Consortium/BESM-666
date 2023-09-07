#ifndef BESM666_RISCV_TYPES_H
#define BESM666_RISCV_TYPES_H

#include <cstdint>

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

} // namespace besm

#endif /* BESM666_RISCV_TYPES_H */
