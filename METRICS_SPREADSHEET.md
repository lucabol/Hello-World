# Code Metrics Spreadsheet

A spreadsheet-like interface for analyzing C source code and displaying various code metrics in a tabular, interactive format.

## Features

- **Code Analysis**: Analyzes C source files and extracts various metrics
- **Spreadsheet Display**: Shows metrics in a clean, tabular format
- **Interactive Sorting**: Sort metrics by name or value in real-time
- **Multiple Metrics**: Tracks 11 different code quality and complexity metrics

## Building

Compile the spreadsheet tool using GCC:

```bash
gcc -Wall -Wextra -o metrics_spreadsheet metrics_spreadsheet.c
```

## Usage

### Basic Usage

Analyze the default file (hello.c):

```bash
./metrics_spreadsheet
```

### Analyze a Specific File

```bash
./metrics_spreadsheet <filename.c>
```

### Interactive Commands

Once the spreadsheet is displayed, you can use these commands:

- **s** - Sort metrics by value (descending)
- **n** - Sort metrics by name (alphabetically)
- **q** - Quit the spreadsheet

## Example

```bash
$ echo "q" | ./metrics_spreadsheet hello.c
```

Output:
```
=== Code Metrics Spreadsheet ===
File: hello.c

------------------------------------------------------------
| Metric                                   | Value        |
------------------------------------------------------------
| Total Lines                              |            5 |
| Code Lines                               |            4 |
| Blank Lines                              |            1 |
| Comment Lines                            |            0 |
| Include Statements                       |            0 |
| Functions                                |            1 |
| Printf Calls                             |            1 |
| Total Characters                         |           63 |
| Opening Braces                           |            1 |
| Closing Braces                           |            1 |
| Semicolons                               |            1 |
------------------------------------------------------------
```

## Metrics Tracked

The spreadsheet displays the following metrics:

1. **Total Lines** - Total number of lines in the file
2. **Code Lines** - Lines containing actual code
3. **Blank Lines** - Empty lines
4. **Comment Lines** - Lines containing comments
5. **Include Statements** - Number of #include directives
6. **Functions** - Number of function definitions
7. **Printf Calls** - Number of printf statements
8. **Total Characters** - Total character count
9. **Opening Braces** - Count of { characters
10. **Closing Braces** - Count of } characters
11. **Semicolons** - Count of ; characters

## Implementation

The tool is implemented in pure C with no external dependencies beyond the standard library. It provides:

- File parsing and analysis
- Metric calculation
- Formatted table display
- Interactive command processing
- Real-time sorting capabilities
