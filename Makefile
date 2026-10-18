# Thin wrapper over CMake — keeps the `make run` workflow.
BUILD_DIR := build
BINARY    := $(BUILD_DIR)/release/workspace
JOBS      := $(shell nproc 2>/dev/null || echo 4)

# Suppress noisy Wayland text-input debug messages by default.
QT_LOGGING_RULES ?= qt.qpa.wayland.textinput=false

.PHONY: all run run-debug clean configure

all:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release
	cmake --build $(BUILD_DIR) -j$(JOBS)

run: all
	QT_LOGGING_RULES="$(QT_LOGGING_RULES)" ./$(BINARY)

# Same as `run`, but leaves all Qt logging untouched.
run-debug: all
	./$(BINARY)

configure:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release

clean:
	rm -rf $(BUILD_DIR)
