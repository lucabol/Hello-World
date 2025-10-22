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
- **Dynamic Memory Management**: Automatically scales to handle files with many functions
- **Robust Parsing**: Handles multi-line comments, strings with comment-like sequences, and various edge cases

## Building

### Using Make (Recommended)

Compile the tool using the provided Makefile:

```bash
make              # Build with strict flags
make test         # Build and run tests
make debug        # Build with debug symbols
make memcheck     # Run valgrind memory leak check
make clean        # Clean build artifacts
```

### Manual Compilation

Build with GCC using C11 standard and strict warnings:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -o metrics_spreadsheet metrics_spreadsheet.c metrics.c
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

## Implementation Details

### Memory Management

The tool uses dynamic memory allocation with automatic capacity expansion:
- Function names array starts with capacity of 8
- Automatically doubles capacity when needed using `realloc`
- All memory is properly freed to prevent leaks
- Uses `calloc` for zero-initialization of structures

### Parsing Approach

The parser is heuristic-based and handles:
- **Multi-line comments** (/* ... */) spanning multiple lines
- **Single-line comments** (// ...)
- **Inline comments** (code followed by //)
- **String literals** containing comment-like sequences (e.g., "/* not a comment */")
- **Mixed lines** with both code and comments

#### Known Limitations

- Function detection is heuristic-based, looking for type keywords followed by identifiers and parentheses
- May detect some variable declarations as functions if they match the pattern (e.g., `int result = func()`)
- Does not handle preprocessor conditionals that split function declarations
- Complex function pointer declarations may not be detected correctly
- K&R style function declarations are not fully supported

### Type Safety

- Uses `size_t` for all count metrics to handle large files
- Proper signed/unsigned arithmetic to avoid warnings
- Format specifiers match variable types (`%zu` for `size_t`)

### Error Handling

- Returns NULL from `analyze_file` on errors
- Checks all malloc/realloc/calloc returns
- Provides descriptive error messages on stderr
- Returns appropriate exit codes (0 on success, 1 on error)

## Testing

### Running Tests

```bash
make test           # Run full test suite
./test/test_metrics.sh  # Run tests directly
```

### Test Coverage

The test suite includes:
- Basic functionality tests
- Edge cases:
  - Multi-line comments
  - Strings with comment-like sequences
  - Empty files
  - Files with only comments
  - Multiple functions
- Error handling:
  - Non-existent files
  - Invalid arguments

### Continuous Integration

GitHub Actions workflow (`.github/workflows/metrics-ci.yml`) runs:
- Build with strict compiler flags
- Full test suite
- Memory leak detection with Valgrind
- Multiple compiler versions (if configured)

## Files

- `metrics_spreadsheet.c` - Main program entry point
- `metrics.c` - Metrics analysis and display logic
- `metrics.h` - Header file with structure and function declarations
- `Makefile` - Build configuration
- `test/test_metrics.sh` - Comprehensive test script
- `test/inputs/` - Test input files for edge cases
- `.github/workflows/metrics-ci.yml` - CI configuration

## Exit Codes

- `0` - Success
- `1` - Error (file not found, invalid arguments, etc.)

## Contributing

When modifying the code:
1. Maintain C11 standard compliance
2. Ensure code compiles with `-Wall -Wextra -Wpedantic -Werror`
3. Run `make test` to verify all tests pass
4. Run `make memcheck` to verify no memory leaks
5. Update tests for new features or bug fixes
