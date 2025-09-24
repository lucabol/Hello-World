# Code Metrics Spreadsheet

A sophisticated web-based tool that analyzes C source code and presents detailed metrics in an interactive spreadsheet interface.

## Features

### 📊 Comprehensive Code Analysis
- **Line counting**: Total lines, code lines, blank lines, comment lines
- **Structural metrics**: Function count, include directives, complexity estimation
- **Character analysis**: Total characters, bytes, average chars per code line
- **Code patterns**: Printf calls, semicolons, braces, string literals
- **Quality metrics**: Code density, comment density

### 🖥️ Interactive Spreadsheet Interface
- **Professional design**: Modern, clean interface with color-coded metrics
- **Real-time sorting**: Click column headers to sort by any metric
- **Live filtering**: Search and filter metrics dynamically
- **Export functionality**: Download data as CSV for external analysis
- **Responsive design**: Works on desktop, tablet, and mobile devices
- **Summary cards**: Key metrics highlighted at the top

### 🔧 Easy to Use
- **Command line interface**: Simple script wrapper for quick analysis
- **Standalone**: No external dependencies beyond Python 3
- **Fast**: Instant analysis for small to medium C files
- **Extensible**: Easy to modify for additional metrics

## Quick Start

### Method 1: Use the wrapper script (Recommended)
```bash
# Analyze hello.c (default)
./analyze_metrics.sh

# Analyze a specific file
./analyze_metrics.sh path/to/your/file.c
```

### Method 2: Direct Python usage
```bash
# Analyze hello.c
python3 metrics_analyzer.py hello.c

# Analyze another file
python3 metrics_analyzer.py src/main.c
```

## Output Files

The tool generates two files:

1. **`code_metrics_spreadsheet.html`** - Interactive web interface
2. **`metrics.json`** - Raw data in JSON format for programmatic use

## Viewing Results

### Web Browser (Recommended)
1. Open `code_metrics_spreadsheet.html` in any modern web browser
2. Or start a local server: `python3 -m http.server 8080` and visit http://localhost:8080

### Features of the Web Interface

#### Summary Dashboard
- **Total Lines**: Overall file size
- **Code Lines**: Actual executable code
- **Functions**: Number of function definitions
- **Code Density**: Percentage of lines that contain code

#### Interactive Table
- **Sorting**: Click any column header to sort data
- **Filtering**: Type in the search box to filter metrics
- **Export**: Download filtered data as CSV
- **Descriptions**: Detailed explanations for each metric

#### Supported Browsers
- Chrome/Chromium (recommended)
- Firefox
- Safari
- Edge

## Metrics Explained

| Metric | Description | Type |
|--------|-------------|------|
| **Total Lines** | Complete line count including blanks | Integer |
| **Code Lines** | Lines containing executable code | Integer |
| **Blank Lines** | Empty or whitespace-only lines | Integer |
| **Comment Lines** | Lines with comments (// or /* */) | Integer |
| **Function Count** | Number of function definitions | Integer |
| **Include Count** | Number of #include directives | Integer |
| **Printf Count** | Number of printf() function calls | Integer |
| **String Literals** | Count of quoted strings | Integer |
| **Brace Count** | Total { and } characters | Integer |
| **Semicolon Count** | Total semicolon characters | Integer |
| **Code Density** | Percentage of lines that are code | Percentage |
| **Comment Density** | Percentage of lines with comments | Percentage |
| **Avg Chars/Line** | Average characters per code line | Decimal |
| **Estimated Complexity** | Basic complexity score | Integer |

## Technical Details

### Requirements
- **Python 3.6+** (uses f-strings and modern features)
- **Web browser** (for viewing results)
- **No external libraries** required

### Architecture
- **Pure Python**: No dependencies beyond standard library
- **Static HTML/CSS/JS**: No server required for viewing
- **JSON output**: Machine-readable data format
- **Responsive design**: CSS Grid and Flexbox

### File Structure
```
metrics_analyzer.py          # Core analysis engine
analyze_metrics.sh           # Convenient wrapper script
code_metrics_spreadsheet.html  # Generated web interface
metrics.json                 # Generated data file
```

## Customization

### Adding New Metrics
Edit the `analyze_c_file()` function in `metrics_analyzer.py`:

```python
# Add your custom metric
metrics["your_metric"] = your_calculation_here

# Add description in generate_html_interface()
metric_descriptions["your_metric"] = "Description of your metric"
```

### Styling the Interface
The CSS is embedded in the HTML template. Key classes:
- `.metric-value` - Numeric values
- `.metric-percentage` - Percentage values  
- `.metric-name` - Metric names
- `.summary-stats` - Top dashboard cards

### Extending for Other Languages
The tool can be adapted for other languages by:
1. Modifying the regex patterns in `analyze_c_file()`
2. Updating the metric descriptions
3. Adjusting the complexity calculation

## Use Cases

### Development
- **Code quality assessment**: Track metrics over time
- **Refactoring planning**: Identify areas for improvement
- **Team standards**: Establish coding guidelines

### Education  
- **Learning C**: Understand code structure
- **Code review**: Systematic analysis approach
- **Best practices**: Demonstrate clean code principles

### Analysis
- **Legacy code**: Understand unfamiliar codebases
- **Migration planning**: Assess complexity before changes
- **Documentation**: Generate code statistics

## Limitations

- **C-specific**: Designed for C language patterns
- **Basic complexity**: Simple heuristic, not full cyclomatic complexity
- **Single file**: Analyzes one file at a time
- **Comment detection**: May not handle all comment edge cases

## Contributing

The tool is designed to be easily extensible. Common improvements:
- Additional language support
- More sophisticated complexity analysis  
- Multi-file analysis capabilities
- Integration with build systems
- Additional export formats

## License

Part of the Hello-World project - see main repository for license details.