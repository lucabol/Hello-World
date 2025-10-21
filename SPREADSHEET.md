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

### Plain Text Mode (No Colors)

Disable colors for compatibility with terminals that don't support ANSI codes:

```bash
./spreadsheet --plain hello.c
# or
./spreadsheet --no-color hello.c
```

### Custom CSV Output Path

Specify a custom path for CSV export:

```bash
./spreadsheet --csv /path/to/output.csv hello.c
```

### Display Help

```bash
./spreadsheet --help
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
Include Count,1
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

### Metric Detection Heuristics

The tool uses heuristic-based pattern matching for code analysis, which has some limitations:

#### Function Counting
- **Detection Method**: Looks for patterns like `int main`, `void` followed by `(`, or `int` followed by `(` and `{`
- **Limitations**:
  - May count function prototypes/declarations in addition to definitions
  - May miss functions with complex return types (e.g., `struct MyType* function()`)
  - Does not distinguish between K&R style and ANSI C style declarations
  - Function pointers and nested macros may cause false positives/negatives

#### Include Counting
- **Detection Method**: Searches for `#include`, `# include`, and `#  include` patterns
- **Limitations**:
  - Includes within comments are correctly excluded (✓)
  - Includes within string literals are correctly excluded (✓)
  - Non-standard spacing beyond two spaces may not be detected

#### Printf Counting
- **Detection Method**: Searches for `printf` pattern outside of comments
- **Limitations**:
  - Printf calls within comments are correctly excluded (✓)
  - Printf within string literals doesn't cause false counts (✓)
  - Macros that wrap printf are not detected
  - Custom printf-like functions (e.g., `my_printf`) are not distinguished

#### Comment Detection
- **Detection Method**: Tracks `//` and `/* */` style comments
- **Limitations**:
  - Handles multi-line comments correctly (✓)
  - May not handle edge cases with comment markers inside string literals perfectly
  - Nested comments (not standard C) are not supported

#### Line Length
- **Detection Method**: Uses `getline()` to handle arbitrarily long lines
- **Limitations**: None - handles lines of any length safely (✓)

### General Limitations

- **C-Specific**: Designed for C source files; may not work correctly with C++ or other languages
- **Single File**: Analyzes one file at a time; does not follow `#include` directives
- **No Preprocessing**: Does not run the C preprocessor, so macro expansions are not evaluated
- **Basic Metrics**: Does not calculate advanced metrics like cyclomatic complexity or Halstead metrics

## Error Handling

The tool includes comprehensive error handling:

- **File Access**: Validates file existence and readability before analysis
- **CSV Export**: Checks for file creation/write errors and reports them
- **Input Validation**: Handles invalid menu choices and EOF (Ctrl-D) gracefully
- **Memory Safety**: Uses `getline()` for safe handling of arbitrarily long lines
- **Exit Codes**: Returns 0 on success, 1 on error (CI-friendly)

## Future Enhancements

Possible improvements for future versions:
- Support for multiple file analysis
- Comparison view for before/after changes
- Graph visualization of metrics trends
- Code complexity metrics (cyclomatic complexity)
- Integration with CI/CD pipelines
- Web-based interface option
