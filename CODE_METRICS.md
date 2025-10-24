# Code Metrics Analyzer - Spreadsheet-like Interface

A tool that analyzes C source files and displays various code metrics in a tabular, interactive spreadsheet-like format.

## Overview

The Code Metrics Analyzer provides a clean, spreadsheet-style view of code quality metrics for C source files. It's designed to give developers quick insights into code structure and complexity.

## Features

- **Line Count Analysis**: Total lines, code lines, blank lines, and comment lines
- **Code Structure**: Function definitions, include statements, printf statements
- **Derived Metrics**: Code-to-total ratio, comment-to-total ratio, complexity estimation
- **Spreadsheet Display**: Clean tabular format with Unicode box-drawing characters

## Building

Compile the tool using GCC:

```bash
gcc -Wall -Wextra -o code_metrics code_metrics.c
```

## Usage

### Analyze hello.c (default)

```bash
./code_metrics
```

### Analyze a specific file

```bash
./code_metrics filename.c
```

### Display help

```bash
./code_metrics --help
```

## Example Output

```
╔════════════════════════════════════════════════════════════════╗
║          CODE METRICS ANALYZER - SPREADSHEET VIEW              ║
╠════════════════════════════════════════════════════════════════╣
║ File: hello.c                                                  ║
╠════════════════════════════════════════╦═══════════════════════╣
║ Metric                                 ║ Value                 ║
╠════════════════════════════════════════╬═══════════════════════╣
║ Total Lines                            ║ 5                     ║
║ Code Lines (executable)                ║ 4                     ║
║ Blank Lines                            ║ 1                     ║
║ Comment Lines                          ║ 0                     ║
║ Function Definitions                   ║ 1                     ║
║ Include Statements                     ║ 1                     ║
║ Printf Statements                      ║ 1                     ║
║ Maximum Line Length                    ║ 28                    ║
╠════════════════════════════════════════╩═══════════════════════╣
║ DERIVED METRICS:                                               ║
║ Code-to-Total Ratio                    ║ 80.00%                 ║
║ Comment-to-Total Ratio                 ║ 0.00%                 ║
║ Estimated Complexity                   ║ Low                   ║
╚════════════════════════════════════════╩═══════════════════════╝
```

## Metrics Explained

### Basic Metrics

- **Total Lines**: Total number of lines in the file
- **Code Lines**: Lines containing executable code (non-blank, non-comment)
- **Blank Lines**: Empty lines or lines with only whitespace
- **Comment Lines**: Lines containing comments (// or /* */ style)
- **Function Definitions**: Number of function definitions detected
- **Include Statements**: Number of #include directives
- **Printf Statements**: Number of printf() calls (useful for output tracking)
- **Maximum Line Length**: Length of the longest line in characters

### Derived Metrics

- **Code-to-Total Ratio**: Percentage of lines that are executable code
- **Comment-to-Total Ratio**: Percentage of lines that are comments
- **Estimated Complexity**: 
  - Low: < 20 lines of code
  - Medium: 20-50 lines of code
  - High: > 50 lines of code

## Use Cases

1. **Code Quality Assessment**: Quickly assess the structure and documentation level of code
2. **Project Analysis**: Compare metrics across multiple files in a project
3. **Code Review**: Identify files that may need refactoring or documentation
4. **Learning Tool**: Understand the composition of different code files

## Limitations

- Simple heuristic-based analysis (not a full parser)
- Best suited for standard C code formatting
- Function detection uses common patterns (int, void, char, float, double)
- Multi-line comments are counted as separate comment lines

## Future Enhancements

Potential improvements for more advanced spreadsheet functionality:
- CSV export for Excel/spreadsheet software
- Comparison mode (analyze multiple files side-by-side)
- Historical tracking (track metrics over time)
- Cyclomatic complexity calculation
- Interactive filtering and sorting
