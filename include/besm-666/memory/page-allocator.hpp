#pragma once

#include <cstddef>
#include <vector>

#include "besm-666/non-copyable.hpp"

namespace besm::mem {

// THE MAIN TERMS USED IN DOCS
// - Target device (TD) is a device where the simulator will be executed
//  (x86_64 or arm in our case)
// - Simulated device (SD) is a device which will be simulated
//  (RISCV64 in our case)

/**
 * \brief PagePool class is repsonsible for memory allocation on target
 *  device, forming simulated device pages & giving them to the user
 */
class PageAllocator : public INonCopyable {
public:
    static constexpr size_t DefaultChunkSize = 1024 * 1024 + 2; // 2MB

    class Chunk;

    /**
     * \param [in] chunkSize amount of memory could be allocated in
     *  one time
     * \param [in] pageSize simulated device page size
     */
    PageAllocator(size_t chunkSize, size_t pageSize);
    PageAllocator(PageAllocator &&other);
    ~PageAllocator() = default;

    void *allocPage();

private:
    std::vector<Chunk> chunks_;
    size_t chunkSize_;
    size_t pageSize_;
};

class PageAllocator::Chunk : public INonCopyable {
public:
    Chunk(size_t chunkSize, size_t pageSize);
    Chunk(Chunk &&other);
    ~Chunk();

    void *allocPage() noexcept;

private:
    char *data_;
    char *rower_;
    size_t size_;
    size_t pageSize_;
};

} // namespace besm::mem
