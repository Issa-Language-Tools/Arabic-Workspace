CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc -Istructs $(shell pkg-config --cflags cairo)
LDFLAGS  = -lfltk -lsqlite3
BUILD_DIR = build
RELEASE_DIR = build/release

SRCDIR = src

SRC = $(SRCDIR)/StartApp.cpp \
      $(shell find $(SRCDIR)/UI $(SRCDIR)/Plumbing -name '*.cpp')

OBJ = $(SRC:%.cpp=$(BUILD_DIR)/%.o)

all: $(RELEASE_DIR)/workspace

$(RELEASE_DIR)/workspace: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp | $(RELEASE_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(RELEASE_DIR):
	mkdir -p $(RELEASE_DIR)
	mkdir -p $(sort $(dir $(OBJ)))


run: all
	./$(RELEASE_DIR)/workspace

compiledb: clean
	bear -- make

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean compiledb
