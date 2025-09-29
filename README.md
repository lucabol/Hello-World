# Hello World with Voice-Driven Code Editing

Test repo for JediMaster featuring voice-controlled code editing capabilities.

## Features

### Traditional Hello World Program
- Simple C program that outputs "Hello world!" and exit code
- Standard compilation and execution

### Voice-Driven Code Editing System
- Parse and execute voice commands for code manipulation
- Support for code insertion, navigation, and refactoring
- Interactive demo showcasing voice command capabilities

## Quick Start

### Building
```bash
make all          # Build both hello and voice_demo
make hello        # Build just the hello program
make voice_demo   # Build just the voice demo
```

### Running
```bash
./hello           # Run the hello program
make demo         # Run voice-driven editing demo
```

### Voice Commands
```bash
./voice_demo "say hello"                    # Execute hello program
./voice_demo "show code"                    # Display code structure
./voice_demo "change message New text"      # Change greeting message
./voice_demo "navigate to main"             # Navigate to main function
./voice_demo "insert code // Comment"       # Insert code
./voice_demo "refactor function main"       # Refactor code elements
```

## Testing
```bash
make test         # Run all validation tests
make test-quiet   # Run tests with minimal output
```