# Code Metrics Spreadsheet Tool

## Overview

The Code Metrics Spreadsheet Tool is an interactive command-line application that analyzes C source files and displays code metrics in a spreadsheet-like tabular format.

## Features

- **Spreadsheet-style display**: Beautiful tabular interface with Unicode box-drawing characters
- **Code metrics analysis**: Analyzes various code quality metrics including:
  - Total lines of code
  - Code lines (non-blank, non-comment)
  - Blank lines
  - Comment lines
  - Function count
  - Include directives count
  - Printf statement count
  - Maximum line length
- **Interactive menu**: Navigate through different views and actions
- **CSV export**: Export metrics to a CSV file for further analysis
- **Detailed view**: Alternative text-based view of metrics
- **Color-coded output**: Enhanced readability with ANSI color codes

## Building

Compile the spreadsheet tool with:

```bash
gcc -Wall -Wextra -o spreadsheet spreadsheet.c metrics.c
```

## Usage

### Basic Usage

Analyze the default `hello.c` file:

```bash
./spreadsheet
```

### Analyze a Custom File

Specify a different C source file:

```bash
./spreadsheet myfile.c
```

## Interactive Menu

Once the tool is running, you'll see an interactive menu with the following options:

1. **Refresh metrics**: Re-analyze the file and update the display
2. **Export to CSV**: Save metrics to `metrics.csv` for spreadsheet applications
3. **Show detailed view**: Display metrics in a simple text format
4. **Exit**: Close the application

## Example Output

```
╔══════════════════════════════════════════════╗
║  Code Metrics Spreadsheet Tool              ║
╚══════════════════════════════════════════════╝

╔════════════════════════════════════════════╗
║  CODE METRICS SPREADSHEET                  ║
╠════════════════════════════════════════════╣
║ File: hello.c                         ║
╠════════════════════════════════════════════╣
║ Metric                   │        Value ║
╠══════════════════════════╪═════════════╣
║ Total Lines              │            5 ║
║ Code Lines               │            4 ║
║ Blank Lines              │            1 ║
║ Comment Lines            │            0 ║
╟──────────────────────────┼─────────────╢
║ Function Count           │            1 ║
║ Include Count            │            0 ║
║ Printf Count             │            1 ║
╟──────────────────────────┼─────────────╢
║ Max Line Length          │           28 ║
╚════════════════════════════════════════════╝
```

## CSV Export Format

The exported CSV file (`metrics.csv`) follows this format:

```csv
Metric,Value
File,hello.c
Total Lines,5
Code Lines,4
Blank Lines,1
Comment Lines,0
Function Count,1
Include Count,0
Printf Count,1
Max Line Length,28
```

## Architecture

The tool consists of three main components:

### 1. `metrics.h` / `metrics.c`

Core analysis engine that:
- Opens and reads C source files line by line
- Counts various code metrics
- Handles comments (both single-line `//` and multi-line `/* */`)
- Identifies code patterns (functions, includes, printf calls)

### 2. `spreadsheet.c`

Main application that:
- Provides the spreadsheet-like UI using Unicode box-drawing characters
- Implements the interactive menu system
- Handles user input and navigation
- Coordinates between the UI and metrics engine
- Manages CSV export functionality

## Limitations

- Function counting uses simple heuristics (looks for `int main`, `void`, `int` with `(`)
- Comment detection may not handle edge cases with strings containing comment-like patterns
- Designed for simple C programs; complex codebases may need more sophisticated analysis

## Future Enhancements

Possible improvements for future versions:
- Support for multiple file analysis
- Comparison view for before/after changes
- Graph visualization of metrics trends
- Code complexity metrics (cyclomatic complexity)
- Integration with CI/CD pipelines
- Web-based interface option
