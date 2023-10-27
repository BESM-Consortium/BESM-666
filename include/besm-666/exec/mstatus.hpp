#pragma once

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
    virtual ~MStatus() = default;
};

} // namespace besm::exec
