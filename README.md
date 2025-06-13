# Hello-World
Test repo for JediMaster

## Code Metrics Spreadsheet Tool

This repository now includes a spreadsheet-like interface for analyzing code metrics from the `hello.c` file.

### Features

The code metrics tool provides detailed analysis including:
- Lines of code (total, source, comment, blank)
- Function and variable counts
- Code complexity metrics
- File size and character counts
- Interactive tabular display

### Usage

You can run the tool in several ways:

1. **Using the launcher script (recommended):**
   ```bash
   ./run_metrics.sh
   ```

2. **Direct Python execution:**
   ```bash
   python3 code_metrics.py
   ```

### Interactive Menu

The tool provides an interactive menu with the following options:
1. **Show Full Metrics Table** - Display all metrics in a formatted table
2. **Show Summary** - Quick overview of key metrics
3. **Show Specific Metrics** - Select individual metrics to view
4. **Exit** - Close the tool

### Sample Output

The tool displays metrics in a spreadsheet-like format:

```
+---------------------+-------+---------------------------+
| Metric              | Value | Description               |
+---------------------+-------+---------------------------+
| File Name           | hello.c | Name of the analyzed file |
| Total Lines         | 8     | Total number of lines     |
| Source Lines        | 5     | Actual source code lines  |
| Functions           | 1     | Number of functions       |
| Cyclomatic Complexity | 1   | Measure of code complexity |
+---------------------+-------+---------------------------+
```

### Requirements

- Python 3.x
- The `hello.c` file must be present in the same directory
