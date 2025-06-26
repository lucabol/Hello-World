# Hello-World
Test repo for JediMaster

## 📊 Code Metrics Spreadsheet Tool

This repository now includes a spreadsheet-like interface for analyzing code metrics of C files. The tool provides both command-line analysis and an interactive HTML interface.

### Features

- **Comprehensive Code Analysis**: Analyzes various metrics including:
  - Lines of code (total, code-only, comments)
  - File size and structure metrics
  - Function and variable counts
  - Include statements
  - Printf calls
  - Cyclomatic complexity
  - Code structure metrics (braces, semicolons)

- **Interactive Spreadsheet Interface**: 
  - Sortable columns
  - Filterable data
  - Summary statistics
  - Responsive design
  - Real-time data refresh

### Quick Start

1. **Run the analyzer and open the spreadsheet**:
   ```bash
   ./run_metrics.sh
   ```

2. **Or run components separately**:
   ```bash
   # Analyze C files
   python3 code_metrics.py
   
   # View in browser
   python3 -m http.server 8000
   # Then open: http://localhost:8000/metrics_spreadsheet.html
   ```

### Files

- `hello.c` - Original C program
- `code_metrics.py` - Python script for code analysis
- `metrics_spreadsheet.html` - Interactive HTML spreadsheet interface
- `run_metrics.sh` - Convenient launcher script
- `code_metrics.json` - Generated metrics data (created after running analysis)

### Usage Examples

**Command Line Analysis**:
```bash
$ python3 code_metrics.py
Code Metrics Analyzer
==================================================

Analyzing hello.c...

Analysis complete. Found 1 file(s).

Code Metrics Table:
========================================================================================================================
| File Name | Total Lines | Code Lines | Comment Lines | Non-Empty Lines | File Size (bytes) | Includes | Functions | Variables | Printf Calls | Complexity | Opening Braces | Closing Braces | Semicolons |
|-----------|-------------|------------|---------------|-----------------|-------------------|----------|-----------|-----------|--------------|------------|----------------|----------------|------------|
| hello.c   | 8           | 5          | 0             | 5               | 80                | 1        | 1         | 1         | 1            | 1          | 1              | 1              | 2          |
|-----------|-------------|------------|---------------|-----------------|-------------------|----------|-----------|-----------|--------------|------------|----------------|----------------|------------|
```

**Interactive Spreadsheet**: Open `metrics_spreadsheet.html` in your browser for a full-featured interface with sorting, filtering, and summary statistics.

### Requirements

- Python 3.x (for analysis script)
- Modern web browser (for HTML interface)
- No additional dependencies required
