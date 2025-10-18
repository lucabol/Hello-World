# Code Metrics Spreadsheet Tool

A spreadsheet-like interface for analyzing code metrics of the hello.c program.

## Overview

The Code Metrics Spreadsheet Tool provides an interactive, tabular interface for viewing various code quality and structure metrics. It analyzes the hello.c source file and displays metrics in a visually appealing spreadsheet format using ASCII box-drawing characters.

## Features

- **Interactive Menu System**: Navigate through different metric views with a user-friendly menu
- **Spreadsheet-Like Display**: Tabular format with clear columns and rows
- **Multiple Metric Views**:
  1. Full Spreadsheet View - Comprehensive overview with percentages
  2. Summary Table - Quick reference of key metrics
  3. Function Inventory - List of all functions detected in the code
  4. Detailed Metrics - In-depth analysis with calculated ratios

## Metrics Collected

### Line Metrics
- **Total Lines**: All lines in the source file
- **Code Lines**: Lines containing executable code
- **Blank Lines**: Empty lines
- **Comment Lines**: Lines containing comments

### Structural Metrics
- **Functions**: Number of function definitions detected
- **Include Statements**: Number of #include directives
- **Code/Comment Ratio**: Ratio of code to documentation
- **Average Code per Function**: Average lines of code per function

### Calculated Percentages
- Code percentage of total lines
- Comment percentage of total lines
- Blank line percentage of total lines

## Building

Compile the metrics tool using GCC:

```bash
gcc -Wall -Wextra -o metrics metrics.c
```

## Usage

Run the interactive tool:

```bash
./metrics
```

The tool will display an interactive menu with the following options:

```
1. Display Full Spreadsheet View
2. Display Summary Table
3. Display Function Inventory
4. Display Detailed Metrics
5. Exit
```

Enter the number corresponding to your desired view and press Enter.

## Example Output

### Full Spreadsheet View
```
╔══════════════════════════════════════════════════════════════════════════╗
║                        METRICS SPREADSHEET VIEW                          ║
╠══════════════════════════════════════════════════════════════════════════╣
║ Metric Category          │ Value      │ Percentage │ Notes              ║
╠══════════════════════════════════════════════════════════════════════════╣
║ Total Lines              │ 5          │   100.00% │ All lines          ║
║ Code Lines               │ 4          │    80.00% │ Executable code    ║
║ Blank Lines              │ 1          │    20.00% │ Empty lines        ║
║ Comment Lines            │ 0          │     0.00% │ Documentation      ║
╠══════════════════════════════════════════════════════════════════════════╣
║ Functions                │ 1          │      N/A   │ Function defs      ║
║ Include Statements       │ 1          │      N/A   │ Header includes    ║
╚══════════════════════════════════════════════════════════════════════════╝
```

### Summary Table
```
╔═══════════════════════════════════════╗
║        SUMMARY METRICS TABLE          ║
╠═══════════════════════════════════════╣
║ Metric            │ Value             ║
╠═══════════════════════════════════════╣
║ Total Lines       │ 5                 ║
║ Code Lines        │ 4                 ║
║ Blank Lines       │ 1                 ║
║ Comment Lines     │ 0                 ║
║ Functions         │ 1                 ║
║ Includes          │ 1                 ║
╚═══════════════════════════════════════╝
```

### Function Inventory
```
╔═══════════════════════════════════════════════╗
║          FUNCTION INVENTORY TABLE             ║
╠═══════════════════════════════════════════════╣
║ #  │ Function Name                            ║
╠═══════════════════════════════════════════════╣
║ 1  │ main                                     ║
╠═══════════════════════════════════════════════╣
║ Total Functions: 1                            ║
╚═══════════════════════════════════════════════╝
```

## Technical Details

### Implementation
- Written in pure C (C99 standard)
- Uses standard C library functions only
- File analysis using line-by-line parsing
- Simple heuristics for function detection

### Limitations
- Function detection uses basic pattern matching
- Multi-line function declarations may not be detected correctly
- Complex macro definitions might affect accuracy
- Best suited for simple C programs like hello.c

## Target File

The tool is configured to analyze `hello.c` by default. This is defined by the `TARGET_FILE` constant in the source code.

## Integration with Project

The metrics tool is an independent utility that can be run alongside the main hello.c program:

```bash
# Compile and run hello.c
gcc -o hello hello.c
./hello

# Compile and run metrics analyzer
gcc -o metrics metrics.c
./metrics
```

## Future Enhancements

Potential features for future versions:
- Analyze multiple files
- Cyclomatic complexity calculation
- Export metrics to CSV format
- Command-line arguments for non-interactive mode
- Historical metrics tracking
- Graphical charts using ASCII art
