CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc -Istructs
LDFLAGS  = -lfltk -lsqlite3
BUILD_DIR = build

SRCDIR = src

SRC = $(SRCDIR)/StartApp.cpp \
      $(SRCDIR)/UI/Editor/Editor.cpp \
      $(SRCDIR)/UI/Panel/Panel.cpp \
      $(SRCDIR)/UI/TitleBar/TitleBar.cpp \
      $(SRCDIR)/Plumbing/Database/Database.cpp

OBJ = $(SRC:%.cpp=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR)/workspace

$(BUILD_DIR)/workspace: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/$(SRCDIR)
	mkdir -p $(BUILD_DIR)/$(SRCDIR)/UI/Editor
	mkdir -p $(BUILD_DIR)/$(SRCDIR)/UI/Panel
	mkdir -p $(BUILD_DIR)/$(SRCDIR)/UI/TitleBar
	mkdir -p $(BUILD_DIR)/$(SRCDIR)/Plumbing/Database


run: all
	./$(BUILD_DIR)/workspace

compiledb:
	bear -- make

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean compiledb
