
BUILD_DIR := build
SRC_DIR := src

JOBS ?= 4

.PHONY: all
all: init format build test

.PHONY: init
init:
	git submodule init

.PHONY: format
.SILENT: format
format:
	./tools/clang-format.sh $(SRC_DIR)

.PHONY: build
build: init
	cmake -S $(PWD) -B $(PWD)/$(BUILD_DIR)
	make -C $(PWD)/$(BUILD_DIR) -j $(JOBS)

.PHONY: test
test: build
	make -C $(PWD)/$(BUILD_DIR) -j $(JOBS) test

.PHONY: clean
.SILENT: clean
clean:
	rm -rf $(PWD)/$(BUILD_DIR)
