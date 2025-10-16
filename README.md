# Hello World
Test repo for JediMaster

## Plugin System

This repository now includes a plugin architecture that allows external developers to extend hello.c functionality without modifying the core file. See [PLUGIN_SYSTEM.md](PLUGIN_SYSTEM.md) for complete documentation.

### Quick Start

```bash
# Build and run (default, no plugins)
make
./hello
# Output: Hello world!

# Build with uppercase plugin
make hello-uppercase
./hello-uppercase
# Output: HELLO WORLD!

# Run demo
make demo
./plugin_demo

# Run tests
make test
make test-plugins
```

See `make help` for all available build targets.
