# Hello World
Test repo for JediMaster

## Features

- Simple C "Hello World" program
- **Voice-driven code editing** - Interactive voice command interface for code navigation and editing
- Comprehensive unit testing with custom test framework
- CI/CD integration with GitHub Actions

## Quick Start

```bash
# Build and run the main program
make
./hello

# Try voice-driven code editing
make voice-demo
./voice_demo "say hello"
./voice_demo "show code"
./voice_demo "change message Greetings!"
```

## Voice Commands

This repository includes a demonstration of voice-driven code editing. See [VOICE_COMMANDS.md](VOICE_COMMANDS.md) for detailed documentation.

Supported commands:
- `say hello` - Execute and display program output
- `change message <text>` - Simulate changing the greeting (safe demo mode)
- `show code` - Display code structure information

## Testing

```bash
# Run all tests
make test

# Run unit tests
make unit-test
```

For more information, see the full documentation in [VOICE_COMMANDS.md](VOICE_COMMANDS.md).