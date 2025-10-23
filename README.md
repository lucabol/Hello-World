# Hello World
Test repo for JediMaster

## Code Metrics Tool

This repository includes a spreadsheet-like code metrics analyzer that can analyze C source files and display various metrics in a tabular format.

See [CODE_METRICS_README.md](CODE_METRICS_README.md) for details.

Quick start:
```bash
make code_metrics
./code_metrics hello.c
```

### Running Tests

Run all tests (including code_metrics tests):
```bash
make test-all
```

Run only code_metrics tests:
```bash
make test-metrics
```

### CI/CD

The project includes GitHub Actions workflows that build and test the code with both GCC and Clang, using strict compiler warnings (`-Wall -Wextra -Wpedantic -Werror`).