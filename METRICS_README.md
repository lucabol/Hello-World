# Code Metrics Spreadsheet Tool

A spreadsheet-like interface for analyzing code metrics of C source files.

## Overview

The Code Metrics Tool analyzes C source files and displays various code metrics in a tabular, interactive format. It provides insights into code structure, size, and complexity.

## Features

- **Spreadsheet-like Display**: Presents metrics in a clear, tabular format with box-drawing characters
- **Interactive Commands**: Navigate through different views and refresh data
- **Detailed Statistics**: View comprehensive breakdowns with percentages and ratios
- **Multiple Metrics**: Tracks:
  - Total lines of code
  - Code lines (executable statements)
  - Blank lines
  - Comment lines
  - Number of functions
  - Include directives
  - Maximum line length
  - Average lines per function

## Building

```bash
gcc -Wall -Wextra -o metrics_tool metrics_tool.c code_metrics.c
```

## Usage

### Analyze hello.c (default)
```bash
./metrics_tool
```

### Analyze a specific file
```bash
./metrics_tool <filename.c>
```

## Interactive Commands

Once the tool is running, you can use these commands:

- **r** - Refresh metrics (re-analyze the file)
- **s** - Show detailed statistics with percentages
- **h** - Display help menu
- **q** - Quit the tool

## Example Output

```
════════════════════════════════════════════════════════════════════════════════
                   CODE METRICS SPREADSHEET TOOL v1.0                          
════════════════════════════════════════════════════════════════════════════════

Analyzing: hello.c

╔════════════════════════════════════════════════════════════════════════════════╗
║                         CODE METRICS SPREADSHEET                               ║
╠═══════════════╦═════════╦═════════╦═════════╦═════════╦═════════╦═════════════╣
║ Filename      ║ Total   ║ Code    ║ Blank   ║ Comment ║ Funcs   ║ Max Line    ║
║               ║ Lines   ║ Lines   ║ Lines   ║ Lines   ║         ║ Length      ║
╠═══════════════╬═════════╬═════════╬═════════╬═════════╬═════════╬═════════════╣
║ hello.c       ║       4 ║       3 ║       1 ║       0 ║       1 ║          29 ║
╚═══════════════╩═════════╩═════════╩═════════╩═════════╩═════════╩═════════════╝

Additional Metrics:
  hello.c: 1 #include directive(s)
```

## Implementation Details

### Components

1. **code_metrics.h** - Header file with data structures and function declarations
2. **code_metrics.c** - Core analysis engine that parses C files and collects metrics
3. **metrics_tool.c** - Interactive interface and display logic

### Metrics Calculation

- **Total Lines**: All lines in the file including blank lines
- **Code Lines**: Lines containing executable code (excludes comments and blank lines)
- **Blank Lines**: Lines containing only whitespace
- **Comment Lines**: Lines with single-line (//) or multi-line (/* */) comments
- **Functions**: Detected using heuristics (return type + parentheses + braces)
- **Max Line Length**: Longest line in the file (useful for style checking)

## Dependencies

- Standard C library (stdio.h, string.h, ctype.h, stdbool.h)
- No external dependencies required

## Compatibility

- Works with standard C99 and later
- Tested with GCC and Clang compilers
- Cross-platform (Linux, macOS, Windows with appropriate compiler)

## Limitations

- Function detection uses simple heuristics and may not catch all edge cases
- Designed primarily for C source files
- Does not perform deep semantic analysis (e.g., cyclomatic complexity)

## Future Enhancements

Potential improvements for future versions:
- Support for multiple file analysis in one session
- Export metrics to CSV format
- More advanced complexity metrics (cyclomatic complexity, nesting depth)
- Visual charts using ASCII art
- Configurable metric thresholds and warnings
