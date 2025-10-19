# Voice-Driven Code Editing for hello.c

This repository includes a demonstration of voice-driven code editing capabilities that allow users to interact with and modify the hello.c program using natural language voice commands.

## Overview

The voice-driven code editing system provides a safe, interactive way to:
- Execute code and view output
- Navigate code structure
- Simulate code modifications (with user confirmation for actual changes)

## Architecture

The implementation consists of three main components:

### 1. Core Components

- **hello.h** - Header file exposing the `get_greeting()` API
- **hello.c** - Main program with refactored greeting functionality
- **voice.h** - Voice command interface definition
- **voice.c** - Voice command processing implementation
- **voice_demo.c** - Interactive demonstration program

### 2. Supported Voice Commands

#### "say hello"
Executes the hello program and displays its output.

```bash
./voice_demo "say hello"
# Output: Hello world!
```

#### "change message <new_text>"
Simulates changing the greeting message. This is a safe demonstration that shows what would happen without actually modifying files.

```bash
./voice_demo "change message Greetings, everyone!"
# Output: Would change message to: Greetings, everyone!
#         (This is a simulation - actual file modification requires user confirmation)
```

#### "show code"
Displays information about the current code structure, including functions and the current greeting message.

```bash
./voice_demo "show code"
# Output: Showing current code structure:
#           - File: hello.c
#           - Function: main()
#           - Function: get_greeting()
#           - Current message: "Hello world!"
```

### 3. Error Handling

The system gracefully handles unrecognized commands and provides helpful feedback:

```bash
./voice_demo "invalid command"
# Output: Voice command not recognized: invalid command
#         Available commands:
#           - say hello
#           - change message <new_text>
#           - show code
# Exit code: 1
```

## Building and Running

### Prerequisites
- GCC or Clang compiler
- Make (optional, but recommended)

### Build Commands

```bash
# Build the main hello program
make

# Build the voice demo
make voice-demo

# Build everything
make all
```

### Running the Voice Demo

#### Interactive Mode
Run without arguments to see a full demonstration of all voice commands:

```bash
./voice_demo
```

#### Command Mode
Pass a voice command as an argument:

```bash
./voice_demo "say hello"
./voice_demo "change message New greeting"
./voice_demo "show code"
```

## Testing

The voice command functionality is thoroughly tested as part of the CI pipeline:

```bash
# Run all tests including voice command tests
make test

# Run just the unit tests
make unit-test

# Run validation tests
./test/validate.sh
```

### Test Coverage

The validation script (`test/validate.sh`) includes comprehensive tests for:
- Voice command parsing and execution
- All supported commands
- Error handling for invalid commands
- Verification that no source files are modified during testing
- Interactive demo mode

## Design Principles

### 1. Safety First
Voice commands that could modify files only simulate the changes and clearly indicate that user confirmation is required for actual modifications. This prevents accidental changes while demonstrating functionality.

### 2. Self-Contained
The voice command system can be compiled and tested independently of the main hello program, making it easy to validate and extend.

### 3. Minimal Dependencies
The implementation uses only standard C library functions (`stdio.h`, `string.h`) with no external dependencies.

### 4. Clear Feedback
Every command provides clear, actionable output that helps users understand what happened and what options are available.

## Future Enhancements

Potential improvements to the voice command system could include:

1. **Actual File Modification**: Add interactive confirmation prompts for real code changes
2. **More Commands**: Support for additional operations like:
   - Code navigation (go to function, go to line)
   - Refactoring operations (rename, extract function)
   - Code insertion and deletion
3. **Voice Input Integration**: Connect to actual speech recognition systems
4. **Undo/Redo**: Implement command history and reversal
5. **Multi-File Support**: Extend to work with larger codebases

## Technical Details

### API Design

The `process_voice_command()` function in `voice.c` follows a simple interface:

```c
int process_voice_command(const char* command);
```

- **Parameters**: A null-terminated string containing the voice command
- **Returns**: `VOICE_CMD_SUCCESS` (0) for recognized commands, `VOICE_CMD_ERROR` (1) for unrecognized commands
- **Thread Safety**: Not thread-safe (uses static storage in `get_greeting()`)

### Command Parsing

Commands are parsed using simple string comparison and prefix matching:
- Exact match for fixed commands (`"say hello"`, `"show code"`)
- Prefix match for parameterized commands (`"change message "` followed by text)

This simple approach ensures predictable behavior and easy testing.

## Contributing

When adding new voice commands:

1. Update `voice.c` with the new command handler
2. Add tests to `test/validate.sh` (lines 146-292)
3. Update this documentation
4. Ensure all existing tests still pass

## License

This code is part of the Hello World repository and follows the same license terms.
