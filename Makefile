# Makefile for Hello World C program
# Supports both default (no newline) and newline modes

CC = gcc
CFLAGS = -Wall -Wextra
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Werror -std=c99
TARGET = hello
TARGET_NEWLINE = hello_newline
TARGET_STRICT = hello_strict
SOURCE = hello.c

# Default target (no newline - original behavior)
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Version with newline for better terminal compatibility
newline: $(TARGET_NEWLINE)

$(TARGET_NEWLINE): $(SOURCE)
	$(CC) $(CFLAGS) -DWITH_NEWLINE -o $(TARGET_NEWLINE) $(SOURCE)

# Strict compilation (for CI validation)
strict: $(TARGET_STRICT)

$(TARGET_STRICT): $(SOURCE)
	$(CC) $(STRICT_FLAGS) -o $(TARGET_STRICT) $(SOURCE)

# Strict compilation with newline
strict-newline: 
	$(CC) $(STRICT_FLAGS) -DWITH_NEWLINE -o hello_strict_newline $(SOURCE)

# Test targets
test: $(TARGET_STRICT)
	./test/validate.sh

test-quiet: $(TARGET_STRICT)
	./test/validate.sh --quiet

# Clean up compiled binaries
clean:
	rm -f $(TARGET) $(TARGET_NEWLINE) $(TARGET_STRICT) hello_strict_newline *.exe *.out *.o

# Build all variants
all: $(TARGET) $(TARGET_NEWLINE) $(TARGET_STRICT)

# Help target
help:
	@echo "Available targets:"
	@echo "  make           - Build default version (no newline)"
	@echo "  make newline   - Build version with trailing newline"
	@echo "  make strict    - Build with strict compiler flags"
	@echo "  make test      - Run validation tests"
	@echo "  make clean     - Remove compiled binaries"
	@echo "  make all       - Build all variants"
	@echo ""
	@echo "Usage examples:"
	@echo "  ./hello        - Run default version"
	@echo "  ./hello_newline - Run version with newline"

.PHONY: newline strict strict-newline test test-quiet clean all help