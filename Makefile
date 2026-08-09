CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc -Istructs
LDFLAGS  = -lfltk -lsqlite3
BUILD_DIR = build

SRCDIR = src

SRC = $(SRCDIR)/StartApp.cpp \
      $(shell find $(SRCDIR)/UI $(SRCDIR)/Plumbing -name '*.cpp')

OBJ = $(SRC:%.cpp=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR)/workspace

$(BUILD_DIR)/workspace: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/$(SRCDIR)
	mkdir -p $(sort $(dir $(OBJ)))


run: all
	./$(BUILD_DIR)/workspace

compiledb: clean
	bear -- make

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean compiledb
