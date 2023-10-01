
BUILD_DIR := build
SRC_DIR := src
INC_DIR := include
TEST_DIR := test
STANDALONE_DIR := standalone

RISCV_SYSROOT := $(PWD)/../sysroot

JOBS ?= 4

.PHONY: all
all: format build test


.PHONY: format
.SILENT: format
format:
	./tools/clang-format.sh $(PWD)/$(SRC_DIR)
	./tools/clang-format.sh $(PWD)/$(INC_DIR)
	./tools/clang-format.sh $(PWD)/$(TEST_DIR)
	./tools/clang-format.sh $(PWD)/$(STANDALONE_DIR)

.PHONY: build
build:
	cmake -S $(PWD) -B $(PWD)/$(BUILD_DIR)
	make -C $(PWD)/$(BUILD_DIR) -j $(JOBS)

.PHONY: build-val
build-val:
	cmake -S $(PWD) -B $(PWD)/$(BUILD_DIR) -DBESM666_TEST_WITH_VALGRIND=ON
	make -C $(PWD)/$(BUILD_DIR) -j $(JOBS)

.PHONY: build-e2e
build-e2e:
	cmake -S $(PWD) -B $(PWD)/$(BUILD_DIR) -DBESM666_WITH_E2E_TESTS=ON \
		-DBESM666_RISCV_SYSROOT=$(RISCV_SYSROOT)
	make -C $(PWD)/$(BUILD_DIR) -j $(JOBS)

.PHONY: test
test: build
	make -C $(PWD)/$(BUILD_DIR)/besm-666 -j $(JOBS) test CTEST_OUTPUT_ON_FAILURE=1

.PHONY: test-val
test-val: build-val
	make -C $(PWD)/$(BUILD_DIR)/besm-666 -j $(JOBS) test CTEST_OUTPUT_ON_FAILURE=1

.PHONY: test-e2e
test_e2e: build-e2e



.PHONY: clean
.SILENT: clean
clean:
	rm -rf $(PWD)/$(BUILD_DIR)
	rm -rf $(PWD)/include/autogen
