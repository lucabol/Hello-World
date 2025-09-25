# Makefile for Hello World C program
# Provides standard build targets as documented in README

# Compiler settings
CC = gcc
CLANG = clang
SOURCE = hello.c
PLUGIN_SOURCES = plugin.c sample_plugins.c

# Default target - optimized build (no plugins)
all: hello

hello: $(SOURCE)
	$(CC) -O2 -Wall -Wextra -o hello $(SOURCE)

# Plugin-enabled build
hello-plugins: $(SOURCE) $(PLUGIN_SOURCES)
	$(CC) -O2 -Wall -Wextra -DENABLE_PLUGINS -o hello-plugins $(SOURCE) $(PLUGIN_SOURCES)

# Strict compilation with error on warnings
strict: hello_strict

hello_strict: $(SOURCE)
	$(CC) -Wall -Wextra -Wpedantic -Werror -std=c99 -o hello_strict $(SOURCE)

# Plugin-enabled strict build  
strict-plugins: hello_strict_plugins

hello_strict_plugins: $(SOURCE) $(PLUGIN_SOURCES)
	$(CC) -Wall -Wextra -Wpedantic -Werror -std=c99 -DENABLE_PLUGINS -o hello_strict_plugins $(SOURCE) $(PLUGIN_SOURCES)

# Clang build
clang: hello_clang

hello_clang: $(SOURCE)
	$(CLANG) -Wall -Wextra -o hello_clang $(SOURCE)

# Clang build with plugins
clang-plugins: hello_clang_plugins

hello_clang_plugins: $(SOURCE) $(PLUGIN_SOURCES)  
	$(CLANG) -Wall -Wextra -DENABLE_PLUGINS -o hello_clang_plugins $(SOURCE) $(PLUGIN_SOURCES)

# Debug build
debug: hello_debug

hello_debug: $(SOURCE)
	$(CC) -g -Wall -Wextra -o hello_debug $(SOURCE)

# Debug build with plugins
debug-plugins: hello_debug_plugins

hello_debug_plugins: $(SOURCE) $(PLUGIN_SOURCES)
	$(CC) -g -Wall -Wextra -DENABLE_PLUGINS -o hello_debug_plugins $(SOURCE) $(PLUGIN_SOURCES)

# Test target - run validation script
test:
	bash test/validate.sh

# Test plugin functionality
test-plugins: hello-plugins
	@echo "Testing plugin-enabled build..."
	@echo "Expected: Plugin system should transform output"
	@echo "Actual output:"
	@./hello-plugins
	@echo "Plugin test completed"

# Clean build artifacts
clean:
	rm -f hello hello_strict hello_clang hello_debug *.exe *.out *.o
	rm -f hello-plugins hello_strict_plugins hello_clang_plugins hello_debug_plugins

# Mark targets that don't create files
.PHONY: all strict clang debug test test-plugins clean hello-plugins strict-plugins clang-plugins debug-plugins