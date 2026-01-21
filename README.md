# Hello World

Test repo for JediMaster

## Quick Start

```bash
# Build
make

# Run tests
make test

# Generate code coverage report
make coverage
```

## Documentation

### Architecture Decision Records

For detailed documentation on key architectural decisions, see our [Architecture Decision Records (ADRs)](docs/adr/README.md):

- [ADR-001: No Trailing Newline in Output](docs/adr/ADR-001-no-trailing-newline.md) - Why the program outputs without a trailing newline
- [ADR-002: Byte-Level Validation](docs/adr/ADR-002-byte-level-validation.md) - Our validation methodology using hex dumps
- [ADR-003: Dual Compiler Support](docs/adr/ADR-003-dual-compiler-support.md) - Why we support both GCC and Clang

### Code Coverage

See [Coverage Documentation](docs/COVERAGE.md) for details on:
- Generating coverage reports locally
- CI/CD coverage integration
- Codecov setup (optional)
- Current coverage: **100%** line coverage