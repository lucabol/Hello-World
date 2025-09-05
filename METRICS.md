# Code Metrics Spreadsheet Tool

This tool provides a spreadsheet-like interface for analyzing code metrics of the hello.c file.

## Usage

### Show All Metrics (Default)
```bash
./code_metrics
```
Displays all available metrics in a comprehensive spreadsheet-style table.

### Show Basic Metrics
```bash
./code_metrics --basic
```
Shows fundamental metrics: lines, characters, and words.

### Show Structural Metrics
```bash
./code_metrics --structural
```
Displays code structure metrics: functions, includes, braces, and syntax elements.

### Help
```bash
./code_metrics --help
```
Shows usage information and available options.

## Building

```bash
gcc -Wall -Wextra -o code_metrics code_metrics.c
```

## Metrics Analyzed

- **Total Lines**: Total number of lines in the file
- **Code Lines**: Non-blank lines of code
- **Blank Lines**: Empty or whitespace-only lines
- **Characters**: Total character count including spaces
- **Words**: Total word count
- **Include Statements**: Number of #include directives
- **Functions**: Number of function definitions
- **Printf Calls**: Number of printf statements
- **Braces**: Total number of { and } characters
- **Semicolons**: Number of semicolon terminators

## Output Format

The tool displays metrics in a spreadsheet-like table format with:
- Metric name (left column)
- Numerical value (center column)
- Description of what the metric measures (right column)

Example output:
```
╔══════════════════════════════════════════════════════════════════════════════╗
║                        CODE METRICS SPREADSHEET                             ║
║                          Analysis of hello.c                                ║
╚══════════════════════════════════════════════════════════════════════════════╝

Complete Code Metrics Report:
┌──────────────────────────────┬───────────┬────────────────────────────────────────┐
│ METRIC                       │   VALUE   │ DESCRIPTION                            │
├──────────────────────────────┼───────────┼────────────────────────────────────────┤
│ Total Lines                  │         5 │ Total number of lines in the file      │
│ Code Lines                   │         4 │ Non-blank lines of code                │
│ Blank Lines                  │         1 │ Empty or whitespace-only lines         │
│ Characters                   │        67 │ Total character count including spaces │
│ Words                        │         8 │ Total word count                       │
│ Include Statements           │         1 │ Number of #include directives          │
│ Functions                    │         1 │ Number of function definitions         │
│ Printf Calls                 │         1 │ Number of printf statements            │
│ Braces                       │         2 │ Total number of { and } characters     │
│ Semicolons                   │         1 │ Number of semicolon terminators        │
└──────────────────────────────┴───────────┴────────────────────────────────────────┘

Analysis complete!
```

## Features

- **Spreadsheet-like tabular display** with clear column headers and borders
- **Multiple viewing modes** for different analysis needs
- **Comprehensive metrics** covering both basic and structural code properties
- **Command-line interface** with intuitive options
- **Automatic file analysis** of hello.c in the current directory
- **Professional formatting** with Unicode box-drawing characters