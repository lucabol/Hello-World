# Hello World
Test repo for JediMaster

## Code Metrics Spreadsheet

This repository includes a spreadsheet-like tool for analyzing C code metrics. See [METRICS_SPREADSHEET.md](METRICS_SPREADSHEET.md) for details.

### Quick Start

```bash
# Build the metrics spreadsheet
gcc -Wall -Wextra -o metrics_spreadsheet metrics_spreadsheet.c

# Analyze hello.c
./metrics_spreadsheet hello.c
```

The tool provides an interactive spreadsheet interface showing code metrics like lines of code, functions, includes, and more.