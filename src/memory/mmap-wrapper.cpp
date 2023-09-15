#include <sys/mman.h>

#include "besm-666/memory/mmap-wrapper.hpp"

extern "C" {
void *besm666_mmap(void *addr, size_t length, int prot, int flags, int fd,
                   off_t offset) {
    return ::mmap(addr, length, prot, flags, fd, offset);
}
int besm666_munmap(void *addr, size_t length) { return ::munmap(addr, length); }
}
