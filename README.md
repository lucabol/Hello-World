# Hello World

Test repo for JediMaster

## Plugin System

This repository now includes an extensible plugin system that allows developers to add custom transformations to the hello.c output without modifying the core code.

### Quick Start

```bash
# Build and run the original hello program
make hello
./hello

# Build and run the plugin demo
make demo

# Run plugin tests
make test
```

### Documentation

- **[Quick Start Guide](QUICK_START.md)** - Get started in 5 minutes
- **[Plugin Guide](PLUGIN_GUIDE.md)** - Complete documentation
- **[Implementation Summary](IMPLEMENTATION_SUMMARY.md)** - Technical details

### Features

✅ Clean plugin API for extending functionality  
✅ Example plugins (uppercase, reverse, prefix)  
✅ Plugin chaining support  
✅ Comprehensive test coverage (20 unit tests)  
✅ Full documentation and examples  
✅ Zero changes to original hello.c code  

For more information, see the [Plugin Guide](PLUGIN_GUIDE.md).