# Makefile for Hello World with Plugin System
# 
# Targets:
#   all          - Build the main program (without plugin support by default)
#   hello        - Build the main program without plugin system
#   hello-plugin - Build the main program with plugin system support
#   plugins      - Build all example plugins
#   test         - Run validation tests
#   clean        - Remove built files

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c99
PLUGIN_CFLAGS = -shared -fPIC

# Default target - build basic hello program without plugins
all: hello

# Build basic hello program (no plugin support, minimal)
hello: hello.c hello.h
	$(CC) -o hello hello.c

# Build hello program with plugin system support
hello-plugin: hello.c plugin_loader.c plugin_loader.h plugin_api.h hello.h
	$(CC) $(CFLAGS) -DWITH_PLUGINS -o hello-plugin hello.c plugin_loader.c -ldl

# Build all example plugins
plugins: plugins/uppercase_plugin.so plugins/bracket_plugin.so plugins/reverse_plugin.so

plugins/uppercase_plugin.so: plugins/uppercase_plugin.c plugin_api.h
	$(CC) $(PLUGIN_CFLAGS) -o plugins/uppercase_plugin.so plugins/uppercase_plugin.c

plugins/bracket_plugin.so: plugins/bracket_plugin.c plugin_api.h
	$(CC) $(PLUGIN_CFLAGS) -o plugins/bracket_plugin.so plugins/bracket_plugin.c

plugins/reverse_plugin.so: plugins/reverse_plugin.c plugin_api.h
	$(CC) $(PLUGIN_CFLAGS) -o plugins/reverse_plugin.so plugins/reverse_plugin.c

# Build for strict validation (used by CI)
strict: hello.c hello.h
	$(CC) $(CFLAGS) -Wformat=2 -Wconversion -Wsign-conversion -o hello hello.c

# Run validation tests
test: hello
	./test/validate.sh

# Run unit tests (requires hello.h)
test-unit: hello.h
	./test/run_unit_tests.sh

# Clean all built files
clean:
	rm -f hello hello-plugin hello_strict
	rm -f plugins/*.so
	rm -f *.o test_hello_runner

.PHONY: all clean test test-unit plugins strict hello hello-plugin
