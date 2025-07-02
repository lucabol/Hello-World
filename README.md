# Hello-World
Test repo for JediMaster

## 📊 Code Metrics Spreadsheet

This repository includes a comprehensive code metrics analysis tool that provides a spreadsheet-like interface for examining code quality and structure metrics of the `hello.c` file.

### Features

- **Comprehensive Metrics Analysis**: Analyzes lines of code, functions, includes, complexity, style metrics, and keyword usage
- **Interactive Spreadsheet Interface**: Web-based table with sorting, filtering, and search capabilities
- **Category-based Organization**: Metrics grouped by categories (Size, Quality, Complexity, Structure, etc.)
- **Visual Summary Dashboard**: Quick overview cards showing key statistics
- **Real-time Filtering**: Search and filter metrics by text or category
- **Responsive Design**: Works on desktop and mobile devices

### Usage

1. **Generate Metrics Data**:
   ```bash
   python3 code_metrics.py hello.c
   ```
   This creates a `metrics.json` file with detailed analysis of the C code.

2. **View Interactive Spreadsheet**:
   ```bash
   python3 -m http.server 8000
   ```
   Then open http://localhost:8000/metrics_spreadsheet.html in your browser.

3. **Explore the Interface**:
   - Use the search box to filter metrics by name or value
   - Select categories from the dropdown to focus on specific types of metrics
   - Click column headers to sort the data
   - View the summary dashboard for key statistics

### Metrics Analyzed

- **File Information**: Name, analysis date
- **Size Metrics**: Total lines, code lines, comment lines, blank lines, character count
- **Quality Metrics**: Code density, comment density
- **Complexity Metrics**: Cyclomatic complexity
- **Style Metrics**: Maximum and average line length
- **Structure Metrics**: Number of functions and includes
- **Function Details**: Individual function signatures and locations
- **Include Details**: Header files and their types (system/local)
- **Keyword Usage**: Frequency of C language keywords

### Files

- `hello.c` - The source C program to analyze
- `code_metrics.py` - Python script for extracting code metrics
- `metrics_spreadsheet.html` - Interactive web interface for viewing metrics
- `metrics.json` - Generated metrics data (created by running the Python script)
