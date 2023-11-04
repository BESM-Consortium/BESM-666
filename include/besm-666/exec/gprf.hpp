#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>

#include "besm-666/instruction.hpp"
#include "besm-666/riscv-types.hpp"
#include "besm-666/util/bit-magic.hpp"

namespace besm::exec {

/// General purpose register file
class GPRF {
public:
    // x0 - x31 + PC
    static constexpr const size_t X0 = 0;
    static constexpr const size_t X1 = 1;
    static constexpr const size_t X2 = 2;
    static constexpr const size_t X3 = 3;
    static constexpr const size_t X4 = 4;
    static constexpr const size_t X5 = 5;
    static constexpr const size_t X6 = 6;
    static constexpr const size_t X7 = 7;
    static constexpr const size_t X8 = 8;
    static constexpr const size_t X9 = 9;
    static constexpr const size_t X10 = 10;
    static constexpr const size_t X11 = 11;
    static constexpr const size_t X12 = 12;
    static constexpr const size_t X13 = 13;
    static constexpr const size_t X14 = 14;
    static constexpr const size_t X15 = 15;
    static constexpr const size_t X16 = 16;
    static constexpr const size_t X17 = 17;
    static constexpr const size_t X18 = 18;
    static constexpr const size_t X19 = 19;
    static constexpr const size_t X20 = 20;
    static constexpr const size_t X21 = 21;
    static constexpr const size_t X22 = 22;
    static constexpr const size_t X23 = 23;
    static constexpr const size_t X24 = 24;
    static constexpr const size_t X25 = 25;
    static constexpr const size_t X26 = 26;
    static constexpr const size_t X27 = 27;
    static constexpr const size_t X28 = 28;
    static constexpr const size_t X29 = 29;
    static constexpr const size_t X30 = 30;
    static constexpr const size_t X31 = 31;
    static constexpr const size_t PC = 32;
    static constexpr const size_t Size = 33;

    inline GPRF() { registers_[PC] = 0; }

    inline void write(Register regId, RV64UDWord value);
    inline RV64UDWord read(Register regId) const;

private:
    std::array<RV64UDWord, GPRF::Size> registers_;
};

inline void GPRF::write(Register regId, RV64UDWord value) {
    assert(regId < Size);
    registers_[regId] = value;
}

inline RV64UDWord GPRF::read(Register regId) const {
    assert(regId < Size);
    return regId == X0 ? 0 : registers_[regId];
}

class GPRFStateDumper {
public:
    GPRFStateDumper(std::ostream &stream) : stream_(stream) {}

    void dump(GPRF const &gprf);
    std::string getRegName(Register regId) const;

private:
    constexpr static char const *REG_ALIAS[] = {
        "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0/fp",
        "s1",   "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
        "s2",   "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10",
        "s11",  "t3", "t4", "t5", "t6", "pc"};

    std::ostream &stream_;
};

} // namespace besm::exec
