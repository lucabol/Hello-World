# Makefile for metrics_spreadsheet tool

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror
SOURCES = metrics_spreadsheet.c metrics.c
HEADERS = metrics.h
TARGET = metrics_spreadsheet

# Default target
all: $(TARGET)

# Build the metrics spreadsheet tool
$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

# Build with debug symbols
debug: CFLAGS += -g -O0
debug: $(TARGET)

# Build with optimizations
optimized: CFLAGS += -O2
optimized: $(TARGET)

# Run tests
test: $(TARGET)
	@echo "Running tests..."
	@./test/test_metrics.sh

# Check for memory leaks with valgrind
memcheck: debug
	@echo "Running valgrind memory check..."
	valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 \
		./$(TARGET) hello.c

# Clean build artifacts
clean:
	rm -f $(TARGET) metrics_spreadsheet_debug metrics_spreadsheet_optimized

# Show help
help:
	@echo "Available targets:"
	@echo "  all        - Build the metrics spreadsheet tool (default)"
	@echo "  debug      - Build with debug symbols"
	@echo "  optimized  - Build with optimizations"
	@echo "  test       - Run the test suite"
	@echo "  memcheck   - Check for memory leaks with valgrind"
	@echo "  clean      - Remove build artifacts"
	@echo "  help       - Show this help message"

.PHONY: all debug optimized test memcheck clean help
