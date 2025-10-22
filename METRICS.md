# Code Metrics Spreadsheet Tool

A spreadsheet-like interface for analyzing C source code and displaying code metrics in a tabular, interactive format.

## Features

- **Spreadsheet Display**: View code metrics in a clean, tabular format
- **Multiple Metrics**: Analyze various aspects of your code:
  - Total lines
  - Code lines vs. comment lines vs. blank lines
  - Function count and names
  - Include statement count
  - Percentage breakdown
- **Interactive Mode**: Navigate through different metric views via an interactive menu
- **Command Line Interface**: Quick analysis from the command line

## Building

Compile the tool using GCC:

```bash
gcc -Wall -Wextra -o metrics_spreadsheet metrics_spreadsheet.c metrics.c
```

## Usage

### Basic Usage (Spreadsheet View)

Analyze `hello.c` (default) and display metrics:

```bash
./metrics_spreadsheet
```

Analyze a specific file:

```bash
./metrics_spreadsheet myfile.c
```

### Interactive Mode

Launch the interactive menu for exploring metrics:

```bash
./metrics_spreadsheet -i
./metrics_spreadsheet --interactive
```

The interactive menu provides:
1. **View Full Spreadsheet** - Complete metrics table with all data
2. **View Line Metrics** - Focused view of line-based metrics
3. **View Function Metrics** - Function count and names
4. **View Summary Statistics** - Calculated statistics like code density
5. **Exit** - Exit the interactive viewer

### Help

Display usage information:

```bash
./metrics_spreadsheet -h
./metrics_spreadsheet --help
```

## Example Output

### Spreadsheet View

```
================================================================================
                     CODE METRICS SPREADSHEET                                  
================================================================================
File: hello.c
================================================================================

+----------------------------------+------------------+---------------------+
| METRIC                           | VALUE            | PERCENTAGE          |
+----------------------------------+------------------+---------------------+
| Total Lines                      | 5                | 100%                |
| Code Lines                       | 4                | 80.0%               |
| Comment Lines                    | 0                | 0.0%                |
| Blank Lines                      | 1                | 20.0%               |
+----------------------------------+------------------+---------------------+
| Function Count                   | 1                | N/A                 |
| Include Statements               | 1                | N/A                 |
+----------------------------------+------------------+---------------------+

Functions Detected:
+----------------------------------+
| main                             |
+----------------------------------+
```

### Summary Statistics

```
SUMMARY STATISTICS:
==================
Code Density:    80.0% (code lines / total lines)
Comment Density: 0.0% (comment lines / total lines)
Average Lines per Function: 4.0
```

## Metrics Explained

- **Total Lines**: Total number of lines in the file
- **Code Lines**: Lines containing executable code (excluding comments and blanks)
- **Comment Lines**: Lines containing comments (// or /* */)
- **Blank Lines**: Empty lines or lines with only whitespace
- **Function Count**: Number of functions detected in the code
- **Include Statements**: Number of #include directives
- **Code Density**: Percentage of code lines relative to total lines
- **Comment Density**: Percentage of comment lines relative to total lines
- **Average Lines per Function**: Average code lines per function

## Files

- `metrics_spreadsheet.c` - Main program entry point
- `metrics.c` - Metrics analysis and display logic
- `metrics.h` - Header file with structure and function declarations
