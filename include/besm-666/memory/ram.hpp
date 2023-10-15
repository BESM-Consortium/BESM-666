#pragma once

#include <cstddef>
#include <set>
#include <vector>

#include "besm-666/memory/phys-mem-device.hpp"
#include "besm-666/riscv-types.hpp"
#include "besm-666/util/non-copyable.hpp"

namespace besm::dev {

class RAMPageAllocator : public INonCopyable {
public:
    RAMPageAllocator(size_t pageSize, size_t chunkSize);
    RAMPageAllocator(RAMPageAllocator &&other) noexcept;
    ~RAMPageAllocator() = default;

    void *allocPage();

private:
    class Chunk;

    size_t chunkSize_;
    size_t pageSize_;
    std::vector<Chunk> chunks_;
};

class RAMPageAllocator::Chunk : public INonCopyable {
public:
    Chunk(size_t pageSize, size_t chunkSize);
    Chunk(Chunk &&other) noexcept;
    ~Chunk();

    void *allocPage() noexcept;

private:
    char *data_;
    char *rower_;
    size_t size_;
    size_t pageSize_;
};

class RAM : public mem::IPhysMemDevice, public INonCopyable {
public:
    RAM(RV64Size ramSize, size_t pageSize, size_t chunkSize);
    RAM(RAM &&other);
    virtual ~RAM() {}

    RV64UChar loadByte(RV64Ptr address) const override;
    RV64UHWord loadHWord(RV64Ptr address) const override;
    RV64UWord loadWord(RV64Ptr address) const override;
    RV64UDWord loadDWord(RV64Ptr address) const override;

    void storeByte(RV64Ptr address, RV64UChar value) override;
    void storeHWord(RV64Ptr address, RV64UHWord value) override;
    void storeWord(RV64Ptr address, RV64UWord value) override;
    void storeDWord(RV64Ptr address, RV64UDWord value) override;

    std::pair<void const *, size_t>
    getHostAddress(RV64Ptr address) const override;
    std::pair<void *, size_t> getHostAddress(RV64Ptr address) override;

    size_t getSize() const noexcept override;

private:
    using PageId = RV64Size;
    struct Page {
        PageId const id;
        char *const mem;
    };

    friend bool operator<(Page left, Page right) noexcept;
    friend bool operator<(Page left, PageId right) noexcept;
    friend bool operator<(PageId left, Page right) noexcept;

    using Pagemap = std::set<Page, std::less<>>;

    void validateAddressBounds(RV64Ptr address) const;

    template <typename DataType>
    void validateAddressAlignment(RV64Ptr address) const;

    PageId getPageId(RV64Ptr address) const noexcept;
    void const *getPageAddress(RV64Ptr address) const noexcept;
    void *touchPageAddress(RV64Ptr address);
    size_t getPageOffset(RV64Ptr address) const noexcept;

    template <typename DataType> DataType load(RV64Ptr address) const;

    template <typename DataType> void store(RV64Ptr address, DataType value);

    size_t ramSize_;
    size_t pageSize_;
    RAMPageAllocator allocator_;
    Pagemap pagemap_;
};

template <typename DataType>
void RAM::validateAddressAlignment(RV64Ptr address) const {
    if (address % sizeof(DataType) != 0) {
        throw IPhysMemDevice::UnalignedAddressError("RAM unaligned access");
    }
}

template <typename DataType> DataType RAM::load(RV64Ptr address) const {
    this->validateAddressAlignment<DataType>(address);

    auto [hostAddress, size] = this->getHostAddress(address);
    return hostAddress == nullptr
               ? 0
               : *reinterpret_cast<DataType const *>(hostAddress);
}

template <typename DataType> void RAM::store(RV64Ptr address, DataType value) {
    this->validateAddressAlignment<DataType>(address);

    auto [hostAddress, size] = this->getHostAddress(address);
    *reinterpret_cast<DataType *>(hostAddress) = value;
}

} // namespace besm::dev
