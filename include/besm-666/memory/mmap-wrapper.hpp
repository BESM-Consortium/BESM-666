#pragma once

#include <sys/types.h>

// This wraps will be used in unit test. I really don't know, how to
// wrap syscall mmap, munmap in unit test without changing the besm666
// source code, because gtest uses this syscalls too.
// I'm looking for better solutions.....

extern "C" {
void *besm666_mmap(void *addr, size_t length, int prot, int flags, int fd,
                   off_t offset);
int besm666_munmap(void *addr, size_t length);
}
