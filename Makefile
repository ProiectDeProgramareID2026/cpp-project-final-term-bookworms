CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++17 -O2 -Iinclude
TARGET = build/library
BUILD_DIR = build
SRCS = src/main.cpp src/CliApplication.cpp src/Book.cpp src/Library.cpp src/FileManager.cpp
OBJS = $(SRCS:src/%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all clean run

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: $(TARGET)
	./$(TARGET) list
