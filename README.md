# Hello-World
Test repo for JediMaster

## Code Metrics Spreadsheet 📊

This repository now includes an interactive spreadsheet-like interface for analyzing code metrics of the `hello.c` file.

### Usage

1. **Generate Metrics**: Run the analyzer to create metrics data
   ```bash
   python3 metrics_analyzer.py
   ```

2. **View Interactive Spreadsheet**: Open `metrics.html` in your web browser
   ```bash
   # On most systems:
   open metrics.html
   # Or double-click the file in your file manager
   ```

### Features

- **Interactive Table**: Sort columns by clicking headers
- **Search Function**: Filter metrics using the search box
- **Detailed Analysis**: View breakdowns of functions, includes, and quality metrics
- **Responsive Design**: Works on desktop and mobile devices

### Metrics Analyzed

- **File Information**: Name, size, path
- **Line Counts**: Total, code, blank, comment lines
- **Structure**: Functions, includes, variables count
- **Quality**: Cyclomatic complexity, code ratios
- **Size**: Character and word counts

### Files

- `metrics_analyzer.py` - Python script that analyzes hello.c and generates metrics
- `metrics.html` - Interactive web interface for viewing metrics
- `metrics.json` - Generated metrics data (created when analyzer runs)
