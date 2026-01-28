# my_grep/Makefile - Build system only
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -g -D_POSIX_C_SOURCE=200809L
TARGET = my_grep
SOURCES = my_grep.c
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Remove build artifacts
clean:
	rm -f $(TARGET) $(OBJECTS)

# Run the full test suite
test: $(TARGET)
	@echo "Running test suite..."
	@if [ -f "test_grep.sh" ]; then \
		chmod +x test_grep.sh; \
		./test_grep.sh; \
	else \
		echo "Error: test_grep.sh not found"; \
		exit 1; \
	fi

# Quick sanity check (fast, for development)
quick-test: $(TARGET)
	@echo "Quick sanity test..."
	@echo "test" | ./$(TARGET) "test" > /dev/null && echo "✓ Basic search works"
	@./$(TARGET) --version > /dev/null && echo "✓ Version flag works"
	@./$(TARGET) --help 2>&1 | head -1 | grep -q "Usage:" && echo "✓ Help flag works"

# Install to /usr/local/bin (optional)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Uninstall (optional)
uninstall:
	rm -f /usr/local/bin/$(TARGET)

.PHONY: all clean test quick-test install uninstall