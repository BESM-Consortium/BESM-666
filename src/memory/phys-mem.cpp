#include <cassert>
#include <stdexcept>

#include "besm-666/memory/phys-mem.hpp"
#include "besm-666/utils/math.hpp"

namespace besm::mem {

PhysMemPagemap::PhysMemPagemap(size_t pageSize, size_t allocatorChunkSize)
    : pageSize_(pageSize), allocator_(allocatorChunkSize, pageSize) {

    if (pageSize == 0 || !Is2Pow(pageSize)) {
        throw std::invalid_argument("Invalid page size");
    }
    if (allocatorChunkSize / pageSize == 0) {
        throw std::invalid_argument("Invalid chunk size & page size ratio");
    }
}

PhysMemPagemap::PhysMemPagemap(PhysMemPagemap &&other)
    : pageSize_(other.pageSize_), allocator_(std::move(other.allocator_)),
      pagemap_(std::move(other.pagemap_)) {}

void PhysMemPagemap::storeByte(RV64Ptr address, RV64UChar value) {
    RV64UChar *pos = reinterpret_cast<RV64UChar *>(this->translate(address));
    *pos = value;
}
RV64UChar PhysMemPagemap::loadByte(RV64Ptr address) {
    RV64UChar *pos = reinterpret_cast<RV64UChar *>(this->translate(address));
    return *pos;
}

RV64Size PhysMemPagemap::addr2PageId(RV64Ptr address) const {
    return address / pageSize_;
}
RV64Size PhysMemPagemap::addr2PageOffset(RV64Ptr address) const {
    return address & (pageSize_ - 1);
}

PhysMemPagemap::Pagemap::iterator
PhysMemPagemap::touchAddress(RV64Ptr address) {
    RV64Size pageId = this->addr2PageId(address);
    auto pageItr = pagemap_.find(pageId);
    if (pageItr == pagemap_.end()) {
        Page page = {.id = pageId,
                     .mem = reinterpret_cast<char *>(allocator_.allocPage())};
        auto [itr, success] = pagemap_.insert(page);
        assert(success);
        return itr;
    } else {
        return pageItr;
    }
}

void *PhysMemPagemap::translate(RV64Ptr address) {
    auto pageItr = this->touchAddress(address);
    RV64Size offset = this->addr2PageOffset(address);
    return pageItr->mem + static_cast<size_t>(offset);
}

bool operator<(PhysMemPagemap::Page lhs, PhysMemPagemap::Page rhs) {
    return lhs.id < rhs.id;
}
bool operator<(PhysMemPagemap::Page lhs, PhysMemPagemap::PageId rhs) {
    return lhs.id < rhs;
}
bool operator<(PhysMemPagemap::PageId lhs, PhysMemPagemap::Page rhs) {
    return lhs < rhs.id;
}

PhysMem::PhysMem(PhysMemPagemap &&pagemap) : pagemap_(std::move(pagemap)) {}

PhysMemBuilder::PhysMemBuilder(size_t pageSize, size_t allocatorChunkSize)
    : pagemap_(pageSize, allocatorChunkSize)
#ifndef NDEBUG
      ,
      wasAlreadyBuilt_(false)
#endif
{
}

PhysMemBuilder &PhysMemBuilder::loadElf(const std::filesystem::path &elfPath) {
    besm::utl::ElfParser parser(elfPath);
    for (const auto &seg : parser.getLoadableSegments()) {
        this->loadContArea(seg.address, seg.data, seg.size);
    }
    return *this;
}

PhysMemBuilder &PhysMemBuilder::loadContArea(RV64Ptr address, void const *data,
                                             RV64Size size) {
    for (RV64Size i = 0; i < size; ++i) {
        pagemap_.storeByte(address + i, *((RV64UChar const *)data + i));
    }

    return *this;
}

PhysMem PhysMemBuilder::build() {
#ifndef NDEBUG
    assert(!wasAlreadyBuilt_);
    wasAlreadyBuilt_ = true;
#endif
    return PhysMem(std::move(pagemap_));
}

} // namespace besm::mem
