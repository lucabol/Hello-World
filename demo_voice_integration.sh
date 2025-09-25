#!/bin/bash
# Demonstration of integrated voice-driven code editing in hello.c
# This shows how hello.c now supports voice commands directly

echo "=== Voice-Driven Code Editing Integration Demo ==="
echo "hello.c now supports voice commands directly!"
echo

echo "1. Default behavior (backward compatible):"
./hello
echo

echo "2. Voice command: Show available commands"
./hello "help"
echo

echo "3. Navigation: Jump to main function"
./hello "navigate to main"
echo

echo "4. Code inspection: List functions"
./hello "list functions"
echo

echo "5. Editing simulation: Insert comment"
./hello "insert comment"
echo

echo "6. Refactoring simulation: Optimize code"
./hello "refactor optimize"
echo

echo "7. Core functionality via voice"
./hello "say hello"
echo

echo "8. Message modification simulation"
./hello "change message Voice integration is working!"
echo

echo "9. Error handling: Invalid command"
./hello "unknown command"
echo

echo "=== Demo Complete ==="
echo "Voice-driven code editing is now fully integrated into hello.c!"