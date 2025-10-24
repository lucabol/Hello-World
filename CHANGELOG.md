# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]

### Added - 2025-10-24

- **Plugin System**: Extensible plugin architecture for hello.c
  - Enables external developers to add features and transformations without modifying core code
  - Clean callback-based API with function pointers
  - Plugin chaining support - multiple plugins can be composed in sequence
  - Registry-based plugin management with registration/unregistration
  - 85 comprehensive unit tests covering all edge cases
  - Safe string operations with NUL-termination guarantees
  - Buffer overflow protection throughout
  - Compile-time configurable capacity (PLUGIN_MAX_COUNT, default 10)
  - Complete API documentation with examples
  - CI integration with GCC, Clang, ASAN/UBSAN testing
  - Example plugins: uppercase, reverse, exclaim, prefix, ROT13, leetspeak
  - Demonstration programs: plugin_demo, hello_with_plugins, custom_plugin_example
  - Thread-safety documentation with pthread synchronization examples
  - Fixes issue #1774

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

**Plugin System:**
- `plugin.h` - Public API header with error codes and function declarations
- `plugin.c` - Core plugin registry and execution engine
- `plugins_examples.c` - Built-in example plugins (uppercase, reverse, exclaim, prefix)
- `plugin_demo.c` - Comprehensive demonstration program
- `hello_with_plugins.c` - Integration example with hello.c
- `custom_plugin_example.c` - Tutorial for creating custom plugins
- `test/test_plugin_unit.c` - 85 unit tests covering all edge cases
- `test/test_plugin_system.sh` - Plugin system validation script
- `.github/workflows/plugin-ci.yml` - CI workflow for plugin system
- `PLUGIN_SYSTEM.md` - Complete API reference and usage guide
- `PLUGIN_QUICKSTART.md` - Quick start guide for plugin authors
- `PLUGIN_README.md` - Overview and getting started
- `CODE_DIFFS.md` - Detailed code diffs and implementation details
- `IMPLEMENTATION_SUMMARY.md` - Project overview and statistics
- `REVIEW_RESPONSE.md` - Detailed response to code review items

**Code Metrics Analyzer:**
- `code_metrics.c` - Main implementation
- `CODE_METRICS.md` - Complete documentation
- `test/test_code_metrics.sh` - Test suite with 11 tests
- `test/test_edge_cases.c` - Test file with edge cases
- `CHANGELOG.md` - This file

### Files Modified

**Plugin System:**
- `hello.c` - Modified to integrate plugin system (10 lines changed, preserves original behavior)
- `Makefile` - Added targets for plugin system: strict, unit-test, asan, plugin_demo, hello-plugins, custom_plugin_example, ci
- `test/validate.sh` - Updated to compile with plugin.c
- `.gitignore` - Added plugin system binaries
- `README.md` - Added plugin system documentation links

**Code Metrics:**
- `.github/workflows/ci.yml` - Added CI steps for code_metrics
- `.gitignore` - Added code_metrics binary
- `README.md` - Added quick start guide for code_metrics
