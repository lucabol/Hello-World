# Hello World

Test repo for JediMaster

## Building and Running

This project uses a Makefile for build automation. By default, code is compiled with strict warning flags enabled to maintain code quality.

### Quick Start

```bash
make          # Build with development flags (-Wall -Wextra)
make test     # Build and run the program
```

### Build Targets

- `make` or `make dev` - Development build with warnings (default)
- `make standard` - Basic build with minimal flags  
- `make optimized` - Optimized build with `-O2`
- `make strict` - Strict compilation with maximum warnings
- `make debug` - Debug build with `-g` flag
- `make test` - Build and run the program
- `make lint` - Check code quality with strict flags
- `make clean` - Remove compiled binaries
- `make help` - Show all available targets

### Code Quality

This project enforces strict compilation standards. All code must compile cleanly with the following flags:

- `-Wall -Wextra` (enabled by default)
- `-Wpedantic -Wformat=2 -Wconversion -Wsign-conversion` (strict mode)

Run `make lint` or `make strict` to check your code against these standards.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for detailed guidelines on code quality standards and development workflow.