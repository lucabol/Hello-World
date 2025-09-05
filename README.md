# Hello World with Code Metrics Spreadsheet

This repository contains a simple C "Hello World" program along with an advanced **Code Metrics Spreadsheet Interface** for analyzing C source files.

## Quick Start

### Basic Hello World Program
```bash
gcc -o hello hello.c
./hello
```

### Code Metrics Spreadsheet Interface
```bash
gcc -o metrics metrics.c
./metrics hello.c
```

## Features

### 📊 Code Metrics Analyzer
The `metrics.c` program provides a comprehensive spreadsheet-like interface for analyzing C source files:

- **Interactive Menu System**: Choose from multiple viewing options
- **Tabular Display**: Beautiful spreadsheet-style output with borders
- **Multiple Export Formats**: Summary statistics, individual metrics, CSV export
- **Real-time Analysis**: Analyzes any C file specified

### 📈 Metrics Collected
- **Total Lines**: Complete line count in the file
- **Code Lines**: Lines containing actual executable code
- **Blank Lines**: Empty or whitespace-only lines  
- **Comments**: Lines with comments (// or /* style)
- **Functions**: Number of function definitions detected
- **Includes**: Number of #include directives
- **File Size**: Size in bytes

### 🎯 Interactive Features
1. **Full Spreadsheet View**: Complete metrics table with descriptions
2. **Summary Statistics**: Quick overview of all metrics
3. **Individual Metric Details**: Drill down into specific metrics
4. **CSV Export**: Machine-readable format for external tools
5. **Multi-file Support**: Analyze any C file via command line

## Usage Examples

### Using Makefile
```bash
make all     # Build both programs
make demo    # Run metrics demo on hello.c
make test    # Test both programs
make clean   # Remove compiled binaries
```

### Manual Usage
```bash
# Analyze the hello.c file
./metrics hello.c

# Analyze the metrics program itself  
./metrics metrics.c

# Analyze any other C file
./metrics /path/to/yourfile.c
```

### Sample Output
```
╔══════════════════════════════════════════════════════════════════════════════╗
║                            CODE METRICS SPREADSHEET                         ║
║                                File: hello.c                             ║
╠══════════════════╤═══════════╤═══════════════════════════════════════════════╣
║ Metric           │   Value   │ Description                               ║
╠══════════════════╪═══════════╪═══════════════════════════════════════════════╣
║ Total Lines      │         5 │ Total number of lines in file             ║
║ Code Lines       │         4 │ Lines containing actual code              ║
║ Blank Lines      │         1 │ Empty or whitespace-only lines            ║
║ Comments         │         0 │ Lines containing comments                 ║
║ Functions        │         1 │ Number of function definitions            ║
║ Includes         │         1 │ Number of #include directives             ║
║ File Size        │        63 │ File size in bytes                        ║
╚══════════════════╧═══════════╧═══════════════════════════════════════════════╝
```

## Building

### Requirements
- GCC compiler
- Standard C library

### Build Commands
```bash
# Build everything
make all

# Build individual components
gcc -Wall -Wextra -o hello hello.c
gcc -Wall -Wextra -o metrics metrics.c
```

## Repository Structure
```
.
├── hello.c        # Original Hello World program
├── metrics.c      # Code Metrics Spreadsheet Interface
├── Makefile       # Build automation
├── README.md      # This documentation
└── .gitignore     # Excludes compiled binaries
```