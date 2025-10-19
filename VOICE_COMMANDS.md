# Voice-Driven Code Editing

This repository now supports voice-driven code editing for `hello.c`.

## Features

- **Voice command parsing**: Interpret natural language commands for code operations
- **Read-only operations**: All commands are safe and don't modify source files
- **Interactive demo**: See all commands in action

## Building

```bash
gcc -o voice_demo voice_demo.c voice.c
```

## Usage

### Interactive Demo
Run the demo to see all voice commands in action:
```bash
./voice_demo
```

### Single Command Execution
Execute a specific voice command:
```bash
./voice_demo "say hello"
./voice_demo "change message Custom greeting!"
./voice_demo "show code"
```

## Supported Commands

### `say hello`
Executes the hello program and displays its output.

**Example:**
```bash
./voice_demo "say hello"
```
**Output:**
```
Hello world!
```

### `change message <text>`
Simulates changing the greeting message (read-only demonstration).

**Example:**
```bash
./voice_demo "change message Greetings, World!"
```
**Output:**
```
Would change message to: Greetings, World!
(Simulation mode - no actual file changes)
```

### `show code`
Displays the current code structure and information.

**Example:**
```bash
./voice_demo "show code"
```
**Output:**
```
Showing current code structure:
  - Function: main()
  - Output: "Hello world!\n"
  - Lines: 5
```

## Error Handling

Invalid commands are handled gracefully:
```bash
./voice_demo "invalid command"
```
**Output:**
```
Voice command not recognized: invalid command
```
**Exit code:** 1

## API

See `voice.h` for the API documentation:
- `int process_voice_command(const char* command)` - Process a voice command string

## Testing

Run the validation script to test all voice command functionality:
```bash
./test/validate.sh
```

This will verify:
- Voice command parsing
- Command execution
- Error handling
- File integrity (no modifications during testing)

## Implementation Details

- **voice.h**: Public API for voice command processing
- **voice.c**: Command parser and handler implementation
- **voice_demo.c**: Demo application showcasing voice commands
- All operations are read-only to ensure safety
- Commands return 0 on success, 1 on error
