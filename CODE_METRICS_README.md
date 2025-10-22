# Code Metrics Analyzer - Spreadsheet Interface

A spreadsheet-like tool for analyzing C source code and displaying various code metrics in a tabular, interactive format.

## Overview

The Code Metrics Analyzer provides comprehensive analysis of C source files, presenting the results in a clean, spreadsheet-style tabular format. It's designed to give developers quick insights into code structure and complexity.

## Features

- **Line Statistics**: Total lines, code lines, blank lines, and comment lines
- **Character & Word Analysis**: Total characters, word count, and maximum line length
- **Code Structure**: Function count and include file count
- **Function Listing**: All detected function definitions
- **Include Files**: List of all included header files
- **Summary Metrics**: Code density percentage and average line length

## Building

### Using Make
```bash
make code_metrics
```

### Manual Build
```bash
gcc -Wall -Wextra -std=c99 -o code_metrics code_metrics.c
```

## Usage

```bash
./code_metrics <source_file>
```

### Examples

Analyze hello.c:
```bash
./code_metrics hello.c
```

Analyze the code_metrics tool itself:
```bash
./code_metrics code_metrics.c
```

## Sample Output

```
╔════════════════════════════════════════════════════════════════════╗
║          CODE METRICS ANALYZER - SPREADSHEET VIEW                  ║
╚════════════════════════════════════════════════════════════════════╝

File: hello.c

+--------------------------------------------------------------------+
|  LINE STATISTICS                                                   |
+--------------------------------------------------------------------+
| Total Lines:                                                   5 |
| Code Lines:                                                    4 |
| Blank Lines:                                                   1 |
| Comment Lines:                                                 0 |
+--------------------------------------------------------------------+

+--------------------------------------------------------------------+
|  CHARACTER & WORD STATISTICS                                       |
+--------------------------------------------------------------------+
| Total Characters:                                             63 |
| Total Words:                                                   8 |
| Max Line Length:                                              28 |
+--------------------------------------------------------------------+

+--------------------------------------------------------------------+
|  CODE STRUCTURE                                                    |
+--------------------------------------------------------------------+
| Function Count:                                                1 |
| Include Count:                                                 1 |
+--------------------------------------------------------------------+

+--------------------------------------------------------------------+
|  FUNCTIONS                                                         |
+--------------------------------------------------------------------+
|  1. main                                                            |
+--------------------------------------------------------------------+

+--------------------------------------------------------------------+
|  INCLUDE FILES                                                     |
+--------------------------------------------------------------------+
|  1. stdio.h                                                         |
+--------------------------------------------------------------------+

+--------------------------------------------------------------------+
|  SUMMARY                                                           |
+--------------------------------------------------------------------+
| Code Density:                                              80.0% |
| Avg Line Length:                                      15.8 chars |
+--------------------------------------------------------------------+
```

## Metrics Explained

### Line Statistics
- **Total Lines**: Total number of lines in the file
- **Code Lines**: Lines containing actual code (non-blank, non-comment)
- **Blank Lines**: Lines containing only whitespace
- **Comment Lines**: Lines that start with comment markers (`//` or `/*`)

### Character & Word Statistics
- **Total Characters**: Total character count in the file
- **Total Words**: Total word count across all lines
- **Max Line Length**: Length of the longest line in the file

### Code Structure
- **Function Count**: Number of function definitions detected
- **Include Count**: Number of `#include` directives

### Summary
- **Code Density**: Percentage of code lines vs. total lines
- **Avg Line Length**: Average characters per code line

## Limitations

- Function detection uses heuristics and may not catch all edge cases
- Multi-line function declarations spanning multiple lines may not be detected
- Comment analysis is basic and doesn't handle multi-line comments spanning multiple lines
- Designed primarily for C source files; may not work correctly with C++ or other languages

## Implementation Details

The tool is written in pure C with no external dependencies beyond the standard C library. It uses:
- `stdio.h` for file I/O
- `stdlib.h` for memory management
- `string.h` for string manipulation
- `ctype.h` for character classification

The analysis is performed in a single pass through the source file for efficiency.
