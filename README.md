# Hello World
Test repo for JediMaster

## Code Metrics Spreadsheet Interface

This repository now includes a spreadsheet-like interface for analyzing code metrics. The metrics analyzer can examine C source files and display various code statistics in both table and CSV formats.

### Features

- **Multiple output formats**: Table format for console viewing and CSV format for spreadsheet import
- **Comprehensive metrics**: Lines of code, blank lines, comments, includes, functions, and more
- **Multiple file analysis**: Analyze single files or multiple files at once
- **Spreadsheet ready**: CSV output can be directly imported into Excel, Google Sheets, etc.

### Usage

```bash
# Build the metrics analyzer
gcc -Wall -Wextra -o metrics metrics.c

# Analyze a single file (table format)
./metrics hello.c

# Analyze a single file (CSV format for spreadsheets)
./metrics --csv hello.c

# Analyze multiple files
./metrics hello.c metrics.c

# Multiple files in CSV format
./metrics --csv hello.c metrics.c
```

### Quick Demo

Run the demo script to see all features:
```bash
./demo.sh
```

### Metrics Collected

| Metric | Description |
|--------|-------------|
| Total Lines | Total number of lines in the file |
| Code Lines | Lines containing actual code |
| Blank Lines | Empty lines or lines with only whitespace |
| Comment Lines | Lines containing comments |
| Include Count | Number of #include statements |
| Function Count | Number of function definitions |
| Printf Count | Number of printf statements |
| Brace Count | Total number of { and } characters |

### Importing to Spreadsheets

The CSV output can be easily imported into popular spreadsheet applications:

1. **Excel**: File → Import → Text File → Select CSV file
2. **Google Sheets**: File → Import → Upload → Select CSV file
3. **LibreOffice Calc**: File → Open → Select CSV file

This allows for advanced analysis, charting, and comparison of code metrics across multiple files.