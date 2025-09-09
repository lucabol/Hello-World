# Makefile for Hello World C Program
# Provides build automation for compilation, cleaning, and testing

# Compiler settings
CC = gcc
CLANG = clang
CFLAGS_BASE = -Wall -Wextra
CFLAGS_DEBUG = $(CFLAGS_BASE) -g
CFLAGS_RELEASE = $(CFLAGS_BASE) -O2
CFLAGS_STRICT = $(CFLAGS_BASE) -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion

# Source and target files
SRC = hello.c
TARGET = hello
TARGET_DEBUG = hello_debug
TARGET_CLANG = hello_clang

# Default target - development build with warnings
all: $(TARGET)

# Standard build with warnings (development build)
$(TARGET): $(SRC)
	$(CC) $(CFLAGS_BASE) -o $(TARGET) $(SRC)

# Debug build with debugging symbols
debug: $(TARGET_DEBUG)

$(TARGET_DEBUG): $(SRC)
	$(CC) $(CFLAGS_DEBUG) -o $(TARGET_DEBUG) $(SRC)

# Optimized release build
release: $(SRC)
	$(CC) $(CFLAGS_RELEASE) -o $(TARGET) $(SRC)

# Strict compilation with pedantic warnings
strict: $(SRC)
	$(CC) $(CFLAGS_STRICT) -o $(TARGET) $(SRC)

# Alternative build with Clang
clang: $(SRC)
	$(CLANG) -o $(TARGET_CLANG) $(SRC)

# Build and test target - compiles the program and executes it immediately
# Useful for quick development workflow: build + run in one command
test: $(TARGET)
	./$(TARGET)

# Clean all compiled binaries, object files, and temporary files
# Removes: executables, object files, Windows binaries, and any test artifacts
clean:
	rm -f $(TARGET) $(TARGET_DEBUG) $(TARGET_CLANG)
	rm -f hello_optimized hello_strict hello_warnings hello_test*
	rm -f *.exe *.out *.o *.obj
	rm -f core core.* *~ *.tmp

# Show usage help and available targets
# Displays all make targets with brief descriptions
help:
	@echo "Available targets:"
	@echo "  all      - Build with warnings (default)"
	@echo "  debug    - Build with debugging symbols"
	@echo "  release  - Build optimized version"
	@echo "  strict   - Build with strict/pedantic warnings"
	@echo "  clang    - Build using Clang compiler"
	@echo "  test     - Build and run the program"
	@echo "  clean    - Remove all compiled binaries"
	@echo "  help     - Show this help message"

# Declare phony targets
.PHONY: all debug release strict clang test clean help