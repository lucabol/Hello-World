# Hello World
Test repo for JediMaster

## Local Development

### Requirements
- GCC or Clang compiler
- For strict validation: One of `hexdump` (util-linux), `xxd`, or `od` for hex output
  - `hexdump` is preferred and available by default on ubuntu-latest CI runners
  - Falls back to `cat -v` if none available (less accurate, may cause validation failures)

### Building and Testing
```bash
# Build with GCC
gcc -o hello hello.c

# Build with Clang  
clang -o hello hello.c

# Run validation (requires hexdump/xxd/od for detailed error reporting)
bash test/validate.sh --quiet

# Validate specific binary
bash test/validate-binary.sh ./hello --quiet

# Run validator self-tests
bash test/test-validator.sh
```

The validation scripts perform byte-level output verification to ensure exact output format including trailing newlines.
