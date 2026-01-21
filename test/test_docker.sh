#!/bin/bash
# Test script to verify Docker build consistency
# Ensures the containerized environment produces expected results

set -e

echo "=== Docker Build Consistency Test ==="
echo ""

# Test 1: Verify Docker image builds successfully
echo "Test 1: Building Docker image..."
docker build -t hello-world-c:test . > /dev/null 2>&1
echo "✓ Docker image built successfully"
echo ""

# Test 2: Verify GCC version in container
echo "Test 2: Verifying GCC version..."
GCC_VERSION=$(docker run --rm hello-world-c:test gcc --version | head -1)
echo "GCC Version: $GCC_VERSION"
if [[ "$GCC_VERSION" == *"13.3.0"* ]]; then
    echo "✓ GCC version matches expected (13.3.0)"
else
    echo "✗ GCC version mismatch"
    exit 1
fi
echo ""

# Test 3: Verify Clang version in container
echo "Test 3: Verifying Clang version..."
CLANG_VERSION=$(docker run --rm hello-world-c:test clang --version | head -1)
echo "Clang Version: $CLANG_VERSION"
if [[ "$CLANG_VERSION" == *"18.1.3"* ]]; then
    echo "✓ Clang version matches expected (18.1.3)"
else
    echo "✗ Clang version mismatch"
    exit 1
fi
echo ""

# Test 4: Verify program output
echo "Test 4: Verifying program output..."
OUTPUT=$(docker run --rm hello-world-c:test)
if [ "$OUTPUT" = "Hello world!" ]; then
    echo "✓ Program output is correct: '$OUTPUT'"
else
    echo "✗ Program output is incorrect. Expected: 'Hello world!' Got: '$OUTPUT'"
    exit 1
fi
echo ""

# Test 5: Verify program built with GCC in container
echo "Test 5: Verifying GCC build in container..."
GCC_OUTPUT=$(docker run --rm hello-world-c:test bash -c "gcc -Wall -Wextra -o hello_gcc hello.c && ./hello_gcc")
if [ "$GCC_OUTPUT" = "Hello world!" ]; then
    echo "✓ GCC build works correctly"
else
    echo "✗ GCC build failed or output is incorrect"
    exit 1
fi
echo ""

# Test 6: Verify program built with Clang in container
echo "Test 6: Verifying Clang build in container..."
CLANG_OUTPUT=$(docker run --rm hello-world-c:test bash -c "clang -Wall -Wextra -o hello_clang hello.c && ./hello_clang")
if [ "$CLANG_OUTPUT" = "Hello world!" ]; then
    echo "✓ Clang build works correctly"
else
    echo "✗ Clang build failed or output is incorrect"
    exit 1
fi
echo ""

# Test 7: Verify docker-compose works
echo "Test 7: Verifying docker-compose test service..."
COMPOSE_OUTPUT=$(docker compose run --rm test 2>/dev/null)
if [[ "$COMPOSE_OUTPUT" == *"Hello world!"* ]]; then
    echo "✓ docker-compose test service works"
else
    echo "✗ docker-compose test service failed"
    exit 1
fi
echo ""

echo "=== All Docker consistency tests passed! ==="
