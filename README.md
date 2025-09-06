# Hello World

Test repo for JediMaster

## Code Metrics Tool

This project now includes a spreadsheet-like interface for analyzing code metrics. The metrics tool provides detailed analysis of C source files in a tabular format.

### Features

- **Interactive Menu System**: Navigate through different options
- **Comprehensive Metrics**: Analyze lines of code, functions, variables, and more
- **Spreadsheet-like Display**: Beautiful tabular output with calculated statistics
- **File Analysis**: Analyze any C source file, not just hello.c

### Usage

#### Building
```bash
# Build both programs
make all

# Or build individually
gcc -Wall -Wextra -o hello hello.c
gcc -Wall -Wextra -o metrics metrics.c
```

#### Running the Metrics Tool
```bash
# Run interactively
./metrics

# Or use the Makefile target
make run-metrics
```

#### Available Metrics
- Total, code, blank, and comment lines
- File size and character count
- Number of functions, variables, and includes
- Code coverage percentages
- Average characters per line

### Menu Options
1. **Display Code Metrics Table** - Shows detailed analysis of hello.c
2. **Analyze Different File** - Analyze any C source file
3. **Exit** - Close the application

### Example Output
The tool displays metrics in a professional spreadsheet-like format with Unicode box drawing characters for clear visual separation of data.