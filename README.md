# Hello World
Test repo for JediMaster

## Projects

- **hello.c** - Simple Hello World program in C
- **Code Metrics Tool** - Spreadsheet-like interface for analyzing code metrics (see [METRICS_README.md](METRICS_README.md))

## Quick Start: Code Metrics Tool

Analyze C source files and display metrics in a spreadsheet-like format:

```bash
# Build the tool
make metrics_tool

# Analyze hello.c interactively
./metrics_tool

# Get CSV output for automation
./metrics_tool --csv hello.c
```

**Example output:**
```
Filename,Total Lines,Code Lines,Blank Lines,Comment Lines,Functions,Includes,Max Line Length
hello.c,8,7,1,0,1,1,41
```

For full documentation, see [METRICS_README.md](METRICS_README.md).