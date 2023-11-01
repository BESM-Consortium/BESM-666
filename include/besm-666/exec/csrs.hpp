#pragma once

#include <iostream>

#include "besm-666/exec/csr.hpp"
#include "besm-666/exec/icsr.hpp"
#include "besm-666/riscv-types.hpp"

namespace besm::exec {

class MStatusDefs {
public:
    using SIE = CSRUncheckedField<MSTATUS_SIE>;
    using MIE = CSRUncheckedField<MSTATUS_MIE>;
    using SPIE = CSRUncheckedField<MSTATUS_SPIE>;
    using MPIE = CSRUncheckedField<MSTATUS_MPIE>;
    using SPP = CSRUncheckedField<MSTATUS_SPP>;
    using MPP = CSRUncheckedField<MSTATUS_MPP>;
};

class MStatus final : public CSRStructure<MStatusDefs::SIE, MStatusDefs::MIE,
                                          MStatusDefs::SPIE, MStatusDefs::MPIE,
                                          MStatusDefs::SPP, MStatusDefs::MPP>,
                      public MStatusDefs {
public:
    MStatus(CSRF &csrf) : CSRStructure(csrf, ICSR::MSTATUS) {}
    ~MStatus() = default;
};

class MEPCDefs {
public:
    static bool ValueValidator(RV64UDWord value) {
        return value % (IALIGN / 8) == 0;
    }

    using Value = CSRWARLField<~static_cast<RV64UDWord>(0), ValueValidator>;
};

class MEPC final : public CSRStructure<MEPCDefs::Value>, public MEPCDefs {
public:
    MEPC(CSRF &csrf) : CSRStructure(csrf, ICSR::MEPC) {}
};

class MCauseDefs {
public:
    using Interrupt =
        CSRUncheckedField<static_cast<RV64UDWord>(1) << (MXLEN - 1)>;
    using ExceptionCode =
        CSRUncheckedField<(static_cast<RV64UDWord>(1) << (MXLEN - 1)) - 1>;
};

class MCause
    : public CSRStructure<MCauseDefs::Interrupt, MCauseDefs::ExceptionCode>,
      public MCauseDefs {
public:
    MCause(CSRF &csrf) : CSRStructure(csrf, ICSR::MCAUSE) {}
};

class MTVecDefs {
public:
    static constexpr RV64UDWord DirectMode = 0;
    static constexpr RV64UDWord VectoredMode = 1;

    static bool ModeValidator(RV64UDWord mode) { return mode <= 1; }
    static bool BaseValidator(RV64UDWord base) { return base % 4 == 0; }

    using Mode = CSRWARLField<0b11, ModeValidator>;
    using Base = CSRWARLField<~static_cast<RV64UDWord>(0b11), BaseValidator>;
};

class MTVec : public CSRStructure<MTVecDefs::Mode, MTVecDefs::Base>,
              public MTVecDefs {
public:
    MTVec(CSRF &csrf) : CSRStructure(csrf, ICSR::MTVEC) {}
};

class MHartIDDefs {
public:
    using Value = CSRUncheckedField<0b11>;
};

class MHartID : public CSRStructure<MHartIDDefs::Value>, public MHartIDDefs {
public:
    MHartID(CSRF &csrf) : CSRStructure(csrf, ICSR::MHARTID) {}
};

} // namespace besm::exec
