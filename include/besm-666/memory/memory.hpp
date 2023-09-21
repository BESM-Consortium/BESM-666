#pragma once

#include <stdexcept>

#include "besm-666/memory/phys-mem.hpp"
#include "besm-666/non-copyable.hpp"
#include "besm-666/riscv-types.hpp"

namespace besm::mem {

class Memory : public INonCopyable {
public:
    RV64UChar loadByte(RV64Ptr address) const;
    RV64UHWord loadHWord(RV64Ptr address) const;
    RV64UWord loadWord(RV64Ptr address) const;
    RV64UDWord loadDWord(RV64Ptr address) const;

    void storeByte(RV64Ptr address, RV64UChar value);
    void storeHWord(RV64Ptr address, RV64HWord value);
    void storeWord(RV64Ptr address, RV64Word value);
    void storeDWord(RV64Ptr address, RV64DWord value);

private:
    RV64Ptr translate(RV64Ptr address) const;

    friend class MemoryBuilder;
    Memory(PhysMem &&pMem);
    PhysMem pMem_;
};

class MemoryBuilder {
public:
    MemoryBuilder(size_t pageSize, size_t allocatorChunkSize)
        : pMemBuilder_(pageSize, allocatorChunkSize) {}

    PhysMemBuilder &physMem() { return pMemBuilder_; }

    Memory build() { return Memory(pMemBuilder_.build()); }

private:
    PhysMemBuilder pMemBuilder_;
};

} // namespace besm::mem
