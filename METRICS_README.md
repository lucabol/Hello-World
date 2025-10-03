# Code Metrics Analyzer - Spreadsheet Interface

A spreadsheet-like tool for analyzing C source code and displaying various metrics in tabular format.

## Features

- **Summary Metrics Display**: Shows key code metrics in a clean, tabular format
  - Total lines, code lines, blank lines, comment lines
  - Include directives, functions, statements
  - Character counts and averages
  - Code density and composition statistics

- **Line-by-Line Analysis**: Detailed spreadsheet view of each line
  - Line number and type (code/blank/comment)
  - Character count per line
  - Indicators for semicolons and include directives
  - Preview of line content

- **CSV Export**: Export all metrics to CSV format for use in spreadsheet applications
  - Summary metrics
  - Line-by-line data
  - Compatible with Excel, Google Sheets, LibreOffice Calc

## Building

```bash
gcc -Wall -Wextra -o metrics metrics.c
```

## Usage

### Basic Analysis
Analyze the default file (hello.c):
```bash
./metrics
```

### Analyze a Specific File
```bash
./metrics myfile.c
```

### Show Line-by-Line Analysis
```bash
./metrics --lines
./metrics -l
```

### Export to CSV
```bash
./metrics --csv
./metrics -c
```

### Combine Options
```bash
./metrics --lines --csv hello.c
```

### Help
```bash
./metrics --help
```

## Example Output

### Summary Metrics
```
================================================================================
                     CODE METRICS ANALYSIS SPREADSHEET
================================================================================
File: hello.c

+---------------------------------------------+
| Metric                         |      Value |
+---------------------------------------------+
| Total Lines                    |          5 |
| Code Lines                     |          4 |
| Blank Lines                    |          1 |
| Comment Lines                  |          0 |
+---------------------------------------------+
| Include Directives             |          1 |
| Functions                      |          1 |
| Statements                     |          1 |
+---------------------------------------------+
| Total Characters               |         63 |
| Avg Chars per Line             |         12 |
+---------------------------------------------+

Summary Statistics:
  Code Density: 80.0% (code lines / total lines)
  Comment Ratio: 0.0% (comment lines / total lines)
  Blank Ratio: 20.0% (blank lines / total lines)
```

### Line-by-Line Analysis (with --lines flag)
```
================================================================================
                     LINE-BY-LINE ANALYSIS (Spreadsheet View)
================================================================================

+------+----------+-------+-----------+---------+---------------------------+
| Line | Type     | Chars | Semicolon | Include | Content                   |
+------+----------+-------+-----------+---------+---------------------------+
|    1 | code     |    19 | No        | Yes     | # include <stdio.h>       |
|    2 | blank    |     0 | No        | No      |                           |
|    3 | code     |    11 | No        | No      | int main(){               |
|    4 | code     |    27 | Yes       | No      |     printf("Hello world!  |
|    5 | code     |     1 | No        | No      | }                         |
+------+----------+-------+-----------+---------+---------------------------+
```

## CSV Export Format

The CSV file contains two sections:

1. **Summary Metrics**: Metric name and value pairs
2. **Line-by-Line Data**: Detailed analysis of each line

Example CSV structure:
```csv
Metric,Value
Total Lines,5
Code Lines,4
...

Line,Type,Chars,Semicolon,Include,Content
1,code,19,No,Yes,"# include <stdio.h>"
2,blank,0,No,No,""
...
```

## Use Cases

- **Code Review**: Quickly assess code structure and composition
- **Quality Metrics**: Track code density, comment ratio, and other quality indicators
- **Documentation**: Generate metrics reports for project documentation
- **Analysis**: Export to spreadsheet tools for further analysis and visualization
- **Education**: Learn about code structure and complexity metrics

## Metrics Explained

- **Total Lines**: Total number of lines in the file
- **Code Lines**: Lines containing executable code
- **Blank Lines**: Empty lines or lines with only whitespace
- **Comment Lines**: Lines starting with // or /* (basic detection)
- **Include Directives**: Number of #include statements
- **Functions**: Number of function definitions (basic heuristic)
- **Statements**: Lines ending with semicolon
- **Total Characters**: Sum of all characters in the file
- **Code Density**: Percentage of lines that contain code
- **Comment Ratio**: Percentage of lines that are comments
- **Blank Ratio**: Percentage of lines that are blank

## Limitations

- Comment detection is basic (only detects lines starting with // or /*)
- Function counting uses simple heuristics
- Multi-line statements may be counted multiple times
- Designed primarily for simple C files

## Integration with Hello World Project

This tool is designed to analyze the hello.c file in this repository but can be used with any C source file. It provides insights into code structure and quality metrics in an easy-to-read spreadsheet format.
