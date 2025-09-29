# Code Metrics Spreadsheet Tool

A command-line tool that analyzes C source code files and displays various code metrics in a spreadsheet-like format.

## Features

- **Spreadsheet-style tabular display** with aligned columns and visual borders
- **Multiple output formats**: 
  - Default spreadsheet view with formatted table
  - CSV export for Excel/Google Sheets
  - Detailed summary with breakdowns and percentages
- **Comprehensive metrics analysis**:
  - Line counts (total, code, comments, blank)
  - Code structure (functions, includes, characters, semicolons, braces)
  - Quality metrics (code density, average line length, complexity estimation)
- **Interactive command-line interface** with help system

## Building

```bash
# Build both hello and metrics programs
make

# Or build just the metrics tool
make metrics

# Build with strict compilation flags
make metrics-strict
```

## Usage

### Basic Analysis
```bash
# Analyze hello.c (default)
./metrics

# Analyze a specific file
./metrics filename.c
```

### Output Formats

#### Spreadsheet View (Default)
```bash
./metrics
```
Displays a formatted table with all metrics in a spreadsheet-like format with borders and aligned columns.

#### CSV Export
```bash
./metrics --csv
```
Outputs comma-separated values suitable for importing into Excel, Google Sheets, or other spreadsheet applications.

#### Detailed Summary
```bash
./metrics --summary
```
Shows a comprehensive breakdown with percentages and detailed explanations of each metric.

#### Help
```bash
./metrics --help
```
Displays interactive help with all available commands and examples.

## Metrics Explained

### Line Distribution
- **Total Lines**: All lines in the file including blank lines
- **Code Lines**: Lines containing actual C code (excluding comments and blank lines)
- **Comment Lines**: Lines containing comments (// or /* */)
- **Blank Lines**: Empty lines or lines with only whitespace

### Code Structure
- **Functions**: Detected function definitions (heuristic-based detection)
- **Includes**: Number of #include statements (handles both #include and # include)
- **Characters**: Total character count excluding newlines
- **Semicolons**: Count of semicolon characters (statement terminators)
- **Braces**: Count of { and } characters (block delimiters)

### Quality Metrics
- **Code Density**: Percentage of lines that contain actual code (Code Lines / Total Lines)
- **Average Line Length**: Average number of characters per line (Characters / Total Lines)
- **Complexity Estimation**: Braces per function ratio (simple complexity indicator)

## Examples

### Example 1: Analyzing hello.c
```bash
$ ./metrics
╔══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
║                                                           CODE METRICS SPREADSHEET                                                                                                                  ║
╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣
║ File          │ Total │ Code  │ Comments │ Blank │ Functions │ Includes │ Characters │ Semicolons │ Braces │ Code Density │ Avg Line Len │ Complexity  ║
║               │ Lines │ Lines │ Lines    │ Lines │           │          │            │            │        │ (Code/Total) │ (Chars/Lines) │ (Braces/Fn) ║
╠═══════════════╪═══════╪═══════╪══════════╪═══════╪═══════════╪══════════╪════════════╪════════════╪════════╪══════════════╪═══════════════╪═════════════╣
║ hello.c       │     5 │     4 │        0 │     1 │         1 │        1 │         58 │          1 │      2 │       80.00% │         11.6 │        2.0 ║
╚═══════════════╧═══════╧═══════╧══════════╧═══════╧═══════════╧══════════╧════════════╧════════════╧════════╧══════════════╧═══════════════╧═════════════╝
```

### Example 2: CSV Export for Spreadsheet Import
```bash
$ ./metrics --csv
Filename,Total Lines,Code Lines,Comment Lines,Blank Lines,Functions,Includes,Characters,Semicolons,Braces,Code Density %,Avg Line Length,Complexity
hello.c,5,4,0,1,1,1,58,1,2,80.00,11.6,2.0
```

### Example 3: Detailed Analysis Summary
```bash
$ ./metrics --summary
╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
║                                                 DETAILED ANALYSIS SUMMARY                                                              ║
╠═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣
║ File: hello.c                                                                                                                        ║
║                                                                                                                                         ║
║ Line Distribution:                                                                                                                      ║
║   • Total Lines:         5 (100.0%)                                                                                                      ║
║   • Code Lines:          4 ( 80.0%)                                                                                                      ║
║   • Comment Lines:       0 (  0.0%)                                                                                                      ║
║   • Blank Lines:         1 ( 20.0%)                                                                                                      ║
║                                                                                                                                         ║
║ Code Structure:                                                                                                                         ║
║   • Functions:           1                                                                                                               ║
║   • Include Statements:  1                                                                                                             ║
║   • Total Characters:   58                                                                                                               ║
║   • Semicolons:          1                                                                                                               ║
║   • Braces:              2                                                                                                               ║
║                                                                                                                                         ║
║ Quality Metrics:                                                                                                                        ║
║   • Code Density:     80.00% (code lines / total lines)                                                                                ║
║   • Avg Line Length:   11.6 characters                                                                                                  ║
║   • Complexity Est.:    2.0 (braces per function)                                                                                       ║
╚═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
```

## Integration with Existing Build System

The metrics tool is integrated with the project's Makefile:

```bash
# Test all metrics functionality
make metrics-test

# Clean all build artifacts (including metrics)
make clean

# Build everything (hello + metrics)
make all
```

## Technical Implementation

- **Language**: C (C99 standard)
- **Dependencies**: Standard C library only (stdio.h, stdlib.h, string.h, ctype.h)
- **Compatibility**: Compiles with both GCC and Clang
- **Validation**: Passes strict compilation flags (-Wall -Wextra -Wpedantic -Werror)

## Use Cases

1. **Code Quality Assessment**: Quickly assess code density and complexity
2. **Project Documentation**: Generate metrics for reports and documentation
3. **Spreadsheet Analysis**: Export to Excel/Sheets for further analysis and charting
4. **Code Review**: Understand code structure and quality metrics during reviews
5. **Education**: Learn about code metrics and quality indicators