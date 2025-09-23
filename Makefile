# Makefile for Hello World C program
# Standardizes build commands and provides consistent targets for development and CI

# Compiler and flags configuration
CC ?= gcc
CLANG ?= clang
CFLAGS ?= -Wall -Wextra
OPTFLAGS = -O2
DEBUGFLAGS = -g
STRICT_FLAGS = -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror

# Source files and targets
SOURCE = hello.c
TARGET = hello
DEBUG_TARGET = hello_debug
CLANG_TARGET = hello_clang
STRICT_TARGET = hello_strict

# Default target (optimized build)
all: $(TARGET)

# Optimized build target
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(OPTFLAGS) -o $(TARGET) $(SOURCE)

# Debug build target
debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(DEBUGFLAGS) -o $(DEBUG_TARGET) $(SOURCE)

# Strict compilation target (with -Werror)
strict: $(STRICT_TARGET)

$(STRICT_TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(STRICT_FLAGS) -o $(STRICT_TARGET) $(SOURCE)

# Clang build target
clang: $(CLANG_TARGET)

$(CLANG_TARGET): $(SOURCE)
	$(CLANG) -o $(CLANG_TARGET) $(SOURCE)

# Run the default binary
run: $(TARGET)
	./$(TARGET)

# Test target: build with strict flags and run, return non-zero on failure
test: strict
	./$(STRICT_TARGET)
	@echo "All tests passed successfully!"

# Validate target: run validation script on built binaries
validate: strict
	@chmod +x validate.sh
	@echo "=== Validating strict build ==="
	@./validate.sh ./$(STRICT_TARGET)

# Comprehensive validation target: test all build variants
validate-all: all strict clang
	@chmod +x validate.sh
	@echo "=== Validating optimized build ==="
	@./validate.sh ./$(TARGET)
	@echo ""
	@echo "=== Validating strict build ==="
	@./validate.sh ./$(STRICT_TARGET)
	@echo ""
	@echo "=== Validating Clang build ==="
	@./validate.sh ./$(CLANG_TARGET)
# Clean build artifacts
clean:
	rm -f $(TARGET) $(DEBUG_TARGET) $(CLANG_TARGET) $(STRICT_TARGET) *.exe *.out *.o

# Help target to show available commands
help:
	@echo "Available targets:"
	@echo "  all      - Build optimized hello (default)"
	@echo "  debug    - Build with debug flags (-g)"
	@echo "  strict   - Build with strict warnings and -Werror"
	@echo "  clang    - Build with clang compiler"
	@echo "  run      - Run the default binary"
	@echo "  test     - Build strict and run (for CI/validation)"
	@echo "  validate - Validate strict build output and exit code"
	@echo "  validate-all - Validate all build variants"
	@echo "  clean    - Remove all build artifacts"
	@echo "  help     - Show this help message"

.PHONY: all debug strict clang run test validate validate-all clean help
