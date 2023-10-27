#pragma once

#include <memory>
#include <stdexcept>

#include "besm-666/memory/phys-mem.hpp"
#include "besm-666/riscv-types.hpp"

namespace besm::mem {

class MMU : public INonCopyable {
public:
    using SPtr = std::shared_ptr<MMU>;

    static MMU::SPtr Create(std::shared_ptr<PhysMem> const &pMem);

    RV64UChar loadByte(RV64Ptr address) const;
    RV64UHWord loadHWord(RV64Ptr address) const;
    RV64UWord loadWord(RV64Ptr address) const;
    RV64UDWord loadDWord(RV64Ptr address) const;

    void storeByte(RV64Ptr address, RV64UChar value);
    void storeHWord(RV64Ptr address, RV64HWord value);
    void storeWord(RV64Ptr address, RV64Word value);
    void storeDWord(RV64Ptr address, RV64DWord value);

    std::pair<void *, size_t> touchHostAddress(RV64Ptr vaddress);
    std::pair<void const *, size_t> getHostAddress(RV64Ptr vaddress) const {
        return pMem_->getHostAddress(translateAddress(vaddress));
    }

private:
    explicit MMU(std::shared_ptr<PhysMem> const &pMem)
        : pMem_(std::move(pMem)) {}

    RV64Ptr translateAddress(RV64Ptr address) const;

    std::shared_ptr<PhysMem> pMem_;
};

} // namespace besm::mem
