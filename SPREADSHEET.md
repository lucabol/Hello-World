# Code Metrics Spreadsheet Tool

A spreadsheet-like interface for analyzing and displaying code metrics from C source files, specifically designed to analyze `hello.c`.

## Overview

The Code Metrics Spreadsheet Tool provides an interactive, tabular interface for viewing various metrics about C source code. It analyzes the code structure and presents the data in a clean, spreadsheet-like format with multiple views and export capabilities.

## Features

### 1. Interactive Menu System
The tool provides an interactive menu with the following options:
- **Full Metrics Table**: Comprehensive view of all code metrics
- **Line Counts Only**: Focused view of line count statistics
- **Code Structure Only**: View of code structure metrics (functions, calls, includes, etc.)
- **Summary Statistics**: Statistical analysis with percentages and averages
- **CSV Export**: Export metrics to a CSV file for further analysis
- **Re-analyze File**: Refresh metrics by re-analyzing the source file

### 2. Comprehensive Metrics

The tool analyzes and displays the following metrics:

#### Line Metrics
- **Total Lines**: Complete line count including all content
- **Code Lines**: Lines containing actual code
- **Comment Lines**: Lines containing comments (single-line // and multi-line /* */)
- **Blank Lines**: Lines containing only whitespace

#### Code Structure Metrics
- **Function Definitions**: Number of function definitions detected
- **Function Calls**: Number of function calls found in the code
- **Include Directives**: Count of #include statements
- **Variable Declarations**: Number of variable declarations
- **Total Characters**: Character count in the file

#### Statistical Analysis
- **Code Coverage**: Percentage of lines that contain code
- **Comment Coverage**: Percentage of lines that are comments
- **Blank Line Coverage**: Percentage of blank lines
- **Average Characters per Line**: Mean character count per line
- **Function Density**: Functions per 100 lines of code
- **Function Call Density**: Function calls per 100 lines of code

### 3. Spreadsheet-like Display

All data is displayed in beautifully formatted tables using box-drawing characters:
```
╔═══════════════════════════════════════════════════════════════════════════╗
║                      CODE METRICS SPREADSHEET                             ║
╠═══════════════════════════════════════════════════════════════════════════╣
║ File: hello.c                                                             ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  METRIC                                              VALUE                ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  Lines of Code (Total)                               5                   ║
║  Lines of Code (Code only)                           4                   ║
...
```

### 4. CSV Export

Export all metrics to a CSV file for use in spreadsheet applications like Excel, Google Sheets, or LibreOffice Calc. The exported file includes:
- All numeric metrics
- Percentage calculations
- File metadata

## Building the Tool

### Prerequisites
- GCC compiler (or compatible C compiler)
- Standard C library

### Compilation

```bash
gcc -Wall -Wextra -o spreadsheet spreadsheet.c code_metrics.c
```

For a stricter build with all warnings enabled:
```bash
gcc -Wall -Wextra -Wpedantic -Werror -o spreadsheet spreadsheet.c code_metrics.c
```

## Usage

### Basic Usage

Analyze the default file (hello.c):
```bash
./spreadsheet
```

Analyze a specific file:
```bash
./spreadsheet path/to/your/file.c
```

### Interactive Menu

Once launched, the tool displays an interactive menu:

```
╔═══════════════════════════════════════════════════════════════════════════╗
║                     CODE METRICS SPREADSHEET MENU                         ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  1. Display Full Metrics Table                                           ║
║  2. Display Line Counts Only                                             ║
║  3. Display Code Structure Only                                          ║
║  4. Display Summary Statistics                                           ║
║  5. Export to CSV Format                                                 ║
║  6. Re-analyze File                                                      ║
║  0. Exit                                                                 ║
╚═══════════════════════════════════════════════════════════════════════════╝

Select an option (0-6):
```

### Example Session

```bash
$ ./spreadsheet hello.c

╔═══════════════════════════════════════════════════════════════════════════╗
║                 CODE METRICS SPREADSHEET TOOL                             ║
║                      Version 1.0                                          ║
╚═══════════════════════════════════════════════════════════════════════════╝

Analyzing file: hello.c
✓ Analysis complete!

Select an option (0-6): 1

╔═══════════════════════════════════════════════════════════════════════════╗
║                      CODE METRICS SPREADSHEET                             ║
╠═══════════════════════════════════════════════════════════════════════════╣
║ File: hello.c                                                             ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  Lines of Code (Total)                               5                   ║
║  Lines of Code (Code only)                           4                   ║
║  Lines of Code (Comments)                            0                   ║
║  Lines of Code (Blank)                               1                   ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  Total Characters                                    63                  ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  Function Definitions                                1                   ║
║  Function Calls                                      2                   ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  Include Directives                                  0                   ║
║  Variable Declarations                               0                   ║
╠═══════════════════════════════════════════════════════════════════════════╣
║                          SUMMARY STATISTICS                               ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  Code Coverage                                        80.0%              ║
║  Comment Coverage                                      0.0%              ║
║  Blank Line Coverage                                  20.0%              ║
╚═══════════════════════════════════════════════════════════════════════════╝

Select an option (0-6): 5

✓ Metrics exported to 'hello.c_metrics.csv'

Select an option (0-6): 0

Exiting Code Metrics Spreadsheet Tool. Goodbye!
```

## File Structure

The tool consists of three main files:

- **spreadsheet.c**: Main program with interactive menu and UI
- **code_metrics.c**: Core analysis engine
- **code_metrics.h**: Header file with data structures and function declarations

## Architecture

### Data Structure

```c
typedef struct {
    int total_lines;
    int code_lines;
    int comment_lines;
    int blank_lines;
    int total_chars;
    int function_count;
    int include_count;
    int variable_declarations;
    int function_calls;
} CodeMetrics;
```

### Key Functions

- `init_metrics()`: Initialize metrics structure
- `analyze_file()`: Parse and analyze C source file
- `display_metrics_table()`: Show full metrics in tabular format
- `display_line_counts()`: Show line count metrics only
- `display_code_structure()`: Show code structure metrics
- `display_summary()`: Show statistical summary
- `export_to_csv()`: Export metrics to CSV file

## CSV Export Format

When you export metrics (option 5), a CSV file is created with the following format:

```csv
Metric,Value
File,hello.c
Total Lines,5
Code Lines,4
Comment Lines,0
Blank Lines,1
Total Characters,63
Function Definitions,1
Function Calls,2
Include Directives,0
Variable Declarations,0
Code Coverage (%),80.0
Comment Coverage (%),0.0
Blank Coverage (%),20.0
```

The CSV file is named `<filename>_metrics.csv` (e.g., `hello.c_metrics.csv`).

## Analysis Methodology

### Line Classification
- **Blank Line**: Contains only whitespace characters
- **Comment Line**: Starts with `//` or `/*` or is part of a multi-line comment
- **Code Line**: Contains actual C code (neither blank nor comment)

### Function Detection
- Simple heuristic looking for patterns like `type name() {`
- Excludes control structures (if, while, for, switch)
- Detects function definitions that span multiple lines

### Function Call Detection
- Identifies identifiers followed by `(`
- Excludes keywords (if, while, for, switch, return, sizeof)
- Counts calls to functions like `printf()`, `scanf()`, etc.

### Variable Declaration Detection
- Looks for type keywords (int, char, float, double, void, long)
- Requires semicolon on the same line
- Excludes function definitions and includes

## Limitations

- **Heuristic-based**: The analysis uses heuristics and may not be 100% accurate for complex code
- **C-specific**: Designed for C source files, may not work correctly with C++ or other languages
- **Simple parsing**: Does not perform full lexical analysis or use a parser
- **Best for simple code**: Works best with straightforward C code like hello.c

## Future Enhancements

Potential improvements for future versions:
- Support for multiple files at once
- More detailed metrics (cyclomatic complexity, nesting levels)
- Graphical visualization of metrics
- Comparison mode for before/after analysis
- Export to other formats (JSON, XML)
- Integration with version control systems

## License

This tool is part of the Hello-World project and follows the same license terms.

## Contributing

Contributions are welcome! Feel free to:
- Report bugs or issues
- Suggest new metrics to track
- Improve the analysis algorithms
- Add new export formats
- Enhance the UI/UX

## See Also

- hello.c - The sample C file
- README.md - Project overview
