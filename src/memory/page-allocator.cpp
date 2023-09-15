#include <cassert>
#include <iostream>
#include <stdexcept>

#include <sys/mman.h>
#include <unistd.h>

#include "besm-666/math.hpp"
#include "besm-666/memory/mmap-wrapper.hpp"
#include "besm-666/memory/page-allocator.hpp"

namespace besm::mem {

PageAllocator::Chunk::Chunk(size_t chunkSize, size_t pageSize)
    : data_(nullptr), size_(0), rower_(nullptr), pageSize_(pageSize) {
    assert(chunkSize != 0);
    assert(pageSize != 0);
    assert(Is2Pow(pageSize));
    assert(chunkSize / pageSize != 0);

    size_t tdPageSize = getpagesize();
    size_t tdMmapSize = Nearest2PowDivident(chunkSize, tdPageSize);

    void *data = besm666_mmap(nullptr, tdMmapSize, PROT_READ | PROT_WRITE,
                              MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
    if (data == nullptr) {
        throw std::bad_alloc();
    }

    rower_ = data_ = reinterpret_cast<char *>(data);
    size_ = tdMmapSize;
    pageSize_ = pageSize;
}

PageAllocator::Chunk::Chunk(Chunk &&other)
    : data_(nullptr), rower_(nullptr), size_(0), pageSize_(0) {
    std::swap(other.data_, data_);
    std::swap(other.rower_, rower_);
    std::swap(other.size_, size_);
    std::swap(other.pageSize_, pageSize_);
}

PageAllocator::Chunk::~Chunk() {
    if (data_ != nullptr) {
        besm666_munmap(data_, size_);
        data_ = rower_ = nullptr;
    }
}

void *PageAllocator::Chunk::allocPage() noexcept {
    if (rower_ == data_ + size_) {
        return nullptr;
    } else {
        void *page = rower_;
        rower_ += pageSize_;
        return page;
    }
}

PageAllocator::PageAllocator(size_t chunkSize, size_t pageSize)
    : chunks_(), chunkSize_(chunkSize), pageSize_(pageSize) {
    assert(chunkSize != 0);
    assert(pageSize != 0);
    assert(Is2Pow(pageSize));
    assert(chunkSize / pageSize != 0);
}

PageAllocator::PageAllocator(PageAllocator &&other)
    : chunks_(std::move(other.chunks_)), chunkSize_(other.chunkSize_),
      pageSize_(other.pageSize_) {}

void *PageAllocator::allocPage() {
    void *page = chunks_.empty() ? nullptr : chunks_.back().allocPage();
    if (page == nullptr) {
        chunks_.emplace_back(chunkSize_, pageSize_);
        return chunks_.back().allocPage();
    } else {
        return page;
    }
}

} // namespace besm::mem
