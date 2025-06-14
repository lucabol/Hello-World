# Code Metrics Spreadsheet

A spreadsheet-like interface for analyzing code metrics of C files, specifically designed to analyze `hello.c`.

## Features

- 📊 **Interactive Spreadsheet Interface**: Display metrics in a tabular format similar to a spreadsheet
- 🔄 **Real-time Analysis**: Analyze hello.c and display various code metrics
- 📈 **Comprehensive Metrics**: Including lines of code, complexity, functions, variables, and more
- 📋 **Export Functionality**: Export metrics to CSV format
- 🎨 **Modern UI**: Clean, responsive interface with hover effects and interactive elements
- ⌨️ **Keyboard Shortcuts**: Ctrl+R to refresh, Ctrl+S to save

## Quick Start

### Option 1: Web Server (Recommended)
Run the complete solution with a web server:

```bash
python3 run_server.py
```

Then open your browser to `http://localhost:8000`

### Option 2: Command Line Analysis
Just run the analyzer to get JSON output:

```bash
python3 analyze_metrics.py
```

### Option 3: Open HTML File Directly
Open `metrics_spreadsheet.html` directly in your browser (will use simulated data).

## Code Metrics Analyzed

The system analyzes the following metrics for `hello.c`:

- **File Information**: Name, size, total lines
- **Line Analysis**: Code lines, comment lines, blank lines
- **Code Structure**: Functions, variables, includes, statements
- **Quality Metrics**: Cyclomatic complexity, comment ratio, code ratio
- **Function Details**: Individual function analysis with parameters and return types

## Files

- `analyze_metrics.py` - Python script that analyzes C files and extracts metrics
- `metrics_spreadsheet.html` - Interactive web interface for displaying metrics
- `run_server.py` - Web server that serves the interface and provides API endpoints
- `hello.c` - The C file being analyzed

## Usage

1. **Refresh Metrics**: Click the "🔄 Refresh Metrics" button to analyze hello.c
2. **Export Data**: Click "📊 Export to CSV" to download metrics as a CSV file
3. **View Functions**: Click "🔍 Function Details" for detailed function information
4. **Interactive Features**: 
   - Hover over rows for highlighting
   - Click on metric values to copy them to clipboard
   - Use keyboard shortcuts for quick actions

## Example Output

The system will analyze `hello.c` and display metrics such as:

- Total Lines: 8
- Code Lines: 5
- Functions: 1 (main)
- Includes: 1 (#include <stdio.h>)
- Cyclomatic Complexity: 1
- And more...

## Requirements

- Python 3.x
- Modern web browser
- No additional dependencies required

## Notes

- The analyzer is specifically designed for C files but can be extended for other languages
- The interface works both with live analysis (when server is running) and with simulated data
- All metrics are calculated using simple but effective heuristics appropriate for basic C code analysis