# Hello World
Test repo for JediMaster

## Code Metrics Analyzer

This repository includes a spreadsheet-like code metrics analyzer that can analyze C source files and display various metrics in a tabular format.

### Quick Start

```bash
# Build the analyzer
gcc -Wall -Wextra -o code_metrics code_metrics.c

# Analyze hello.c (default)
./code_metrics

# Analyze any C file
./code_metrics filename.c

# Use ASCII output for compatibility
./code_metrics --plain filename.c

# View help
./code_metrics --help

# Check version
./code_metrics --version
```

### Features

The analyzer displays metrics including:
- Line counts (total, code, blank, comments)
- Function definitions
- Include statements
- Printf statements
- Code-to-total and comment-to-total ratios
- Complexity estimation

See [CODE_METRICS.md](CODE_METRICS.md) for detailed documentation.

### Testing

Run the test suite:
```bash
./test/test_code_metrics.sh
```