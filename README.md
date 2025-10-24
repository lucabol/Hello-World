# Hello World
Test repo for JediMaster

## Plugin System

This repository includes a plugin architecture that allows external developers to extend hello.c functionality without modifying the core file.

### Quick Start - Plugin System

```bash
# Build and run basic hello
make
./hello

# See plugin system demonstrations
make demo
```

### Plugin Documentation

- **[PLUGIN_QUICKSTART.md](PLUGIN_QUICKSTART.md)** - Quick start for plugin authors
- **[PLUGIN_README.md](PLUGIN_README.md)** - Overview and examples
- **[PLUGIN_SYSTEM.md](PLUGIN_SYSTEM.md)** - Complete API documentation
- **[CODE_DIFFS.md](CODE_DIFFS.md)** - Implementation details and code review
- **[IMPLEMENTATION_SUMMARY.md](IMPLEMENTATION_SUMMARY.md)** - Technical summary

### Plugin Features

- ✅ Zero-modification extensibility
- ✅ Plugin chaining support
- ✅ Type-safe callback-based API
- ✅ No external dependencies
- ✅ Production-ready with full error handling
- ✅ Comprehensive examples and documentation

### Example Plugins Included

- **Uppercase** - Converts to uppercase
- **Reverse** - Reverses the message
- **Exclaim** - Adds extra exclamation marks
- **Prefix** - Adds a prefix tag
- **ROT13** - Caesar cipher transformation
- **Leetspeak** - Letter-to-number conversion

See the documentation for how to create your own plugins!

## Code Metrics Analyzer

This repository also includes a spreadsheet-like code metrics analyzer that can analyze C source files and display various metrics in a tabular format.

### Quick Start - Code Metrics

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

### Code Metrics Features

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
