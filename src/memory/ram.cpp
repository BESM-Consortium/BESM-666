#include <cassert>
#include <stdexcept>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "besm-666/memory/mmap-wrapper.hpp"
#include "besm-666/memory/ram.hpp"
#include "besm-666/util/math.hpp"

namespace besm::dev {

RAMPageAllocator::Chunk::Chunk(size_t pageSize, size_t chunkSize)
    : data_(nullptr), size_(0), rower_(nullptr), pageSize_(pageSize) {
    assert(chunkSize != 0);
    assert(pageSize != 0);
    assert(Is2Pow(pageSize));
    assert(chunkSize / pageSize != 0);

    size_t hostPageSize = getpagesize();
    size_t hostMmapSize = Nearest2PowDivident(chunkSize, hostPageSize);

    void *data = besm666_mmap(nullptr, hostMmapSize, PROT_READ | PROT_WRITE,
                              MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);

    if (data == nullptr) {
        throw std::bad_alloc();
    }

    rower_ = data_ = reinterpret_cast<char *>(data);
    size_ = hostMmapSize;
    pageSize_ = pageSize;
}

RAMPageAllocator::Chunk::Chunk(Chunk &&other) noexcept
    : data_(nullptr), rower_(nullptr), size_(0), pageSize_(0) {
    std::swap(other.data_, data_);
    std::swap(other.rower_, rower_);
    std::swap(other.size_, size_);
    std::swap(other.pageSize_, pageSize_);
}

RAMPageAllocator::Chunk::~Chunk() {
    if (data_ != nullptr) {
        besm666_munmap(data_, size_);
        data_ = rower_ = nullptr;
        size_ = pageSize_ = 0;
    }
}

void *RAMPageAllocator::Chunk::allocPage() noexcept {
    if (rower_ < data_ + size_) {
        void *page = rower_;
        rower_ += pageSize_;
        return page;
    } else {
        return nullptr;
    }
}

RAMPageAllocator::RAMPageAllocator(size_t pageSize, size_t chunkSize)
    : chunkSize_(chunkSize), pageSize_(pageSize) {
    assert(chunkSize != 0);
    assert(pageSize != 0);
    assert(Is2Pow(pageSize));
    assert(chunkSize / pageSize != 0);
}

RAMPageAllocator::RAMPageAllocator(RAMPageAllocator &&other) noexcept
    : chunks_(std::move(other.chunks_)), chunkSize_(other.chunkSize_),
      pageSize_(other.pageSize_) {}

void *RAMPageAllocator::allocPage() {
    void *page = chunks_.empty() ? nullptr : chunks_.back().allocPage();
    if (page == nullptr) {
        chunks_.emplace_back(pageSize_, chunkSize_);
        return chunks_.back().allocPage();
    } else {
        return page;
    }
}

RAM::RAM(size_t ramSize, size_t pageSize, size_t chunkSize)
    : IPhysMemDevice(IPhysMemDevice::RAM), ramSize_(ramSize),
      pageSize_(pageSize), allocator_(pageSize, chunkSize) {

    if (ramSize == 0) {
        throw std::invalid_argument("Invalid RAM size");
    }
    if (pageSize == 0 || !Is2Pow(pageSize)) {
        throw std::invalid_argument("Invalid RAM page size");
    }
    if (chunkSize / pageSize == 0) {
        throw std::invalid_argument("Invalid RAM chunk size");
    }
}

RAM::RAM(RAM &&other)
    : IPhysMemDevice(other.getType()), ramSize_(other.ramSize_),
      pageSize_(other.pageSize_), allocator_(std::move(other.allocator_)),
      pagemap_(std::move(other.pagemap_)) {}

RV64UChar RAM::loadByte(RV64Ptr address) const {
    return this->load<RV64UChar>(address);
}
RV64UHWord RAM::loadHWord(RV64Ptr address) const {
    return this->load<RV64UHWord>(address);
}
RV64UWord RAM::loadWord(RV64Ptr address) const {
    return this->load<RV64UWord>(address);
}
RV64UDWord RAM::loadDWord(RV64Ptr address) const {
    return this->load<RV64UDWord>(address);
}

void RAM::storeByte(RV64Ptr address, RV64UChar value) {
    this->store<RV64UChar>(address, value);
}
void RAM::storeHWord(RV64Ptr address, RV64UHWord value) {
    this->store<RV64UHWord>(address, value);
}
void RAM::storeWord(RV64Ptr address, RV64UWord value) {
    this->store<RV64UWord>(address, value);
}
void RAM::storeDWord(RV64Ptr address, RV64UDWord value) {
    this->store<RV64UDWord>(address, value);
}

std::pair<void const *, size_t> RAM::getHostAddress(RV64Ptr address) const {
    size_t offset = this->getPageOffset(address);

    void const *hostAddress = this->getPageAddress(address);
    if (hostAddress == nullptr) {
        return std::make_pair(nullptr, 0);
    }

    return std::make_pair(reinterpret_cast<char const *>(hostAddress) + offset,
                          pageSize_ - offset);
}

std::pair<void *, size_t> RAM::getHostAddress(RV64Ptr address) {
    size_t offset = this->getPageOffset(address);
    return std::make_pair(
        reinterpret_cast<char *>(this->touchPageAddress(address)) + offset,
        pageSize_ - offset);
}

size_t RAM::getSize() const noexcept { return ramSize_; }

bool operator<(RAM::Page left, RAM::Page right) noexcept {
    return left.id < right.id;
}
bool operator<(RAM::Page left, RAM::PageId right) noexcept {
    return left.id < right;
}
bool operator<(RAM::PageId left, RAM::Page right) noexcept {
    return left < right.id;
}

void const *RAM::getPageAddress(RV64Ptr address) const noexcept {
    auto itr = pagemap_.find(this->getPageId(address));
    if (itr == pagemap_.end()) {
        return nullptr;
    } else {
        return itr->mem;
    }
}

void *RAM::touchPageAddress(RV64Ptr address) {
    PageId pageId = this->getPageId(address);
    auto itr = pagemap_.find(pageId);

    if (itr == pagemap_.end()) {
        void *newPageAddress = allocator_.allocPage();
        Page newPage = {.id = pageId,
                        .mem = reinterpret_cast<char *>(newPageAddress)};
        pagemap_.insert(newPage);
        return newPageAddress;
    } else {
        return itr->mem;
    }
}

void RAM::validateAddressBounds(RV64Ptr address) const {
    if (address >= ramSize_) {
        throw IPhysMemDevice::InvalidAddressError("RAM out of bounds");
    }
}

RAM::PageId RAM::getPageId(RV64Ptr address) const noexcept {
    return address / pageSize_;
}
size_t RAM::getPageOffset(RV64Ptr address) const noexcept {
    return static_cast<size_t>(address) % pageSize_;
}

} // namespace besm::dev
