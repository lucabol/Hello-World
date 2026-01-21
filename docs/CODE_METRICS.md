# Code Metrics Tool

## Overview

The `code_metrics.c` program is a robust file analysis tool that computes basic metrics for C source files. It demonstrates comprehensive error handling for various file I/O scenarios.

## Features

- **Line counting**: Counts the number of lines in a file
- **Character counting**: Counts total characters
- **Word counting**: Counts space-delimited words
- **Comprehensive error handling**: Handles all common file I/O errors gracefully
- **Clear error messages**: Provides informative feedback for all failure scenarios

## Building

### Using Make (recommended)
```bash
make code_metrics
```

### Manual compilation
```bash
gcc -Wall -Wextra -Wpedantic -o code_metrics code_metrics.c
```

### Strict compilation (with -Werror)
```bash
make strict
```

## Usage

### Basic usage
```bash
./code_metrics <input_file>
```

Example:
```bash
./code_metrics hello.c
```

Output:
```
Code Metrics for: hello.c
Lines: 5
Characters: 65
Words: 8
```

### Write results to file
```bash
./code_metrics <input_file> <output_file>
```

Example:
```bash
./code_metrics hello.c metrics_report.txt
```

This will display metrics on stdout AND write a formatted report to the output file.

## Error Handling

The program implements comprehensive error handling for the following scenarios:

### 1. File Not Found
**Scenario**: Input file does not exist

**Example**:
```bash
./code_metrics nonexistent.c
```

**Error message**:
```
Error: File not found - nonexistent.c
Analysis failed with error code: 1
```

### 2. Permission Denied
**Scenario**: Insufficient permissions to read input file or write output file

**Example**:
```bash
chmod 000 test.c
./code_metrics test.c
```

**Error message**:
```
Error: Permission denied - test.c
Please check file permissions and try again.
Analysis failed with error code: 2
```

### 3. Invalid File Format
**Scenario**: File is not a valid C source file (.c) or header file (.h)

**Example**:
```bash
./code_metrics README.md
```

**Error message**:
```
Error: Invalid file format - README.md
Expected a valid C source file (.c extension).
Analysis failed with error code: 5
```

### 4. Read Failure
**Scenario**: File exists but cannot be read (corrupted, I/O error)

**Error message**:
```
Error: Failed to read from file - <filename>
The file may be corrupted or inaccessible.
Analysis failed with error code: 3
```

### 5. Write Failure
**Scenario**: Cannot write to output file

**Error message**:
```
Error: Failed to write to file - <filename>
Check disk space and write permissions.
Failed to write metrics to file
```

### 6. Disk Full
**Scenario**: Insufficient disk space for writing output

**Error message**:
```
Error: Disk full or insufficient space - <filename>
Free up disk space and try again.
Failed to write metrics to file
```

## Error Codes

The program returns the following exit codes:

| Code | Error Type | Description |
|------|------------|-------------|
| 0 | SUCCESS | Operation completed successfully |
| 1 | ERROR_FILE_NOT_FOUND | Input file does not exist |
| 2 | ERROR_PERMISSION_DENIED | Insufficient permissions |
| 3 | ERROR_READ_FAILURE | Failed to read from file |
| 4 | ERROR_WRITE_FAILURE | Failed to write to file |
| 5 | ERROR_INVALID_FORMAT | Invalid file format |
| 6 | ERROR_DISK_FULL | Insufficient disk space |
| 7 | ERROR_MEMORY_ALLOCATION | Memory allocation failed |

## Testing

### Run all code_metrics tests
```bash
make test-metrics
```

### Run all tests (hello.c and code_metrics.c)
```bash
make test
```

The test suite validates:
- ✅ Missing command line arguments
- ✅ Non-existent files
- ✅ Permission errors (read and write)
- ✅ Invalid file formats
- ✅ Valid file analysis
- ✅ Output file generation
- ✅ Write to read-only directories
- ✅ Self-analysis capability
- ✅ Multiple sequential analyses

## Implementation Details

### Key Error Handling Features

1. **Errno-based error detection**: Uses `errno` to determine specific error types
2. **Resource cleanup**: Ensures file handles are properly closed even on errors
3. **Graceful degradation**: Continues operation where possible, fails safely otherwise
4. **Informative messages**: Provides context-specific error messages to users
5. **Input validation**: Validates all inputs before processing

### File I/O Operations

All file operations include comprehensive error checking:

```c
// Example: fopen with error handling
file = fopen(filename, "r");
if (file == NULL) {
    if (errno == ENOENT) {
        // Handle file not found
    } else if (errno == EACCES || errno == EPERM) {
        // Handle permission denied
    } else {
        // Handle other read errors
    }
}
```

### Memory Safety

- All pointers are validated before use
- File handles are closed in all code paths
- No dynamic memory allocation (stack-based structures only)

## Examples

### Analyze a C source file
```bash
$ ./code_metrics code_metrics.c
Code Metrics for: code_metrics.c
Lines: 287
Characters: 8710
Words: 1234
```

### Generate a report file
```bash
$ ./code_metrics hello.c report.txt
Code Metrics for: hello.c
Lines: 5
Characters: 65
Words: 8
Metrics written to: report.txt

$ cat report.txt
Code Metrics Report
===================
Lines: 5
Characters: 65
Words: 8
```

### Handle permission errors gracefully
```bash
$ touch no_read.c
$ chmod 000 no_read.c
$ ./code_metrics no_read.c
Error: Permission denied - no_read.c
Please check file permissions and try again.
Analysis failed with error code: 2
```

## Integration with CI/CD

The code_metrics tool integrates seamlessly with CI/CD pipelines:

```bash
# In your CI script
make test-metrics
if [ $? -eq 0 ]; then
    echo "All code_metrics tests passed"
else
    echo "Code_metrics tests failed"
    exit 1
fi
```

## Limitations

- Only supports C source files (.c) and header files (.h)
- Word counting is space-delimited (doesn't handle all C tokenization rules)
- Line counting includes empty lines
- Character counting includes all characters (including whitespace)

## Future Enhancements

Potential improvements for future versions:
- Support for additional file types
- More sophisticated tokenization for accurate word counting
- Code complexity metrics
- Function and variable counting
- Comment detection and statistics
- Configurable output formats (JSON, XML, CSV)
