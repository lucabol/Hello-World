# Code Metrics Spreadsheet Tool

A spreadsheet-like interface for analyzing code metrics of C source files with support for interactive and non-interactive modes, CSV export, and comprehensive edge case handling.

## Overview

The Code Metrics Tool analyzes C source files and displays various metrics in a tabular, interactive format. It provides insights into code structure, size, and complexity with robust parsing that handles comments in strings, escaped characters, CRLF line endings, and other edge cases.

## Features

- **Spreadsheet-like Display**: Presents metrics in a clear, tabular format with Unicode box-drawing characters
- **Interactive & Non-Interactive Modes**: Use interactively for exploration or non-interactively for automation
- **CSV Export**: Output metrics in CSV format for integration with other tools
- **Robust Parsing**:
  - Handles comments inside strings and character literals correctly
  - Supports escaped quotes and special characters
  - Processes both LF and CRLF line endings
  - Uses getline() for unlimited line length support
  - Expands tabs to 8 spaces for accurate line length calculation
  - Distinguishes function definitions from prototypes and control statements
- **Signal Handling**: Gracefully handles SIGINT (Ctrl+C) and SIGTERM
- **CLI Flags**: Supports --help, --version, and other command-line options

### Code Metrics Collected

- Total lines of code
- Code lines (executable statements)
- Blank lines
- Comment lines (single-line and multi-line)
- Number of functions (definitions only, not prototypes)
- Include directives (#include)
- Maximum line length (with tab expansion)
- Average lines per function
- Percentage breakdowns

## Building

### Using Make (Recommended)

```bash
make                # Build all (hello and metrics_tool)
make metrics_tool   # Build only metrics_tool
make strict         # Build with -Werror flag
make test           # Build and run all tests
```

### Using GCC Directly

```bash
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion \
    -std=c99 -D_POSIX_C_SOURCE=200809L \
    -o metrics_tool metrics_tool.c code_metrics.c
```

### Platform Support

- **Linux/Unix**: Full support (tested with GCC and Clang)
- **macOS**: Full support (requires C99 and POSIX.1-2008)
- **Windows**: Requires POSIX-compatible environment (WSL, Cygwin, MinGW)

## Usage

### Basic Usage

```bash
# Analyze hello.c (default)
./metrics_tool

# Analyze a specific file
./metrics_tool myfile.c

# Display help
./metrics_tool --help

# Display version
./metrics_tool --version
```

### Non-Interactive Mode

```bash
# Analyze and display metrics, then exit
./metrics_tool --non-interactive myfile.c
./metrics_tool -n myfile.c
```

### CSV Export

```bash
# Export metrics in CSV format
./metrics_tool --csv myfile.c
./metrics_tool -c myfile.c

# Redirect to file
./metrics_tool --csv myfile.c > metrics.csv
```

## Interactive Commands

Once the tool is running in interactive mode, you can use these commands:

- **r** - Refresh metrics (re-analyze the file)
- **s** - Show detailed statistics with percentages
- **h** - Display help menu
- **q** - Quit the tool
- **Ctrl+C** - Gracefully exit

## Example Output

### Table View

```
════════════════════════════════════════════════════════════════════════════════
                   CODE METRICS SPREADSHEET TOOL v1.0.1
════════════════════════════════════════════════════════════════════════════════

Analyzing: hello.c

╔════════════════════════════════════════════════════════════════════════════════╗
║                         CODE METRICS SPREADSHEET                               ║
╠═══════════════╦═════════╦═════════╦═════════╦═════════╦═════════╦═════════════╣
║ Filename      ║ Total   ║ Code    ║ Blank   ║ Comment ║ Funcs   ║ Max Line    ║
║               ║ Lines   ║ Lines   ║ Lines   ║ Lines   ║         ║ Length      ║
╠═══════════════╬═════════╬═════════╬═════════╬═════════╬═════════╬═════════════╣
║ hello.c       ║       5 ║       4 ║       1 ║       0 ║       1 ║          29 ║
╚═══════════════╩═════════╩═════════╩═════════╩═════════╩═════════╩═════════════╝

Additional Metrics:
  hello.c: 1 #include directive(s)
```

### CSV Output

```csv
Filename,Total Lines,Code Lines,Blank Lines,Comment Lines,Functions,Includes,Max Line Length
hello.c,5,4,1,0,1,1,29
```

## Implementation Details

### Components

1. **code_metrics.h** - Header file with data structures and API documentation
2. **code_metrics.c** - Core analysis engine that parses C files and collects metrics
3. **metrics_tool.c** - Interactive/non-interactive interface and display logic

### Metrics Calculation

- **Total Lines**: All lines in the file including blank lines and comments
- **Code Lines**: Lines containing executable code (excludes pure comment and blank lines)
- **Blank Lines**: Lines containing only whitespace
- **Comment Lines**: Lines that are primarily comments (not counting code with trailing comments)
- **Functions**: Function definitions detected using improved heuristics:
  - Excludes function prototypes (lines ending with semicolon)
  - Excludes control statements (if, for, while, switch, etc.)
  - Excludes function pointer declarations
  - Requires return type keyword and opening brace
- **Include Directives**: Counts #include directives (handles whitespace variations)
- **Max Line Length**: Longest line with tabs expanded to 8 spaces

### Edge Cases Handled

1. **Comments in Strings**: `printf("/* not a comment */")` - not counted as comment
2. **Escaped Quotes**: `"He said \"Hello\""` and `'\''` - handled correctly
3. **Function Prototypes**: `int add(int, int);` - not counted as function
4. **Control Statements**: `if (x) { }` - not counted as function
5. **Mixed Line Endings**: Both LF and CRLF line endings supported
6. **Long Lines**: No line length limit (uses getline())
7. **Tabs**: Expanded to 8 spaces for display length calculation
8. **Code + Comment**: `int x = 1; // comment` - counted as code line

## Testing

### Run All Tests

```bash
make test           # Run all tests
make test-quiet     # Run tests in quiet mode (for CI)
```

### Test Suite

The test suite (`test/test_metrics.sh`) includes 16 comprehensive tests covering:
- Blank files and files with only whitespace
- Single-line and multi-line comments
- Comments inside strings and escaped characters
- Function prototypes vs definitions
- Control statements
- Include directives with various formatting
- Long lines and tab handling
- CRLF line endings
- Code mixed with comments

All tests validate that metrics are calculated correctly for edge cases.

## Dependencies

- **Standard C Library**: stdio.h, string.h, ctype.h, stdlib.h, stdbool.h, signal.h
- **POSIX**: getline() function (POSIX.1-2008)
- **No external dependencies**: Pure C implementation

## Compatibility

- **C Standard**: C99 or later
- **POSIX**: Requires POSIX.1-2008 for getline()
- **Compilers**: GCC, Clang (tested)
- **Line Endings**: LF and CRLF
- **Text Encoding**: ASCII/UTF-8

## Security & Robustness

- ✅ Compiles with strict warning flags (-Wall -Wextra -Wpedantic -Werror)
- ✅ No fixed buffer limits (uses getline() for dynamic allocation)
- ✅ Safe string copying (no unsafe strncpy usage)
- ✅ Error handling for file operations
- ✅ Signal handling for graceful shutdown
- ✅ No unchecked buffer writes

## API Documentation

### analyze_file()

```c
int analyze_file(const char* filename, CodeMetrics* metrics);
```

Analyzes a C source file and populates the metrics structure.

**Parameters:**
- `filename`: Path to the C source file to analyze
- `metrics`: Pointer to CodeMetrics structure to populate

**Returns:**
- 0 on success
- -1 on error (file not found, invalid parameters)

**Thread Safety:** Not thread-safe. Each thread should use separate CodeMetrics structures.

**Memory Ownership:** Caller owns the CodeMetrics structure.

## Limitations

- Function detection uses heuristics and may not catch all edge cases (macros, templates, etc.)
- Designed primarily for C source files (not C++)
- Does not perform deep semantic analysis (e.g., cyclomatic complexity, nesting depth)
- Requires POSIX.1-2008 for getline() (not available on pure Windows without compatibility layer)

## Future Enhancements

Potential improvements for future versions:
- Support for multiple file analysis in one session
- Advanced complexity metrics (cyclomatic complexity, nesting depth)
- Configurable metric thresholds and warnings
- Support for other languages (C++, Java, Python)
- Interactive terminal UI improvements (arrow key navigation, colors)
- Export to JSON format

## Contributing

When contributing improvements:
1. Ensure all tests pass: `make test`
2. Add tests for new features
3. Compile with strict flags: `make strict`
4. Update documentation as needed

## License

Part of the lucabol/Hello-World repository.
