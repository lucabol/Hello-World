# Hello World
Test repo for JediMaster

## Projects

This repository contains:
- **hello.c** - A simple C "Hello World" program
- **code_metrics** - A spreadsheet-like interface for analyzing C code metrics

## Code Metrics Tool

The Code Metrics Analyzer provides a spreadsheet-like view of code statistics:

```bash
# Build the tool
make code_metrics

# Run analysis on hello.c
./code_metrics

# Run analysis on any C file
./code_metrics filename.c
```

See [CODE_METRICS.md](CODE_METRICS.md) for detailed documentation.

## Building

```bash
# Build everything
make all

# Build just hello
make hello

# Build just code_metrics
make code_metrics

# Run validation tests
make test
```