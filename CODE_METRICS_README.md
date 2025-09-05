# Code Metrics Spreadsheet

A spreadsheet-like interface for analyzing code metrics of `hello.c`.

## Features

### 📊 Interactive Metrics Table
- **Sortable columns**: Click any column header to sort data
- **Visual indicators**: Sort direction shown with ▲/▼ arrows
- **10 different metrics** analyzed:
  - Lines of Code
  - Non-empty Lines
  - Functions
  - Printf Statements
  - Variable Declarations
  - Include Statements
  - Return Statements
  - Cyclomatic Complexity
  - Character Count
  - Indentation Levels

### 🎯 Metric Categories
- **Size**: Code size metrics (lines, characters)
- **Structure**: Code organization metrics (functions)
- **Statements**: Statement analysis (printf, return)
- **Variables**: Variable usage analysis
- **Dependencies**: External dependencies (includes)
- **Complexity**: Code complexity measures
- **Style**: Code style metrics (indentation)

### 🚀 Interactive Controls
- **🔍 Analyze Code**: Run fresh analysis of hello.c
- **🔄 Refresh**: Re-analyze the code
- **📤 Export CSV**: Download metrics as CSV file

## How to Use

1. **Open the interface**: Open `code_metrics.html` in a web browser
2. **Automatic analysis**: The tool automatically analyzes `hello.c` on load
3. **Interact with data**:
   - Click column headers to sort
   - Use buttons to refresh or export data
4. **Export results**: Click "Export CSV" to download metrics data

## Technical Details

- **Pure HTML/CSS/JavaScript**: No external dependencies
- **Client-side analysis**: Code parsing done in browser
- **Responsive design**: Works on desktop and mobile
- **CSV export**: Standard format for spreadsheet applications

## Metrics Explained

| Metric | Description |
|--------|-------------|
| Lines of Code | Total number of lines in the file |
| Non-empty Lines | Lines containing actual code or comments |
| Functions | Number of function definitions |
| Printf Statements | Number of printf function calls |
| Variable Declarations | Number of integer variable declarations |
| Include Statements | Number of header file inclusions |
| Return Statements | Number of return statements |
| Cyclomatic Complexity | Measure of code complexity based on control flow |
| Character Count | Total number of characters in the file |
| Indentation Levels | Maximum indentation depth (assuming 4 spaces per level) |

## Example Results for hello.c

The current `hello.c` file shows:
- 9 lines of code
- 1 function (main)
- 2 printf statements
- 1 variable declaration
- Cyclomatic complexity of 1 (simple linear flow)
- 154 total characters