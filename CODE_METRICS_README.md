# Code Metrics Analyzer - Spreadsheet Interface

<!--
SPDX-License-Identifier: MIT
-->

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

### Build with Strict Warnings (for contributors)
The code is designed to compile cleanly with very strict compiler warnings:
```bash
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -o code_metrics code_metrics.c
```

**Compiler Compatibility:**
- Tested with GCC 9.0+ and Clang 10.0+
- Uses C99 standard features
- All strict warnings are treated as errors in CI
- If you encounter build issues, please report them with your compiler version

**Note for Contributors:** If you modify the code, ensure it compiles with the strict flags above. The CI system validates this on every PR.

## Usage

```bash
./code_metrics [OPTIONS] <source_file>
```

### Command-Line Options

- `-h, --help`: Display help message and exit
- `-v, --version`: Display version information and exit

### Examples

Analyze hello.c:
```bash
./code_metrics hello.c
```

Analyze the code_metrics tool itself:
```bash
./code_metrics code_metrics.c
```

Get help:
```bash
./code_metrics --help
```

### Exit Codes

The tool uses standard exit codes:
- **0**: Success - file analyzed without errors
- **1**: Error - file not found, parse error, invalid arguments, or exceeded limits

Error messages are printed to stderr, while metrics output goes to stdout. This allows you to separate errors from data in scripts:
```bash
./code_metrics myfile.c 2>errors.log >metrics.txt
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

### Configuration Limits

The tool has the following hard-coded limits (defined in code_metrics.c):
- **MAX_LINE_LENGTH**: 1024 characters per line
- **MAX_FUNCTIONS**: 100 function definitions tracked
- **MAX_INCLUDES**: 50 include directives tracked

**Behavior when limits are exceeded:**
- Long lines (>1024 chars): Warning printed to stderr, line truncated safely, analysis continues
- Too many functions: Warning printed to stderr after 100th function, additional functions not tracked
- Too many includes: Warning printed to stderr after 50th include, additional includes not tracked

**To modify limits:** Edit the `#define` values at the top of `code_metrics.c` and recompile:
```c
#define MAX_LINE_LENGTH 2048  // Increase to handle longer lines
#define MAX_FUNCTIONS 200     // Track more functions
#define MAX_INCLUDES 100      // Track more includes
```

### Parsing Heuristics and Known Limitations

The code metrics analyzer uses heuristics to parse C source code without a full compiler frontend. While it handles many common cases correctly, there are some limitations:

**Comment Detection:**
- Multi-line block comments (`/* ... */`) are tracked across lines
- Inline comments (code followed by `//` or `/*`) are correctly identified
- Lines with both code and comments are counted as code lines
- Comment markers inside string literals (`"// not a comment"`) are correctly ignored
- Comment markers inside character literals (`'/'`) are correctly ignored
- Nested comments (non-standard C) may not be handled correctly in all cases

**Function Detection:**
- Detects function definitions with opening brace `{` on the same line as the closing parenthesis
- Also detects K&R/Allman style where `{` appears on the next line after the function signature
- Function prototypes (declarations ending with `;`) are correctly excluded
- May not correctly identify functions defined via macros or with unusual formatting
- Function pointers and function-like macros may occasionally be misidentified
- Requires function names to be valid C identifiers (start with letter or underscore)

**Include Detection:**
- Handles both angle bracket (`#include <...>`) and quote (`#include "..."`) forms
- Handles whitespace variations (`#  include`, `#include  <...>`)
- Commented-out includes (e.g., `// #include <...>`) are correctly ignored
- May not correctly handle macro-generated includes or conditional includes

**String and Character Handling:**
- Correctly handles escape sequences in strings and character literals (`\"`, `\'`, `\\`)
- Ignores comment markers inside string and character literals
- Assumes standard C string/character literal syntax

**Preprocessor Directives:**
- Basic handling of `#include` directives
- Multi-line preprocessor directives (with backslash continuation) are treated as separate lines
- Conditional compilation (`#ifdef`, `#ifndef`, etc.) directives are counted as code lines

**General Limitations:**
- Designed primarily for C source files; may not work correctly with C++ or other languages
- **C++ is not supported**: C++-specific features (classes, templates, namespaces, etc.) will not be recognized correctly
- Assumes UTF-8 or ASCII encoding for character/line length metrics
- Maximum line length is 1024 characters (longer lines are truncated with warning)
- Maximum of 100 functions and 50 include files tracked per file (warnings issued if exceeded)
- Does not perform semantic analysis or type checking

### Testing

The tool includes a test suite in the `tests/` directory with fixtures covering various edge cases including:
- Simple C programs
- Multi-line block comments
- Inline comments
- Strings containing comment-like sequences
- Various function definition styles (same-line brace, next-line brace, K&R style)
- Commented-out includes

Run the test suite with:
```bash
make test-metrics
```

Or directly:
```bash
./tests/run_tests.sh
```

## Limitations

## Limitations

The tool uses parsing heuristics and has some limitations. See the "Parsing Heuristics and Known Limitations" section above for detailed information.

Brief summary:
- Function detection uses heuristics and may not catch all edge cases (macros, unusual formatting)
- Multi-line preprocessor directives (backslash continuation) are treated as separate lines
- Designed primarily for C source files; may not work correctly with C++ or other languages
- Maximum line length is 1024 characters
- Maximum of 100 functions and 50 include files tracked per file

## Implementation Details

The tool is written in pure C with no external dependencies beyond the standard C library. It uses:
- `stdio.h` for file I/O
- `stdlib.h` for memory management
- `string.h` for string manipulation
- `ctype.h` for character classification

The analysis is performed in a single pass through the source file for efficiency.
