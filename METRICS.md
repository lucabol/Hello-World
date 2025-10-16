# Code Metrics Spreadsheet Tool

A spreadsheet-like interface for analyzing C source code and displaying various code metrics in a tabular, interactive format.

## Overview

The Code Metrics Spreadsheet tool (`metrics.c`) provides a comprehensive analysis of C source files with an easy-to-read tabular display. It can analyze code and display various metrics including lines of code, function counts, complexity indicators, and more.

## Features

- **Spreadsheet-like Table Display**: Metrics are presented in a clean, bordered table format
- **Interactive Mode**: Menu-driven interface for exploring metrics
- **CSV Export**: Export metrics to CSV format for use in spreadsheet applications
- **Multiple Analysis Modes**: 
  - Quick analysis (default)
  - Interactive exploration
  - Batch analysis of specific files

## Metrics Analyzed

The tool analyzes and displays the following metrics:

| Metric | Description |
|--------|-------------|
| Total Lines | Total number of lines in the file |
| Blank Lines | Number of blank or whitespace-only lines |
| Comment Lines | Number of lines containing comments |
| Code Lines | Number of lines containing actual code |
| Include Statements | Number of `#include` directives |
| Functions | Number of function definitions detected |
| Brace Pairs | Number of opening/closing brace pairs |
| Semicolons | Number of statement terminators |
| Code Density | Percentage of lines containing code vs. total lines |

## Building

Compile the metrics tool using gcc:

```bash
gcc -Wall -Wextra -o metrics metrics.c
```

## Usage

### Quick Analysis (Default Mode)

Analyze hello.c and display metrics in a table:

```bash
./metrics
```

Analyze a specific file:

```bash
./metrics myfile.c
```

### Interactive Mode

Run the interactive menu-driven interface:

```bash
./metrics --interactive
```

Interactive menu options:
1. **Analyze hello.c** - Analyze and display metrics in table format
2. **Display summary** - Show compact summary of metrics
3. **Export to CSV** - Export metrics to a CSV file
4. **Exit** - Quit the program

### Help

Display usage information:

```bash
./metrics -h
```

## Example Output

### Table Display

```
╔════════════════════════════════════════════════════════════════╗
║           CODE METRICS SPREADSHEET ANALYSIS                    ║
╠════════════════════════════════════════════════════════════════╣
║ File: hello.c                                                  ║
╠══════════════════════════════════╦═════════════════════════════╣
║ Metric                           ║ Value                       ║
╠══════════════════════════════════╬═════════════════════════════╣
║ Total Lines                      ║ 5                           ║
║ Blank Lines                      ║ 1                           ║
║ Comment Lines                    ║ 0                           ║
║ Code Lines                       ║ 4                           ║
╠══════════════════════════════════╬═════════════════════════════╣
║ Include Statements               ║ 1                           ║
║ Functions                        ║ 1                           ║
║ Brace Pairs                      ║ 1                           ║
║ Semicolons                       ║ 1                           ║
╠══════════════════════════════════╬═════════════════════════════╣
║ Code Density                     ║ 80.0%                       ║
╚══════════════════════════════════╩═════════════════════════════╝
```

### CSV Export

When you export metrics to CSV, a file named `metrics_<filename>.csv` is created:

```csv
Metric,Value
Source File,hello.c
Total Lines,5
Blank Lines,1
Comment Lines,0
Code Lines,4
Include Statements,1
Functions,1
Brace Pairs,1
Semicolons,1
Code Density,80.0%
```

This CSV can be opened in Excel, Google Sheets, or any spreadsheet application for further analysis.

## Use Cases

- **Code Review**: Quickly assess code structure and complexity
- **Project Documentation**: Generate metrics reports for documentation
- **Quality Assurance**: Track code metrics over time
- **Learning**: Understand code composition and structure

## Integration with Existing Tools

The metrics tool is designed to complement the existing hello.c program:

1. **Standalone Analysis**: Run metrics on hello.c or any other C file
2. **CSV Export**: Use exported data in spreadsheet applications
3. **Scriptable**: Can be integrated into build scripts or CI/CD pipelines

## Technical Details

- **Language**: C (C99 standard)
- **Dependencies**: Standard C library only (stdio.h, stdlib.h, string.h, ctype.h)
- **Portability**: Works on any system with a C compiler
- **File Format Support**: Analyzes C source files (.c, .h)

## Limitations

- Function detection uses simplified pattern matching
- Multi-line comments are counted per line, not per comment block
- Macro definitions and preprocessor directives (other than includes) are not separately tracked
- Does not perform deep syntax analysis (this is a metrics tool, not a parser)

## Future Enhancements

Potential future features could include:
- Support for multiple file analysis
- Cyclomatic complexity calculation
- More detailed comment analysis
- HTML report generation
- Historical metrics tracking
