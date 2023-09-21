#pragma once

#include <filesystem>
#include <set>

#include "besm-666/memory/page-allocator.hpp"
#include "besm-666/non-copyable.hpp"
#include "besm-666/riscv-types.hpp"

namespace besm::mem {

class PhysMemPagemap : public INonCopyable {
public:
    using PageId = RV64Size;
    struct Page {
        RV64Size const id;
        char *const mem;
    };
    using Pagemap = std::set<Page, std::less<>>;

    PhysMemPagemap(size_t pageSize, size_t allocatorChunkSize);
    PhysMemPagemap(PhysMemPagemap &&other);
    ~PhysMemPagemap() = default;

    void storeByte(RV64Ptr address, RV64UChar value);
    RV64UChar loadByte(RV64Ptr address) const;

private:
    RV64Size addr2PageId(RV64Ptr address) const;
    RV64Size addr2PageOffset(RV64Ptr address) const;
    void *touchAddress(RV64Ptr address);
    void const *translate(RV64Ptr address) const;

    size_t const pageSize_;
    PageAllocator allocator_;
    mutable Pagemap pagemap_;
};

bool operator<(PhysMemPagemap::Page lhs, PhysMemPagemap::Page rhs);
bool operator<(PhysMemPagemap::Page lhs, PhysMemPagemap::PageId rhs);
bool operator<(PhysMemPagemap::PageId lhs, PhysMemPagemap::Page rhs);

class PhysMem : public INonCopyable {
public:
    ~PhysMem() = default;

    PhysMem(PhysMem &&other);

    template <typename RV64Type> void store(RV64Ptr address, RV64Type value);
    template <typename RV64Type> RV64Type load(RV64Ptr address) const;

private:
    friend class PhysMemBuilder;
    PhysMem(PhysMemPagemap &&pagemap);

    PhysMemPagemap pagemap_;
};

template <typename RV64Type>
void PhysMem::store(RV64Ptr address, RV64Type value) {
    for (RV64Ptr i = 0; i < sizeof(RV64Type); ++i) {
        pagemap_.storeByte(address + i, *((RV64UChar *)&value + i));
    }
}
template <typename RV64Type> RV64Type PhysMem::load(RV64Ptr address) const {
    RV64Type result;
    for (RV64Ptr i = 0; i < sizeof(RV64Type); ++i) {
        *((RV64UChar *)&result + i) = pagemap_.loadByte(address + i);
    }
    return result;
}

class PhysMemBuilder : public INonCopyable {
public:
    PhysMemBuilder(size_t pageSize,
                   size_t allocatorChunkSize = PageAllocator::DefaultChunkSize);
    ~PhysMemBuilder() = default;

    PhysMemBuilder &loadElf(std::filesystem::path elfPath);
    PhysMemBuilder &loadContArea(RV64Ptr address, void const *data,
                                 RV64Size size);

    PhysMem build();

private:
    PhysMemPagemap pagemap_;

#ifndef NDEBUG
    bool wasAlreadyBuilt_;
#endif
};

} // namespace besm::mem
