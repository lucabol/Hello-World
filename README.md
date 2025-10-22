# Hello World

Test repo for JediMaster

## Plugin System

This repository includes a plugin architecture for extending the hello.c functionality. See [PLUGIN_GUIDE.md](PLUGIN_GUIDE.md) for detailed documentation.

### Quick Start

```bash
# Build and run default program
make
./hello

# Build and test plugin examples
make examples
./hello-with-uppercase
./hello-with-reverse
./hello-with-chain
```