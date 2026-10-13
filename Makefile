# Thin wrapper over CMake — keeps the `make run` workflow.
BUILD_DIR := build
BINARY    := $(BUILD_DIR)/release/workspace
JOBS      := $(shell nproc 2>/dev/null || echo 4)

.PHONY: all run clean configure

all:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release
	cmake --build $(BUILD_DIR) -j$(JOBS)

run: all
	./$(BINARY)

configure:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release

clean:
	rm -rf $(BUILD_DIR)
