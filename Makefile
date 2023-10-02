
BUILD_DIR := build
SRC_DIR := src
INC_DIR := include
TEST_DIR := test
STANDALONE_DIR := standalone

RISCV_SYSROOT := $(PWD)/../sysroot

JOBS ?= 4

.PHONY: all
all: test test-e2e


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
	cmake --build $(PWD)/$(BUILD_DIR) --parallel $(JOBS)

.PHONY: build-val
build-val:
	cmake -S $(PWD) -B $(PWD)/$(BUILD_DIR) -DBESM666_TEST_WITH_VALGRIND=ON
	cmake --build $(PWD)/$(BUILD_DIR) --parallel $(JOBS)

.PHONY: build-e2e
build-e2e:
	cmake -S $(PWD) -B $(PWD)/$(BUILD_DIR) -DBESM666_WITH_E2E_TESTS=ON \
		-DBESM666_RISCV_SYSROOT=$(RISCV_SYSROOT)
	cmake --build $(PWD)/$(BUILD_DIR) --parallel $(JOBS)

.PHONY: test
test: build
	ctest $(PWD)/$(BUILD_DIR)/besm-666 --parallel $(JOBS) --output-on-failure

.PHONY: test-val
test-val: build-val
	ctest $(PWD)/$(BUILD_DIR)/besm-666 --parallel $(JOBS) --output-on-failure

.PHONY: test-e2e
test-e2e: build-e2e
	ctest --test-dir $(PWD)/$(BUILD_DIR)/e2e --output-on-failure

.PHONY: clean
.SILENT: clean
clean:
	rm -rf $(PWD)/$(BUILD_DIR)
	rm -rf $(PWD)/include/autogen
