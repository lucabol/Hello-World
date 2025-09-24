# Hello World
Test repo for JediMaster

A simple C "Hello World" program that demonstrates basic C programming concepts and includes a web-based visual block editor.

## Quick Start

### Using the Visual Block Editor

Experience drag-and-drop C programming with our web-based visual editor:

```bash
# Start the visual editor server
python3 server.py

# Open http://localhost:8080 in your browser
# Drag blocks to create your C program
# Export the result to hello.c
```

See [VISUAL_EDITOR.md](VISUAL_EDITOR.md) for detailed instructions.

### Traditional C Development

You can also work directly with the C source code:

```bash
# Basic build
gcc -o hello hello.c

# Run the program
./hello
```

## Expected Output

When you run the program (whether built traditionally or from the visual editor), it will produce:

```
Hello world!
```

**Note:** The program outputs "Hello world!" without a trailing newline and exits with code 0.