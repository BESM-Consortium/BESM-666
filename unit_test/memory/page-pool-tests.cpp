#include <stack>
#include <sys/mman.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "besm-666/memory/ram.hpp"

char *MMapReturnValue = nullptr;

extern "C" {

extern void *__real_besm666_mmap(void *addr, size_t len, int prot, int flags,
                                 int fd, off_t off);
extern int __real_besm666_munmap(void *addr, size_t len);

void *__wrap_besm666_mmap(void *addr, size_t len, int prot, int flags, int fd,
                          off_t off) {
    return MMapReturnValue;
}
int __wrap_besm666_munmap(void *addr, size_t len) { return 0; }
}

TEST(page_allocator, out_of_memory) {
    MMapReturnValue = nullptr;
    EXPECT_THROW(besm::mem::RAMPageAllocator pageAllocator(4096, 1024 * 1024);
                 pageAllocator.allocPage();, std::bad_alloc);
}

TEST(page_allocator, page_overlapping) {
    constexpr size_t PageSize = 1024;
    constexpr size_t ChunkSize = PageSize * 2;
    MMapReturnValue = new char[ChunkSize];

    besm::mem::RAMPageAllocator allocator(PageSize, ChunkSize);

    size_t pageAddr1 = reinterpret_cast<size_t>(allocator.allocPage());

    MMapReturnValue = nullptr;

    size_t pageAddr2 = reinterpret_cast<size_t>(allocator.allocPage());

    EXPECT_GE(std::max(pageAddr1, pageAddr2) - std::min(pageAddr1, pageAddr2),
              PageSize);

    delete[] MMapReturnValue;
}

TEST(page_allocator, page_writting) {
    constexpr size_t PageSize = 1024;
    constexpr size_t ChunkSize = PageSize * 2;
    MMapReturnValue = new char[ChunkSize];

    besm::mem::RAMPageAllocator allocator(PageSize, ChunkSize);

    void *page1 = allocator.allocPage();
    for (char *c = reinterpret_cast<char *>(page1);
         c < reinterpret_cast<char *>(page1) + PageSize; ++c) {
        *c = 42;
    }
}
