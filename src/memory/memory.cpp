#include "besm-666/memory/memory.hpp"

namespace besm::mem {

RV64UChar Memory::loadByte(RV64Ptr address) const {
    RV64Ptr paddr = this->translate(address);
    return pMem_.load<RV64UChar>(paddr);
}
RV64UHWord Memory::loadHWord(RV64Ptr address) const {
    RV64Ptr paddr = this->translate(address);
    return pMem_.load<RV64UHWord>(paddr);
}
RV64UWord Memory::loadWord(RV64Ptr address) const {
    RV64Ptr paddr = this->translate(address);
    return pMem_.load<RV64UWord>(paddr);
}
RV64UDWord Memory::loadDWord(RV64Ptr address) const {
    RV64Ptr paddr = this->translate(address);
    return pMem_.load<RV64UDWord>(paddr);
}

void Memory::storeByte(RV64Ptr address, RV64UChar value) {
    RV64Ptr paddr = this->translate(address);
    pMem_.store(paddr, value);
}
void Memory::storeHWord(RV64Ptr address, RV64HWord value) {
    RV64Ptr paddr = this->translate(address);
    pMem_.store(paddr, value);
}
void Memory::storeWord(RV64Ptr address, RV64Word value) {
    RV64Ptr paddr = this->translate(address);
    pMem_.store(paddr, value);
}
void Memory::storeDWord(RV64Ptr address, RV64DWord value) {
    RV64Ptr paddr = this->translate(address);
    pMem_.store(paddr, value);
}

RV64Ptr Memory::translate(RV64Ptr address) const { return address; }

Memory::Memory(PhysMem &&pMem) : pMem_(std::move(pMem)) {}

} // namespace besm::mem
