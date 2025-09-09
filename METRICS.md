# Code Metrics Analyzer

This project includes a spreadsheet-like interface for analyzing code metrics of C source files.

## Usage

### Build and Run the Analyzer

1. **Compile the metrics analyzer:**
   ```bash
   gcc -Wall -Wextra -o metrics_analyzer metrics_analyzer.c
   ```

2. **Analyze hello.c (default):**
   ```bash
   ./metrics_analyzer
   ```

3. **Analyze any C file:**
   ```bash
   ./metrics_analyzer your_file.c
   ```

### View the Interactive Report

After running the analyzer, open the generated `code_metrics.html` file in your web browser:

```bash
# Option 1: Open directly (if your system supports it)
open code_metrics.html

# Option 2: Use a local web server
python3 -m http.server 8000
# Then visit: http://localhost:8000/code_metrics.html
```

## Features

### Metrics Analyzed

The analyzer tracks the following code metrics:

- **Total Lines** - Total number of lines in the file
- **Code Lines** - Lines containing actual code
- **Comment Lines** - Lines containing comments
- **Blank Lines** - Empty or whitespace-only lines
- **Include Statements** - Number of `#include` directives
- **Functions** - Number of function definitions
- **Variables** - Number of variable declarations
- **Statements** - Number of statements (semicolons)
- **Braces** - Number of opening/closing braces
- **Code Ratio** - Percentage of lines that are code
- **Comment Ratio** - Percentage of lines that are comments

### Interactive Features

The HTML interface provides:

- **Sortable columns** - Click any column header to sort by that metric
- **Responsive design** - Works on desktop and mobile browsers
- **Professional styling** - Clean, spreadsheet-like appearance
- **Hover effects** - Visual feedback for interactive elements

## Example Output

For the default `hello.c` file:

```
=== Code Metrics Summary ===
Total Lines: 5
Code Lines: 4
Comment Lines: 0
Blank Lines: 1
Include Statements: 1
Functions: 1
Variables: 0
Statements: 1
Braces: 2
```

## Technical Details

### File Analysis

The analyzer performs line-by-line parsing and uses pattern matching to identify:

- Preprocessor directives (`#include`, `# include`)
- Function definitions (patterns with parentheses and braces)
- Variable declarations (type keywords followed by identifiers)
- Comments (both `//` and `/* */` styles)
- Code structure elements (braces, semicolons)

### HTML Generation

The tool generates a complete HTML document with:

- Embedded CSS for styling
- JavaScript for interactive sorting
- Responsive table layout
- Professional color scheme

## Integration

This tool can be integrated into build processes or used for code quality analysis:

```bash
# Quick analysis and report generation
gcc -Wall -Wextra -o metrics_analyzer metrics_analyzer.c && ./metrics_analyzer hello.c
```

The generated HTML report can be integrated into documentation systems or shared with team members for code review purposes.