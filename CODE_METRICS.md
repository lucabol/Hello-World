# Code Metrics Analyzer - Spreadsheet Interface

A spreadsheet-like tool for analyzing C source code and displaying various metrics in an interactive tabular format.

## Overview

The Code Metrics Analyzer provides a comprehensive view of code characteristics including:
- Line counts (total, code, blank, comments)
- Function detection and counting
- Variable declarations
- Include directives
- Character and word counts
- Statistical summaries

## Building

### Using Make (Recommended)
```bash
# Build the code metrics tool
make code_metrics

# Build everything (hello + code_metrics)
make all
```

### Manual Build
```bash
# Using GCC
gcc -Wall -Wextra -std=c99 -o code_metrics code_metrics.c

# Using Clang
clang -Wall -Wextra -std=c99 -o code_metrics code_metrics.c
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

### Quick analysis using Make
```bash
make metrics
```

## Output Format

The tool displays metrics in a spreadsheet-like table with three columns:
- **METRIC**: The name of the metric being measured
- **VALUE**: The numerical value for that metric
- **DESCRIPTION**: A brief explanation of what the metric represents

### Example Output

```
╔══════════════════════════════════════════════════════════════════════╗
║           CODE METRICS ANALYZER - SPREADSHEET VIEW                  ║
╚══════════════════════════════════════════════════════════════════════╝

File: hello.c

+----------------------+----------+-------------------------------------+
| METRIC               | VALUE    | DESCRIPTION                         |
+----------------------+----------+-------------------------------------+
| Total Lines          | 5        | All lines in file                   |
| Code Lines           | 4        | Lines with actual code              |
| Blank Lines          | 1        | Empty or whitespace-only lines      |
| Comment Lines        | 0        | Lines with comments                 |
+----------------------+----------+-------------------------------------+
| Functions            | 1        | Function definitions detected       |
| Variables            | 0        | Variable declarations (approx)      |
| Include Files        | 1        | #include directives                 |
+----------------------+----------+-------------------------------------+
| Total Characters     | 63       | All characters in file              |
| Total Words          | 8        | Word count (identifiers, keywords)  |
+----------------------+----------+-------------------------------------+

╔══════════════════════════════════════════════════════════════════════╗
║                           SUMMARY STATISTICS                         ║
╚══════════════════════════════════════════════════════════════════════╝

  Code Density:      80.0% (code lines / total lines)
  Comment Ratio:     0.0% (comment lines / total lines)
  Avg Line Length:   12.6 characters
  Avg Words/Line:    2.0 words per code line
```

## Metrics Explained

### Line Metrics
- **Total Lines**: All lines in the file, including blank lines and comments
- **Code Lines**: Lines containing actual C code (excludes blanks and comments)
- **Blank Lines**: Lines that are empty or contain only whitespace
- **Comment Lines**: Lines with single-line comments (`//`) or part of multi-line comments (`/* */`)

### Code Structure Metrics
- **Functions**: Number of function definitions detected (uses heuristic matching)
- **Variables**: Approximate count of variable declarations (common types: int, char, float, etc.)
- **Include Files**: Count of `#include` directives (preprocessor includes)

### Content Metrics
- **Total Characters**: Total character count including all whitespace and newlines
- **Total Words**: Count of identifiers and keywords (alphanumeric sequences)

### Summary Statistics
- **Code Density**: Percentage of lines that contain actual code (code lines / total lines)
- **Comment Ratio**: Percentage of lines that are comments (comment lines / total lines)
- **Avg Line Length**: Average characters per line (total characters / total lines)
- **Avg Words/Line**: Average words per code line (total words / code lines)

## Features

### Interactive Display
- Box-drawing characters for clean table borders
- Three-column spreadsheet layout
- Clear section separators
- Statistical summaries

### Robust Analysis
- Handles various C coding styles
- Detects both `#include` and `# include` (with space)
- Recognizes single-line (`//`) and multi-line (`/* */`) comments
- Identifies function definitions using pattern matching

### Flexible Usage
- Can analyze any C source file
- Defaults to analyzing hello.c if no file specified
- Provides detailed error messages if file cannot be opened

## Limitations

The tool uses heuristic-based detection for some metrics:
- **Function detection**: May not catch all function definitions, especially with unusual formatting
- **Variable counting**: Uses simple pattern matching and may miss complex declarations
- **Comment detection**: Best effort for standard comment styles

For most common C code patterns, the tool provides accurate metrics.

## Integration with Build System

The Code Metrics tool is integrated into the Makefile:

```bash
# Build and run metrics analysis
make metrics

# Clean all build artifacts including code_metrics
make clean
```

## Error Handling

If the specified file cannot be opened, the tool will:
1. Display an error message: `Error: Unable to open file 'filename'`
2. Exit with code 1

Example:
```bash
$ ./code_metrics nonexistent.c
Error: Unable to open file 'nonexistent.c'
$ echo $?
1
```

## Technical Details

- **Language**: C (C99 standard)
- **Compiler Compatibility**: GCC, Clang, and other C99-compliant compilers
- **Dependencies**: Standard C library only (stdio.h, stdlib.h, string.h, ctype.h)
- **Platform**: Cross-platform (Linux, macOS, Windows with appropriate compiler)

## Development

### Code Organization
The code is organized into logical sections:
- Structures for metrics and analysis results
- File analysis functions
- Display and formatting functions
- Helper functions for pattern detection

### Testing
Test the tool with various C files to verify accuracy:
```bash
./code_metrics hello.c
./code_metrics code_metrics.c
./code_metrics test/test_hello.c
```

### Contributing
When enhancing the tool:
1. Maintain the spreadsheet-like table format
2. Add new metrics to the appropriate section
3. Update the summary statistics as needed
4. Ensure backward compatibility with existing output format
