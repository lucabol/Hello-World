# Hello World

Test repo for JediMaster

## Building

Build the program with:
```bash
make
```

Or directly with gcc:
```bash
gcc -Wall -Wextra -o hello hello.c
```

## Binary Size Checking

This project includes automated size checks to prevent accidental binary bloat. The optimized binary is built with `-Os -s` flags for minimal size.

### Local Size Check

To build an optimized binary and check its size locally:
```bash
make size
```

This will:
- Build `hello_optimized` with GCC using `-Os -s` optimization flags
- Display the binary size in bytes
- Show a detailed size breakdown (text, data, bss segments)
- Display file information

### CI Size Threshold

The CI pipeline automatically checks that optimized binaries (built with both GCC and Clang) do not exceed a configurable size threshold. The default threshold is **20 KB (20480 bytes)**.

**Rationale for the threshold:**
- This is a minimal "Hello World" program that should stay tiny
- The 20 KB threshold provides reasonable headroom (current size is ~14.5 KB)
- Prevents accidental introduction of unnecessary dependencies or code bloat
- Enforces discipline in keeping the codebase lean and efficient

**Configuring the threshold:**
The size threshold can be adjusted via workflow dispatch inputs in the GitHub Actions UI, or by modifying the `SIZE_THRESHOLD_KB` environment variable in `.github/workflows/ci.yml`.

## Testing

Run all tests:
```bash
make test
```

Run unit tests only:
```bash
make unit-test
```

## Available Make Targets

- `make` or `make all` - Build standard binary
- `make debug` - Build with debug symbols
- `make strict` - Build with strict warnings (CI-like)
- `make clang` - Build with clang compiler
- `make size` - Build optimized binary and report size
- `make unit-test` - Run unit tests
- `make test` - Run validation tests
- `make clean` - Remove build artifacts
- `make help` - Show available targets