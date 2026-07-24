CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS  = -lfltk
BUILD_DIR = build

SRCDIR = src

SRC = $(SRCDIR)/StartApp.cpp \
      $(SRCDIR)/Editor/Editor.cpp \
      $(SRCDIR)/Panel/Panel.cpp \
      $(SRCDIR)/TitleBar/TitleBar.cpp

OBJ = $(SRC:%.cpp=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR)/workspace

$(BUILD_DIR)/workspace: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/$(SRCDIR)
	mkdir -p $(BUILD_DIR)/$(SRCDIR)/Editor
	mkdir -p $(BUILD_DIR)/$(SRCDIR)/Panel
	mkdir -p $(BUILD_DIR)/$(SRCDIR)/TitleBar


run: all
	./$(BUILD_DIR)/workspace

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
