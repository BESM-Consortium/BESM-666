#include "besm-666/memory/mmu.hpp"

namespace besm::mem {

RV64UChar MMU::loadByte(RV64Ptr address) const {
    RV64Ptr paddr = this->translate(address);
    return pMem_->load<RV64UChar>(paddr);
}
RV64UHWord MMU::loadHWord(RV64Ptr address) const {
    RV64Ptr paddr = this->translate(address);
    return pMem_->load<RV64UHWord>(paddr);
}
RV64UWord MMU::loadWord(RV64Ptr address) const {
    RV64Ptr paddr = this->translate(address);
    return pMem_->load<RV64UWord>(paddr);
}
RV64UDWord MMU::loadDWord(RV64Ptr address) const {
    RV64Ptr paddr = this->translate(address);
    return pMem_->load<RV64UDWord>(paddr);
}

void MMU::storeByte(RV64Ptr address, RV64UChar value) {
    RV64Ptr paddr = this->translate(address);
    pMem_->store(paddr, value);
}
void MMU::storeHWord(RV64Ptr address, RV64HWord value) {
    RV64Ptr paddr = this->translate(address);
    pMem_->store(paddr, value);
}
void MMU::storeWord(RV64Ptr address, RV64Word value) {
    RV64Ptr paddr = this->translate(address);
    pMem_->store(paddr, value);
}
void MMU::storeDWord(RV64Ptr address, RV64DWord value) {
    RV64Ptr paddr = this->translate(address);
    pMem_->store(paddr, value);
}

RV64Ptr MMU::translate(RV64Ptr address) const { return address; }

} // namespace besm::mem
