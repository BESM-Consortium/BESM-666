# BESM-666

BESM-666 is a study project implementing a RISCV functional simulator.

# Building & testing commands (w/o e2e testing)

There is a `Makefile` file in the project root directory working as
pretty cute face of CMake build system. It has next common usage
build & test targets:
- `build` performs build of the BESM-666 simulator shared library and
    its standalone runner
- `test` performs unit test running
You can control build or unit test jobs count by setting variable `JOBS`. 
Example:
```
make JOBS=2 build
```

The BESM-666 standalone runner can be found at path 
`build/besm-666/standalone/besm666_standalone`.

# Building & running E2E tests

First of all, before building E2E tests you should build & install
RISCV toolchain (its default search path is `../sysroot` relatively
to the simulator root directory, but it can be changed by setting
`RISCV_SYSROOT` variable before make invokation (same as `JOBS` variable)).

So, the default installation script is next:
```
mkdir BESM-666
git clone https://github.com/riscv-collab/riscv-gnu-toolchain
git clone https://github.com/BESM-Consortium/BESM-666

cd riscv-gnu-toolchain
./configure --prefix=$PWD/../sysroot --enable-multilib --disable-gdb
make -j
cd ..
```

After this you can build & run E2E tests using commands
`make build-e2e` and `make test-e2e`.

The default target `all` depends on all mentioned targets:
`build`, `build-e2e`, `test`, `test-e2e`.

Good luck!
