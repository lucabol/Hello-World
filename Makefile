# Makefile for Hello World C program
# Includes standard builds and sanitizer builds for runtime error detection

# Compiler settings
CC = gcc
CLANG = clang
CFLAGS = -Wall -Wextra
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Werror
DEBUG_FLAGS = -g -Wall -Wextra
OPT_FLAGS = -O2 -Wall -Wextra

# Source files
SRC = hello.c
TARGET = hello

# Sanitizer flags
ASAN_FLAGS = -fsanitize=address -fno-omit-frame-pointer -g
UBSAN_FLAGS = -fsanitize=undefined -fno-omit-frame-pointer -g
MSAN_FLAGS = -fsanitize=memory -fno-omit-frame-pointer -g -fPIE -pie

# Default target
all: $(TARGET)

# Standard build
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Strict build with all warnings as errors (used by CI)
strict: $(SRC)
	$(CC) $(STRICT_FLAGS) -o hello_strict $(SRC)
	@./hello_strict

# Debug build
debug: $(SRC)
	$(CC) $(DEBUG_FLAGS) -o hello_debug $(SRC)

# Optimized build
optimized: $(SRC)
	$(CC) $(OPT_FLAGS) -o hello_optimized $(SRC)

# Clang build
clang: $(SRC)
	$(CLANG) $(CFLAGS) -o hello_clang $(SRC)

# AddressSanitizer build - detects memory errors
asan: $(SRC)
	$(CC) $(CFLAGS) $(ASAN_FLAGS) -o hello_asan $(SRC)
	@echo "AddressSanitizer build complete. Run with: ./hello_asan"

# UndefinedBehaviorSanitizer build - detects undefined behavior
ubsan: $(SRC)
	$(CC) $(CFLAGS) $(UBSAN_FLAGS) -o hello_ubsan $(SRC)
	@echo "UndefinedBehaviorSanitizer build complete. Run with: ./hello_ubsan"

# MemorySanitizer build - detects uninitialized memory reads
# Note: MemorySanitizer requires Clang and instrumented libraries
msan: $(SRC)
	$(CLANG) $(CFLAGS) $(MSAN_FLAGS) -o hello_msan $(SRC)
	@echo "MemorySanitizer build complete. Run with: ./hello_msan"

# Test target - runs validation
test: strict
	@echo "Running validation tests..."
	@./test/validate.sh

# Quiet test target (minimal output, used by CI)
test-quiet: strict
	@./test/validate.sh --quiet

# Unit test target (placeholder for future unit tests)
unit-test:
	@echo "No unit tests defined yet"

# Test all sanitizer builds
test-sanitizers: asan ubsan msan
	@echo "Testing AddressSanitizer build..."
	@./hello_asan > /dev/null && echo "  ✓ ASan build runs successfully"
	@echo "Testing UndefinedBehaviorSanitizer build..."
	@./hello_ubsan > /dev/null && echo "  ✓ UBSan build runs successfully"
	@echo "Testing MemorySanitizer build..."
	@./hello_msan > /dev/null && echo "  ✓ MSan build runs successfully"
	@echo "All sanitizer builds passed!"

# Clean build artifacts
clean:
	rm -f $(TARGET) hello_debug hello_optimized hello_strict hello_clang
	rm -f hello_asan hello_ubsan hello_msan
	rm -f *.o *.exe *.out

# Help target
help:
	@echo "Available targets:"
	@echo "  make          - Build standard executable"
	@echo "  make strict   - Build with strict warnings (CI mode)"
	@echo "  make debug    - Build with debug symbols"
	@echo "  make optimized- Build with optimizations"
	@echo "  make clang    - Build with Clang compiler"
	@echo ""
	@echo "Sanitizer builds (for error detection):"
	@echo "  make asan     - Build with AddressSanitizer"
	@echo "  make ubsan    - Build with UndefinedBehaviorSanitizer"
	@echo "  make msan     - Build with MemorySanitizer (Clang only)"
	@echo "  make test-sanitizers - Build and test all sanitizers"
	@echo ""
	@echo "Testing:"
	@echo "  make test     - Run validation tests"
	@echo "  make test-quiet - Run tests with minimal output"
	@echo ""
	@echo "Utilities:"
	@echo "  make clean    - Remove all build artifacts"
	@echo "  make help     - Show this help message"

.PHONY: all strict debug optimized clang asan ubsan msan test test-quiet unit-test test-sanitizers clean help
