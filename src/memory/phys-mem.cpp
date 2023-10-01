#include <cassert>
#include <stdexcept>

#include "besm-666/memory/phys-mem.hpp"
#include "besm-666/util/math.hpp"

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
    RV64UChar *pos = reinterpret_cast<RV64UChar *>(this->touchAddress(address));
    *pos = value;
}
RV64UChar PhysMemPagemap::loadByte(RV64Ptr address) const {
    RV64UChar const *pos =
        reinterpret_cast<RV64UChar const *>(this->translateAddress(address));
    return pos == nullptr ? 0 : *pos;
}

RV64Size PhysMemPagemap::addr2PageId(RV64Ptr address) const {
    return address / pageSize_;
}
RV64Size PhysMemPagemap::addr2PageOffset(RV64Ptr address) const {
    return address & (pageSize_ - 1);
}

void *PhysMemPagemap::touchAddress(RV64Ptr address) {
    RV64Size pageId = this->addr2PageId(address);
    auto pageItr = pagemap_.find(pageId);
    if (pageItr == pagemap_.end()) {
        Page page = {.id = pageId,
                     .mem = reinterpret_cast<char *>(allocator_.allocPage())};
        auto [itr, success] = pagemap_.insert(page);
        assert(success);
        pageItr = itr;
    }

    return pageItr->mem + this->addr2PageOffset(address);
}

void const *PhysMemPagemap::translateAddress(RV64Ptr address) const {
    auto pageItr = pagemap_.find(this->addr2PageId(address));
    if (pageItr == pagemap_.end()) {
        return nullptr;
    } else {
        return pageItr->mem + this->addr2PageOffset(address);
    }
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

PhysMem::PhysMem(PhysMem &&other) : pagemap_(std::move(other.pagemap_)) {}

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
    std::clog << "[PhysMem] Loading ELF file \'" << elfPath << "\'" << std::endl;

    auto parserPtr = besm::util::createParser(elfPath);
    for (const auto &seg : parserPtr->getLoadableSegments()) {
        std::clog << "\tLoading segment of size " << seg.size << " at address "
                  << seg.address << std::endl;

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

PhysMem::SPtr PhysMemBuilder::build() {
#ifndef NDEBUG
    assert(!wasAlreadyBuilt_);
    wasAlreadyBuilt_ = true;
#endif
    return PhysMem::SPtr(new PhysMem(std::move(pagemap_)));
}

} // namespace besm::mem
