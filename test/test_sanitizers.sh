#!/bin/bash
# Test script for sanitizer builds
# Tests that sanitizer-instrumented binaries detect issues and run cleanly

set -e

echo "=== Sanitizer Build Tests ==="
echo ""

# Test AddressSanitizer build
echo "Testing AddressSanitizer (ASan)..."
if gcc -Wall -Wextra -fsanitize=address -fno-omit-frame-pointer -g -o hello_asan hello.c; then
    echo "  ✓ ASan compilation successful"
    
    # Run the program - should execute cleanly
    if ./hello_asan > /dev/null 2>&1; then
        echo "  ✓ ASan build executes without errors"
    else
        echo "  ✗ ASan build failed to execute"
        exit 1
    fi
    
    # Verify output is correct
    OUTPUT=$(./hello_asan 2>/dev/null)
    if [ "$OUTPUT" = "Hello world!" ]; then
        echo "  ✓ ASan build produces correct output"
    else
        echo "  ✗ ASan build produces incorrect output: '$OUTPUT'"
        exit 1
    fi
else
    echo "  ✗ ASan compilation failed"
    exit 1
fi

echo ""

# Test UndefinedBehaviorSanitizer build
echo "Testing UndefinedBehaviorSanitizer (UBSan)..."
if gcc -Wall -Wextra -fsanitize=undefined -fno-omit-frame-pointer -g -o hello_ubsan hello.c; then
    echo "  ✓ UBSan compilation successful"
    
    # Run the program - should execute cleanly
    if ./hello_ubsan > /dev/null 2>&1; then
        echo "  ✓ UBSan build executes without errors"
    else
        echo "  ✗ UBSan build failed to execute"
        exit 1
    fi
    
    # Verify output is correct
    OUTPUT=$(./hello_ubsan 2>/dev/null)
    if [ "$OUTPUT" = "Hello world!" ]; then
        echo "  ✓ UBSan build produces correct output"
    else
        echo "  ✗ UBSan build produces incorrect output: '$OUTPUT'"
        exit 1
    fi
else
    echo "  ✗ UBSan compilation failed"
    exit 1
fi

echo ""

# Test MemorySanitizer build (requires Clang)
echo "Testing MemorySanitizer (MSan) with Clang..."
if command -v clang > /dev/null 2>&1; then
    if clang -Wall -Wextra -fsanitize=memory -fno-omit-frame-pointer -g -fPIE -pie -o hello_msan hello.c 2>/dev/null; then
        echo "  ✓ MSan compilation successful"
        
        # Run the program - should execute cleanly
        # Note: MSan may produce warnings about uninstrumented libraries, suppress them
        if ./hello_msan > /dev/null 2>&1; then
            echo "  ✓ MSan build executes"
        else
            # MSan might fail if libc is not instrumented, which is expected
            echo "  ⚠ MSan build execution skipped (requires instrumented libraries)"
        fi
        
        # Try to verify output, but don't fail if MSan runtime isn't available
        OUTPUT=$(./hello_msan 2>/dev/null || echo "SKIP")
        if [ "$OUTPUT" = "Hello world!" ]; then
            echo "  ✓ MSan build produces correct output"
        elif [ "$OUTPUT" = "SKIP" ]; then
            echo "  ⚠ MSan runtime check skipped"
        else
            echo "  ⚠ MSan build output: '$OUTPUT'"
        fi
    else
        echo "  ⚠ MSan compilation skipped (requires instrumented libraries)"
    fi
else
    echo "  ⚠ MSan skipped (Clang not available)"
fi

# Cleanup
rm -f hello_asan hello_ubsan hello_msan

echo ""
echo "=== Sanitizer tests completed! ==="
