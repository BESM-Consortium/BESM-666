#include "besm-666/memory/mmu.hpp"

namespace besm::mem {

MMU::SPtr MMU::Create(std::shared_ptr<PhysMem> const &pMem) {
    return SPtr(new MMU(pMem));
}

RV64UChar MMU::loadByte(RV64Ptr address) const {
    RV64Ptr paddr = this->translateAddress(address);
    return pMem_->loadByte(address);
}
RV64UHWord MMU::loadHWord(RV64Ptr address) const {
    RV64Ptr paddr = this->translateAddress(address);
    return pMem_->loadHWord(paddr);
}
RV64UWord MMU::loadWord(RV64Ptr address) const {
    RV64Ptr paddr = this->translateAddress(address);
    return pMem_->loadWord(paddr);
}
RV64UDWord MMU::loadDWord(RV64Ptr address) const {
    RV64Ptr paddr = this->translateAddress(address);
    return pMem_->loadDWord(paddr);
}

void MMU::storeByte(RV64Ptr address, RV64UChar value) {
    RV64Ptr paddr = this->translateAddress(address);
    pMem_->storeByte(paddr, value);
}
void MMU::storeHWord(RV64Ptr address, RV64HWord value) {
    RV64Ptr paddr = this->translateAddress(address);
    pMem_->storeHWord(paddr, value);
}
void MMU::storeWord(RV64Ptr address, RV64Word value) {
    RV64Ptr paddr = this->translateAddress(address);
    pMem_->storeWord(paddr, value);
}
void MMU::storeDWord(RV64Ptr address, RV64DWord value) {
    RV64Ptr paddr = this->translateAddress(address);
    pMem_->storeDWord(paddr, value);
}

RV64Ptr MMU::translateAddress(RV64Ptr address) const { return address; }

} // namespace besm::mem
