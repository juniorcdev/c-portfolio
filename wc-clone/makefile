CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O2
TARGET = my_wc
SOURCES = my_wc.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJECTS)

test: $(TARGET)
	@echo "Running tests..."
	@chmod +x test_*.sh 2>/dev/null || true
	@for test in test_*.sh; do \
		if [ -f "$$test" ]; then \
			echo "=== Running $$test ==="; \
			./$$test; \
		fi; \
	done

.PHONY: all clean test