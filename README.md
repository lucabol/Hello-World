# Hello World with Voice-Driven Code Editing

Test repo for JediMaster featuring voice-controlled code editing capabilities.

## Features

### Traditional Hello World Program
- Simple C program that outputs "Hello world!" (no trailing newline)
- Standard compilation and execution with zero exit code
- Maintains traditional Hello World behavior for backward compatibility

### Voice-Driven Code Editing System
- Parse and execute voice commands for code manipulation
- **Safety-first design**: Commands default to demo mode to prevent accidental file modification
- Support for code insertion, navigation, and refactoring operations
- Interactive demo showcasing voice command capabilities
- Comprehensive unit test suite with memory safety validation

## Quick Start

### Building
```bash
make all          # Build both hello and voice_demo
make hello        # Build just the hello program
make voice_demo   # Build just the voice demo
make strict       # Build with strict compilation flags (-Werror, -Wpedantic)
```

### Running
```bash
./hello           # Run the hello program
make demo         # Run voice-driven editing demo (safe mode)
./voice_demo --help  # Show usage and safety information
```

## Voice Commands

### Safety Modes
- `--demo` (default): Safe demonstration mode - shows intended actions without modifying files
- `--apply`: Apply mode - would actually modify files (requires explicit flag, not fully implemented)

### Supported Commands
```bash
# Basic usage (demo mode - safe)
./voice_demo "say hello"                    # Execute hello program
./voice_demo "show code"                    # Display code structure with line numbers
./voice_demo "change message New text"      # Show what message change would do
./voice_demo "navigate to main"             # Navigate to main function
./voice_demo "insert code // Comment"       # Show where code would be inserted
./voice_demo "refactor function main"       # Show what refactoring would do

# With explicit apply mode (shows warnings)
./voice_demo --apply "change message New text"   # Warns about file modification
```

### Interactive Demo
```bash
./voice_demo        # Run full interactive demonstration
```

## Safety & Quality Assurance

### Memory Safety
- Bounds-checked string operations using `strncpy` instead of `strcpy`
- Proper memory allocation and deallocation with cleanup functions
- Comprehensive memory management unit tests

### Testing
```bash
make test         # Run all tests: unit tests + validation tests
make test-quiet   # Run tests with minimal output
./test_voice      # Run unit tests only
```

### Code Quality
- Strict compilation with `-Wall -Wextra -Wpedantic -Werror`
- Compatible with both GCC and Clang compilers
- Comprehensive error handling for edge cases
- Zero memory leaks verified through testing

## File Safety

The voice-driven editing system is designed with safety as the primary concern:

1. **Demo Mode Default**: All commands default to demonstration mode
2. **Explicit Apply Flag**: File modifications require explicit `--apply` flag
3. **Apply Mode Warnings**: Clear warnings when destructive operations are requested
4. **File Modification Detection**: Tests verify no unintended file changes occur
5. **Future Implementation**: Current apply mode shows intended actions without actual file modification

## Dependencies and Compatibility

### Required Tools
- **GCC or Clang**: C99-compatible compiler
- **GNU Make**: For build automation
- **POSIX**: Standard POSIX environment for shell scripts

### No External Dependencies
- Uses only standard C library functions
- Self-contained voice parsing and execution system
- No package managers or external libraries required

## Development

### Project Structure
```
.
├── hello.c           # Traditional Hello World program
├── voice.h           # Voice command system API
├── voice.c           # Voice command parser and executor
├── voice_demo.c      # Interactive demonstration application
├── test_voice.c      # Comprehensive unit test suite
├── test/validate.sh  # Integration and regression tests
├── Makefile          # Build system with multiple targets
└── README.md         # This documentation
```

### Build Targets
```bash
make all         # Build main programs
make debug       # Build with debug symbols
make strict      # Build with strict error checking
make test        # Run complete test suite
make clean       # Remove all build artifacts
```

This implementation provides a robust foundation for voice-controlled code editing while maintaining complete safety through demonstration mode and comprehensive testing.