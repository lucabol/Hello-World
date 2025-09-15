# Code Metrics Spreadsheet Analyzer

This tool provides a spreadsheet-like interface for analyzing C source code metrics. It analyzes hello.c and displays various code metrics in a tabular, interactive format.

## Features

- **Interactive Menu System**: User-friendly interface with numbered options
- **Comprehensive Metrics Analysis**: Analyzes multiple code quality indicators
- **Spreadsheet-like Display**: Professional tabular format with Unicode borders
- **Detailed Breakdown**: Percentage distributions and complexity indicators  
- **Export Functionality**: Save metrics to text file for external use

## How to Use

### 1. Compile the Tool
```bash
gcc -Wall -Wextra -o metrics_analyzer metrics_analyzer.c
```

### 2. Run the Analyzer
```bash
./metrics_analyzer
```

### 3. Interactive Options

**Option 1: Analyze hello.c**
- Performs comprehensive analysis of the hello.c source file
- Displays results in a professional spreadsheet-like table
- Shows all key metrics in an organized format

**Option 2: Display Detailed Breakdown**
- Shows percentage distribution of line types
- Displays code complexity indicators
- Provides per-function analysis ratios

**Option 3: Export Metrics to Text**
- Saves analysis results to `metrics_report.txt`
- Creates a plain text version for documentation or sharing

**Option 4: Exit**
- Safely exits the application

## Metrics Analyzed

| Metric | Description |
|--------|-------------|
| **Total Lines** | Complete line count including all content |
| **Code Lines** | Executable code lines (excludes blanks and comments) |
| **Blank Lines** | Empty lines and whitespace-only lines |
| **Comment Lines** | Single-line (//) and multi-line (/* */) comments |
| **Include Statements** | #include and # include directives |
| **Function Definitions** | Detected function declarations |
| **Variable Declarations** | Local and global variable definitions |
| **Printf Statements** | Output function calls |
| **File Size** | Total file size in bytes |

## Sample Output

```
╔═══════════════════════════════════════════════════════════════════════════════╗
║                          CODE METRICS ANALYSIS REPORT                        ║
╠═══════════════════════════════════════════════════════════════════════════════╣
║ File: hello.c                                                                 ║
╠═══════════════════════════════════════════════════════════════════════════════╣
║                                  METRICS                                     ║
╠═══════════════════════════════════════════╤═══════════════════════════════════╣
║ Metric                                    │ Value                             ║
╠═══════════════════════════════════════════╪═══════════════════════════════════╣
║ Total Lines                               │ 5                                 ║
║ Code Lines (non-blank, non-comment)      │ 4                                 ║
║ Blank Lines                               │ 1                                 ║
║ Comment Lines                             │ 0                                 ║
║ Include Statements                        │ 1                                 ║
║ Function Definitions                      │ 1                                 ║
║ Variable Declarations                     │ 0                                 ║
║ Printf Statements                         │ 1                                 ║
║ File Size (bytes)                         │ 63                                ║
╚═══════════════════════════════════════════╧═══════════════════════════════════╝
```

## Technical Implementation

- **Language**: C with standard library functions
- **File Analysis**: Line-by-line parsing with pattern matching
- **Display Format**: Unicode box-drawing characters for professional presentation
- **Interactive Loop**: Menu-driven interface with input validation
- **Memory Management**: Efficient stack-based allocation for metrics data

## Files Created

- `metrics_analyzer.c` - Main analyzer source code
- `metrics_analyzer` - Compiled executable (gitignored)
- `metrics_report.txt` - Exported metrics report (gitignored)

This tool successfully implements the requested spreadsheet-like interface for code metrics analysis with professional formatting and interactive functionality.