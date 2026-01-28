# vector_int/Makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -g -I./include
AR = ar
ARFLAGS = rcs

# Directories
SRC_DIR = src
TEST_DIR = tests
INCLUDE_DIR = include
BUILD_DIR = build
EXAMPLES_DIR = examples

# Targets
TARGET = libvector_int.a
TEST_TARGET = test_vector
EXAMPLE_TARGET = example_vector

# Source files
SRC_FILES = $(SRC_DIR)/vector_int.c
OBJ_FILES = $(SRC_FILES:.c=.o)

# Test files
TEST_SRC = $(TEST_DIR)/test_vector.c
TEST_OBJ = $(TEST_SRC:.c=.o)

# Example files
EXAMPLE_SRC = $(EXAMPLES_DIR)/example_usage.c
EXAMPLE_OBJ = $(EXAMPLE_SRC:.c=.o)

# Default target
all: $(TARGET) $(TEST_TARGET) $(EXAMPLE_TARGET)

# Static library
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BUILD_DIR)
	$(AR) $(ARFLAGS) $(BUILD_DIR)/$@ $^
	@echo "✓ Built static library: $(BUILD_DIR)/$@"

# Test executable
$(TEST_TARGET): $(TEST_OBJ) $(TARGET)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $(TEST_OBJ) -L$(BUILD_DIR) -lvector_int
	@echo "✓ Built test executable: $(BUILD_DIR)/$@"

# Example executable
$(EXAMPLE_TARGET): $(EXAMPLE_OBJ) $(TARGET)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $(EXAMPLE_OBJ) -L$(BUILD_DIR) -lvector_int
	@echo "✓ Built example executable: $(BUILD_DIR)/$@"

# Object files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/vector_int.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_DIR)/%.o: $(TEST_DIR)/%.c $(INCLUDE_DIR)/vector_int.h
	$(CC) $(CFLAGS) -c $< -o $@

$(EXAMPLES_DIR)/%.o: $(EXAMPLES_DIR)/%.c $(INCLUDE_DIR)/vector_int.h
	$(CC) $(CFLAGS) -c $< -o $@

# Run tests
test: $(TEST_TARGET)
	@echo "=== Running tests ==="
	@cd $(BUILD_DIR) && ./$(TEST_TARGET)

# Run example
example: $(EXAMPLE_TARGET)
	@echo "=== Running example ==="
	@cd $(BUILD_DIR) && ./$(EXAMPLE_TARGET)

# Quick check (fast)
check: $(TEST_TARGET)
	@echo "Quick sanity check..."
	@cd $(BUILD_DIR) && timeout 2 ./$(TEST_TARGET) && echo "✓ Basic tests pass"

# Run with valgrind (if available)
valgrind-test: $(TEST_TARGET)
	@if command -v valgrind >/dev/null 2>&1; then \
		echo "=== Running with Valgrind ==="; \
		cd $(BUILD_DIR) && valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_TARGET); \
	else \
		echo "Valgrind not found. Install with: sudo apt-get install valgrind"; \
	fi

# Install (optional)
install: $(TARGET)
	@echo "Installing to /usr/local..."
	@sudo cp $(INCLUDE_DIR)/vector_int.h /usr/local/include/
	@sudo cp $(BUILD_DIR)/$(TARGET) /usr/local/lib/
	@echo "✓ Installed. Use with: -lvector_int"

# Uninstall (optional)
uninstall:
	@echo "Uninstalling..."
	@sudo rm -f /usr/local/include/vector_int.h
	@sudo rm -f /usr/local/lib/$(TARGET)
	@echo "✓ Uninstalled"

# Clean up
clean:
	rm -rf $(BUILD_DIR) $(OBJ_FILES) $(TEST_OBJ) $(EXAMPLE_OBJ)
	@echo "✓ Cleaned build artifacts"

# Create directory structure
init:
	mkdir -p $(SRC_DIR) $(TEST_DIR) $(INCLUDE_DIR) $(EXAMPLES_DIR) $(BUILD_DIR)
	@echo "✓ Created directory structure"

# Help
help:
	@echo "Available targets:"
	@echo "  all       - Build everything (library, tests, examples)"
	@echo "  test      - Build and run tests"
	@echo "  example   - Build and run example"
	@echo "  check     - Quick sanity check"
	@echo "  valgrind-test - Run tests with Valgrind memory checker"
	@echo "  install   - Install library system-wide"
	@echo "  uninstall - Uninstall library"
	@echo "  clean     - Remove build artifacts"
	@echo "  init      - Create directory structure"
	@echo "  help      - Show this help"

.PHONY: all test example check valgrind-test install uninstall clean init help