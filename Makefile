# Makefile for Hello-World with Voice-Driven Code Editing
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
STRICT_FLAGS = -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99

# Main targets
all: hello voice_demo

hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

voice_demo: voice_demo.c voice.c voice.h
	$(CC) $(CFLAGS) -o voice_demo voice_demo.c voice.c

# Build with strict flags (CI-like)
strict: hello.c voice_demo.c voice.c voice.h
	$(CC) $(STRICT_FLAGS) -o hello_strict hello.c
	$(CC) $(STRICT_FLAGS) -o voice_demo_strict voice_demo.c voice.c

# Alternative compiler support
clang: hello.c
	clang $(CFLAGS) -o hello_clang hello.c

# Debug builds
debug: hello.c voice_demo.c voice.c
	$(CC) $(CFLAGS) -g -o hello_debug hello.c
	$(CC) $(CFLAGS) -g -o voice_demo_debug voice_demo.c voice.c

# Test targets
test: all test_voice
	./test_voice
	./test/validate.sh

test-quiet: all test_voice
	./test_voice > /dev/null
	./test/validate.sh --quiet

# Unit tests for voice parsing
test_voice: test_voice.c voice.c voice.h
	$(CC) $(CFLAGS) -o test_voice test_voice.c voice.c

# Demo target
demo: voice_demo
	@echo "Running voice-driven code editing demo..."
	./voice_demo

# Clean up build artifacts
clean:
	rm -f hello hello_debug hello_clang hello_strict voice_demo voice_demo_debug voice_demo_strict voice_demo_test test_voice *.exe *.out *.o

# Install/setup (placeholder for future extensions)
install:
	@echo "Voice-driven code editing system is ready to use!"
	@echo "Run 'make demo' to see it in action."

.PHONY: all strict clang debug test test-quiet demo clean install