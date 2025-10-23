# Code Metrics Spreadsheet Tool

A spreadsheet-like interface for analyzing C code and displaying various code metrics in an interactive, tabular format.

## Features

- 📊 **Comprehensive Metrics**: Lines of code, functions, complexity, and more
- 🎨 **Interactive Dashboard**: Beautiful, sortable, and filterable spreadsheet interface
- 📤 **Export Options**: Export metrics to CSV, JSON, or print as PDF
- 🔍 **Function Analysis**: Detailed breakdown of each function
- 📈 **Visual Cards**: Color-coded metric cards with hover effects
- 🎯 **Real-time Filtering**: Search and filter functions instantly

## Installation

No installation required! The tool uses only Node.js built-in modules.

## Usage

### Basic Analysis

```bash
npm run analyze
```

This will analyze `hello.c` and generate:
- `metrics.json` - Raw metrics data
- `metrics-report.html` - Interactive HTML dashboard

### Analyze a Specific File

```bash
node metrics-analyzer.js path/to/your-file.c
```

### View the Dashboard

After running the analysis, open `metrics-report.html` in your web browser to see the interactive spreadsheet-like interface.

Alternatively, run a local server:

```bash
npm run serve
```

Then open http://localhost:3000 in your browser.

## Metrics Collected

### Summary Metrics
- **Total Lines**: Total number of lines in the file
- **Code Lines**: Lines containing actual code
- **Comment Lines**: Lines with comments
- **Blank Lines**: Empty lines
- **Functions**: Number of functions defined
- **Characters**: Total character count
- **Code Density**: Percentage of code lines vs total lines
- **Comment Density**: Percentage of comment lines vs total lines
- **Average Line Length**: Average characters per line

### Function Metrics
For each function, the tool tracks:
- Function name
- Return type
- Parameters
- Starting line number
- Lines of code in function

### Additional Metrics
- Include directives
- Printf calls
- Curly braces count
- Semicolons count
- Presence of main function

## Interactive Features

### Sorting
Click on any column header in the functions table to sort by that column. Click again to reverse the sort order.

### Filtering
Use the filter input box to search for specific functions by name, return type, or parameters. The table updates in real-time as you type.

### Exporting
- **CSV**: Export all metrics to a CSV file for use in Excel or other spreadsheet tools
- **JSON**: Export raw metrics data in JSON format
- **Print/PDF**: Use the print button to generate a PDF report

## Examples

### Example Output for hello.c

```
=== Code Metrics Summary ===
File: hello.c
Total Lines: 6
Code Lines: 4
Comment Lines: 0
Blank Lines: 2
Functions: 1
Code Density: 66.67%
Comment Density: 0.00%

Functions:
  - main() [int] at line 3
```

## Browser Compatibility

The HTML dashboard works in all modern browsers:
- Chrome/Edge (recommended)
- Firefox
- Safari
- Opera

## License

MIT
