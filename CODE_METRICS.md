# Code Metrics Analyzer - Spreadsheet-like Interface

A tool that analyzes C source files and displays various code metrics in a tabular, interactive spreadsheet-like format.

## Overview

The Code Metrics Analyzer provides a clean, spreadsheet-style view of code quality metrics for C source files. It's designed to give developers quick insights into code structure and complexity.

## Features

- **Line Count Analysis**: Total lines, code lines, blank lines, and comment lines
- **Code Structure**: Function definitions, include statements, printf statements
- **Derived Metrics**: Code-to-total ratio, comment-to-total ratio, complexity estimation
- **Spreadsheet Display**: Clean tabular format with Unicode box-drawing characters or ASCII fallback
- **Multiple Output Formats**: Unicode (default) or ASCII for compatibility
- **Standard CLI Options**: --help, --version, --plain/--ascii flags

## Building

Compile the tool using GCC with strict security flags:

```bash
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -o code_metrics code_metrics.c
```

For basic builds:

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

### Use plain ASCII output (for pipes or terminals without Unicode support)

```bash
./code_metrics --plain hello.c
./code_metrics --ascii hello.c
```

### Display help

```bash
./code_metrics --help
```

### Display version

```bash
./code_metrics --version
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
- **Estimated Complexity**: Simple size-based heuristic (NOT cyclomatic complexity)
  - Low: < 20 lines of code (simple, single-purpose)
  - Medium: 20-50 lines of code (moderate complexity)
  - High: > 50 lines of code (complex, may need refactoring)

## Output Formats

The analyzer supports two output formats:

### Unicode Format (Default)
Uses Unicode box-drawing characters (╔═╗║╠╣╚╝) for an attractive spreadsheet appearance. Best for modern terminals with UTF-8 support.

### ASCII Format (--plain or --ascii)
Uses standard ASCII characters (+|-) for maximum compatibility. Recommended when:
- Piping output to files or other programs
- Using terminals without Unicode support
- Generating machine-readable output
- Working in environments where Unicode may not render correctly

## Use Cases

1. **Code Quality Assessment**: Quickly assess the structure and documentation level of code
2. **Project Analysis**: Compare metrics across multiple files in a project
3. **Code Review**: Identify files that may need refactoring or documentation
4. **Learning Tool**: Understand the composition of different code files

## Limitations

- Simple heuristic-based analysis (not a full C parser)
- Best suited for standard C code formatting
- Function detection uses common return type patterns (int, void, char, float, double)
- Multi-line comments are counted as separate comment lines
- String literals containing comment-like patterns (e.g., "/* comment */") are handled but not perfectly
- Printf detection includes any occurrence of "printf" in code (not just function calls)

## Edge Cases and Robustness

The analyzer handles several edge cases:
- **Multi-line comments**: Properly tracks `/*` ... `*/` comment blocks
- **String literals**: Attempts to avoid counting comment markers inside strings
- **Mixed line endings**: Handles both LF and CRLF line endings
- **Long lines**: Buffer size of 1024 characters (lines longer than this are truncated)
- **File errors**: Returns non-zero exit code and error message for missing files

## Exit Codes

- **0**: Success - file analyzed and metrics displayed
- **1**: Error - file not found, cannot open, invalid options, etc.

## Security and Safety

The implementation follows secure coding practices:
- **Buffer Safety**: Fixed-size buffers with explicit size limits
  - MAX_LINE_LENGTH (1024): fgets() buffer limit
  - MAX_FILENAME (256): Filename buffer with null termination
- **String Safety**: Uses `strncpy()` with explicit null termination
- **No Dynamic Allocation**: All buffers are stack-allocated
- **Input Validation**: Checks for file open failures and returns appropriate errors
- **No Unsafe Functions**: Avoids `gets()`, `strcpy()`, `sprintf()` without bounds

## Future Enhancements

Potential improvements for more advanced spreadsheet functionality:
- CSV export for Excel/spreadsheet software
- Comparison mode (analyze multiple files side-by-side)
- Historical tracking (track metrics over time)
- Cyclomatic complexity calculation
- Interactive filtering and sorting
