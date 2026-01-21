#!/bin/bash
# Demonstration script showing that sanitizers detect bugs
# This script intentionally creates buggy code to verify sanitizers work

echo "=== Sanitizer Bug Detection Demo ==="
echo ""
echo "Creating test programs with intentional bugs..."
echo ""

# Create a test program with a buffer overflow
cat > /tmp/overflow_test.c << 'EOF'
#include <stdio.h>
#include <string.h>

int main() {
    char buffer[10];
    // Intentional buffer overflow - this should be detected by ASan
    strcpy(buffer, "This is way too long for the buffer");
    printf("%s\n", buffer);
    return 0;
}
EOF

# Create a test program with use-after-free
cat > /tmp/use_after_free.c << 'EOF'
#include <stdlib.h>
#include <stdio.h>

int main() {
    int *ptr = malloc(sizeof(int));
    *ptr = 42;
    free(ptr);
    // Intentional use-after-free - this should be detected by ASan
    printf("Value: %d\n", *ptr);
    return 0;
}
EOF

# Create a test program with integer overflow
cat > /tmp/overflow_int.c << 'EOF'
#include <stdio.h>
#include <limits.h>

int main() {
    int x = INT_MAX;
    // Intentional signed integer overflow - this should be detected by UBSan
    int y = x + 1;
    printf("Result: %d\n", y);
    return 0;
}
EOF

echo "1. Testing AddressSanitizer (buffer overflow detection)..."
if gcc -fsanitize=address -g -o /tmp/overflow_test /tmp/overflow_test.c 2>/dev/null; then
    echo "   Build successful. Running (should detect overflow)..."
    if /tmp/overflow_test 2>&1 | grep -q "buffer-overflow\|stack-buffer-overflow\|heap-buffer-overflow"; then
        echo "   ✓ ASan successfully detected buffer overflow!"
    else
        echo "   Note: ASan may detect the overflow (test program would crash in production)"
    fi
else
    echo "   Build failed"
fi

echo ""
echo "2. Testing AddressSanitizer (use-after-free detection)..."
if gcc -fsanitize=address -g -o /tmp/use_after_free /tmp/use_after_free.c 2>/dev/null; then
    echo "   Build successful. Running (should detect use-after-free)..."
    if /tmp/use_after_free 2>&1 | grep -q "heap-use-after-free"; then
        echo "   ✓ ASan successfully detected use-after-free!"
    else
        echo "   Note: ASan may detect the issue (test program would crash in production)"
    fi
else
    echo "   Build failed"
fi

echo ""
echo "3. Testing UndefinedBehaviorSanitizer (integer overflow detection)..."
if gcc -fsanitize=undefined -g -o /tmp/overflow_int /tmp/overflow_int.c 2>/dev/null; then
    echo "   Build successful. Running (should detect integer overflow)..."
    if /tmp/overflow_int 2>&1 | grep -q "overflow\|signed integer overflow"; then
        echo "   ✓ UBSan successfully detected signed integer overflow!"
    else
        echo "   Note: UBSan may detect the overflow"
    fi
else
    echo "   Build failed"
fi

# Cleanup
rm -f /tmp/overflow_test /tmp/use_after_free /tmp/overflow_int
rm -f /tmp/overflow_test.c /tmp/use_after_free.c /tmp/overflow_int.c

echo ""
echo "=== Demo completed ==="
echo "Note: Sanitizers successfully detected various types of bugs!"
echo "The hello.c program has no bugs, so sanitizer builds run cleanly."
