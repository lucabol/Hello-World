# Hello World
Test repo for JediMaster

## Features

### Hello World Program
A simple C program that prints "Hello world!"

### Code Metrics Analyzer
An interactive spreadsheet-like tool that analyzes C source files and displays code metrics in a tabular format.

## Building and Running

### Build everything:
```bash
make
```

### Build individual components:
```bash
make hello    # Build hello program only
make metrics  # Build metrics analyzer only
```

### Run the hello program:
```bash
./hello
```

### Run the metrics analyzer:
```bash
./metrics [filename]
```
If no filename is provided, it defaults to analyzing `hello.c`.

### Demo the metrics analyzer:
```bash
./demo_metrics.sh
```

## Metrics Analyzer Features

The metrics analyzer provides an interactive, spreadsheet-like interface with three views:

1. **Summary View**: Overall file statistics including:
   - Total lines, code lines, blank lines, comment lines
   - Function count, include statements count
   - File size and character count

2. **Functions View**: Lists all detected functions with line numbers

3. **Includes View**: Lists all include statements with line numbers

### Navigation:
- **TAB**: Switch between views
- **Arrow Keys**: Navigate within lists
- **Q**: Quit the analyzer

## Clean up:
```bash
make clean
```