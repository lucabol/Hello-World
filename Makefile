# Simple Makefile for hello.c with plugin support

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
PLUGIN_CFLAGS = $(CFLAGS) -fPIC -shared

# Default target - build hello without plugin support
hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

# Build hello with plugin support
hello-plugins: hello.c plugin.h
	$(CC) $(CFLAGS) -DPLUGIN_ENABLED -ldl -o hello-plugins hello.c

# Build all example plugins
plugins: plugins/uppercase.so plugins/exclamation.so

plugins/uppercase.so: plugins/uppercase.c plugin.h
	$(CC) $(PLUGIN_CFLAGS) -o plugins/uppercase.so plugins/uppercase.c

plugins/exclamation.so: plugins/exclamation.c plugin.h
	$(CC) $(PLUGIN_CFLAGS) -o plugins/exclamation.so plugins/exclamation.c

# Build everything
all: hello hello-plugins plugins

# Clean build artifacts
clean:
	rm -f hello hello-plugins plugins/*.so

# Test targets
test: hello
	./hello

test-plugins: hello-plugins plugins
	@echo "Testing without plugin:"
	./hello-plugins
	@echo
	@echo "Testing with uppercase plugin:"
	HELLO_PLUGIN=./plugins/uppercase.so ./hello-plugins
	@echo
	@echo "Testing with exclamation plugin:"
	HELLO_PLUGIN=./plugins/exclamation.so ./hello-plugins

.PHONY: all clean test test-plugins plugins