# Code Metrics Spreadsheet

A lightweight, interactive spreadsheet-like interface for analyzing code metrics of the Hello World C program.

## Features

- 📊 **Interactive Spreadsheet Interface**: View code metrics in a sortable, tabular format
- 🔄 **Real-time Analysis**: Refresh metrics with a single click
- 📈 **Comprehensive Metrics**: 16+ different code quality and size metrics
- 🎯 **Summary Cards**: Quick overview of key metrics (lines, functions, complexity)
- ↕️ **Sortable Columns**: Click any column header to sort data
- 🎨 **Professional Styling**: Clean, modern interface with hover effects

## Available Metrics

### Size Metrics
- **Total Lines**: Complete line count including blanks
- **Code Lines**: Lines containing actual code
- **Non-Empty Lines**: Lines with code or comments
- **Blank Lines**: Empty or whitespace-only lines
- **Total Characters**: Character count including whitespace
- **Characters (no spaces)**: Character count excluding whitespace
- **Word Count**: Total number of words
- **Average Line Length**: Characters per non-empty line

### Structure Metrics
- **Functions**: Number of function definitions
- **Function Calls**: Number of function invocations
- **Curly Braces**: Total opening and closing braces
- **Semicolons**: Statement terminators

### Dependency Metrics
- **Include Statements**: Number of #include directives

### Quality Metrics
- **Complexity Score**: Simple complexity calculation
- **Comment Lines**: Documentation lines

### Literals
- **String Literals**: Number of string constants

## Quick Start

### Option 1: Launch Script (Recommended)
```bash
# Start the metrics interface (default port 8080)
./launch_metrics.sh

# Or specify a custom port
./launch_metrics.sh 3000
```

### Option 2: Manual Setup
```bash
# Start a web server
python3 -m http.server 8080

# Open in browser
# http://localhost:8080/code_metrics.html
```

## Usage

1. **Launch the Interface**: Use the launch script or manual setup
2. **View Metrics**: The interface automatically analyzes hello.c on load
3. **Sort Data**: Click column headers to sort by different metrics
4. **Refresh Analysis**: Click "🔄 Refresh Metrics" to re-analyze
5. **Explore**: Hover over rows for better visibility

## Technical Details

- **Pure HTML/CSS/JavaScript**: No external dependencies
- **Embedded Analysis**: Source code is embedded for analysis
- **Client-Side Processing**: All metrics calculated in the browser
- **Responsive Design**: Works on desktop and mobile
- **Modern Browser Support**: Uses ES6+ features

## Metrics Categories

- **Size**: Lines, characters, words, and length measurements
- **Structure**: Functions, calls, and code organization
- **Dependencies**: External includes and imports
- **Documentation**: Comments and documentation coverage
- **Literals**: String and numeric constants
- **Quality**: Complexity and maintainability indicators

## File Structure

```
code_metrics.html     # Main spreadsheet interface
launch_metrics.sh     # Launcher script
METRICS_README.md     # This documentation
```

## Browser Compatibility

- Chrome/Chromium 70+
- Firefox 65+
- Safari 12+
- Edge 79+

## Development

The interface is self-contained in a single HTML file with embedded CSS and JavaScript. To modify metrics or styling, edit `code_metrics.html` directly.

### Adding New Metrics

1. Add metric calculation in the `analyzeCode()` function
2. Update the `metricsData` array with the new metric
3. Optionally add to summary cards in `populateSummary()`

### Customizing Appearance

- Modify CSS variables in the `<style>` section
- Update colors, fonts, or layout as needed
- All styling is contained within the HTML file