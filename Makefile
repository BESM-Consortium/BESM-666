
BUILD_DIR := build
SRC_DIR := src
INC_DIR := include
TEST_DIR := test

JOBS ?= 4

.PHONY: all
all: init format build test

.PHONY: init
init:
	git submodule init
	git submodule update

.PHONY: format
.SILENT: format
format:
	./tools/clang-format.sh $(PWD)/$(SRC_DIR)
	./tools/clang-format.sh $(PWD)/$(INC_DIR)
	./tools/clang-format.sh $(PWD)/$(TEST_DIR)

.PHONY: build
build: init
	cmake -S $(PWD) -B $(PWD)/$(BUILD_DIR) $(CMAKE_FLAGS)
	make -C $(PWD)/$(BUILD_DIR) -j $(JOBS)

.PHONY: test
test: build
	make -C $(PWD)/$(BUILD_DIR) -j $(JOBS) test CTEST_OUTPUT_ON_FAILURE=1

.PHONY: clean
.SILENT: clean
clean:
	rm -rf $(PWD)/$(BUILD_DIR)
