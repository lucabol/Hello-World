# Makefile for Hello World C Program
# Provides build automation for standard, development, and optimized builds

# Compiler settings
CC = gcc
TARGET = hello
SOURCE = hello.c

# Compiler flags for different build types
CFLAGS_STANDARD = 
CFLAGS_DEV = -Wall -Wextra
CFLAGS_OPTIMIZED = -O2 -Wall -Wextra
CFLAGS_STRICT = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion

# Reusable compilation template
# Usage: $(call COMPILE_TEMPLATE,output_file,compiler_flags)
define COMPILE_TEMPLATE
$(CC) $(2) -o $(1) $(SOURCE)
endef

# Default target (standard build)
all: $(TARGET)

# Standard build
$(TARGET): $(SOURCE)
	$(call COMPILE_TEMPLATE,$(TARGET),$(CFLAGS_STANDARD))

# Development build with warnings
dev: hello_dev
hello_dev: $(SOURCE)
	$(call COMPILE_TEMPLATE,hello_dev,$(CFLAGS_DEV))

# Optimized build
optimized: hello_optimized
hello_optimized: $(SOURCE)
	$(call COMPILE_TEMPLATE,hello_optimized,$(CFLAGS_OPTIMIZED))

# Strict compilation (maximum warnings)
strict: hello_strict
hello_strict: $(SOURCE)
	$(call COMPILE_TEMPLATE,hello_strict,$(CFLAGS_STRICT))

# Debug build
debug: hello_debug
hello_debug: $(SOURCE)
	$(call COMPILE_TEMPLATE,hello_debug,-g $(CFLAGS_DEV))

# Clean target - remove all compiled binaries
clean:
	rm -f $(TARGET) hello_dev hello_optimized hello_strict hello_debug hello_clang *.exe *.out *.o

# Test target - build and run the program
test: $(TARGET)
	./$(TARGET)

# Help target - show available targets
help:
	@echo "Available targets:"
	@echo "  all        - Standard build (default)"
	@echo "  dev        - Development build with warnings (-Wall -Wextra)"
	@echo "  optimized  - Optimized build (-O2 -Wall -Wextra)"
	@echo "  strict     - Strict compilation with maximum warnings"
	@echo "  debug      - Debug build with -g flag"
	@echo "  test       - Build and run the program"
	@echo "  clean      - Remove all compiled binaries"
	@echo "  help       - Show this help message"

# Declare phony targets
.PHONY: all dev optimized strict debug clean test help