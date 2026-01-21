# Makefile for Hello World C program

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra
STRICT_CFLAGS = -Wall -Wextra -Wpedantic -Werror

# Source files
SOURCES = hello.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = hello

# Build targets
.PHONY: all clean format format-check test strict debug

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

strict: $(SOURCES)
	$(CC) $(STRICT_CFLAGS) -o $(TARGET) $(SOURCES)

debug: $(SOURCES)
	$(CC) $(CFLAGS) -g -o $(TARGET)_debug $(SOURCES)

# Format all C source files
format:
	@echo "Formatting C source files..."
	@clang-format -i $(SOURCES)
	@echo "Formatting complete."

# Check if code is properly formatted (non-destructive)
format-check:
	@echo "Checking code formatting..."
	@for file in $(SOURCES); do \
		if ! clang-format "$$file" | diff -u "$$file" - > /dev/null; then \
			echo "ERROR: $$file is not properly formatted"; \
			echo "Run 'make format' to fix formatting issues"; \
			exit 1; \
		fi; \
	done
	@echo "All files are properly formatted."

# Run tests (placeholder for CI)
test: strict
	@echo "Running tests..."
	@./$(TARGET)
	@echo "Tests passed."

clean:
	rm -f $(TARGET) $(TARGET)_debug *.o *.out *.exe
