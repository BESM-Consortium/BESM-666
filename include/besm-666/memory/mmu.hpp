#pragma once

#include <memory>
#include <stdexcept>

#include "besm-666/memory/phys-mem.hpp"
#include "besm-666/riscv-types.hpp"

namespace besm::mem {

class MMU : public INonCopyable {
public:
    using SPtr = std::shared_ptr<MMU>;

    static MMU::SPtr Create(PhysMem::SPtr pMem);

    RV64UChar loadByte(RV64Ptr address) const;
    RV64UHWord loadHWord(RV64Ptr address) const;
    RV64UWord loadWord(RV64Ptr address) const;
    RV64UDWord loadDWord(RV64Ptr address) const;

    void storeByte(RV64Ptr address, RV64UChar value);
    void storeHWord(RV64Ptr address, RV64HWord value);
    void storeWord(RV64Ptr address, RV64Word value);
    void storeDWord(RV64Ptr address, RV64DWord value);

private:
    explicit MMU(PhysMem::SPtr pMem) : pMem_(pMem) {}

    RV64Ptr translateAddress(RV64Ptr address) const;

    PhysMem::SPtr pMem_;
};

} // namespace besm::mem
