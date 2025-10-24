# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]

### Added - 2025-10-24

- **Code Metrics Analyzer**: New spreadsheet-like interface for analyzing C source files
  - Displays comprehensive code metrics in tabular format
  - Tracks total lines, code lines, blank lines, comments, function definitions, includes, printf statements
  - Calculates derived metrics: code-to-total ratio, comment-to-total ratio, complexity estimation
  - Supports both Unicode box-drawing characters (default) and ASCII mode (`--plain`/`--ascii`)
  - Standard CLI options: `--help`, `--version`, `--plain`/`--ascii`
  - Robust edge case handling: strings with comment markers, multi-line comments, mixed line endings
  - Comprehensive test suite with 11 tests including edge cases
  - CI integration with strict compilation flags for both GCC and Clang
  - Complete documentation in CODE_METRICS.md
  - Fixes issue #1771

### Security

- All buffers are stack-allocated with fixed size limits (MAX_LINE_LENGTH=1024, MAX_FILENAME=256)
- Explicit null termination for all string operations
- No unsafe functions (gets, strcpy, sprintf without bounds)
- Compiles cleanly with strict security flags: `-Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c11`

### Files Added

- `code_metrics.c` - Main implementation
- `CODE_METRICS.md` - Complete documentation
- `test/test_code_metrics.sh` - Test suite with 11 tests
- `test/test_edge_cases.c` - Test file with edge cases
- `CHANGELOG.md` - This file

### Files Modified

- `.github/workflows/ci.yml` - Added CI steps for code_metrics
- `.gitignore` - Added code_metrics binary
- `README.md` - Added quick start guide for code_metrics
