# Hello World

Test repo for JediMaster

## Projects

This repository contains two C programs:

1. **hello.c** - A simple "Hello World" program
2. **code_metrics.c** - A robust file analysis tool with comprehensive error handling

## Performance Benchmarking

This repository includes a comprehensive performance benchmarking system that tracks compilation time, binary size, and execution time across different compilers and optimization levels.

See [benchmarks/README.md](benchmarks/README.md) for detailed documentation.

## Quick Start

### Building
```bash
make          # Build both programs
make hello    # Build hello only
make code_metrics  # Build code_metrics only
```

### Testing
```bash
make test            # Run all tests
make test-hello      # Test hello.c only
make test-metrics    # Test code_metrics.c only
```

### Performance Benchmarks
```bash
make benchmark        # Run performance benchmarks
make benchmark-compare  # Compare with baseline
```

## Documentation

- [Code Metrics Tool Documentation](docs/CODE_METRICS.md) - Comprehensive guide to the code metrics analyzer
- [Benchmark Documentation](benchmarks/README.md) - Performance benchmarking system

## Architecture Decision Records

For detailed documentation on key architectural decisions, see our [Architecture Decision Records (ADRs)](docs/adr/README.md):

- [ADR-001: No Trailing Newline in Output](docs/adr/ADR-001-no-trailing-newline.md) - Why the program outputs without a trailing newline
- [ADR-002: Byte-Level Validation](docs/adr/ADR-002-byte-level-validation.md) - Our validation methodology using hex dumps
- [ADR-003: Dual Compiler Support](docs/adr/ADR-003-dual-compiler-support.md) - Why we support both GCC and Clang
