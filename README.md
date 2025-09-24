# Hello World
Test repo for JediMaster

## Local Development

### Requirements
- GCC or Clang compiler
- One of: `hexdump` (util-linux), `xxd`, or `od` for validation hex output

### Building and Testing
```bash
# Build with GCC
gcc -o hello hello.c

# Build with Clang  
clang -o hello hello.c

# Run validation (requires hexdump/xxd/od)
bash test/validate.sh --quiet

# Validate specific binary
bash test/validate-binary.sh ./hello --quiet
```

The validation scripts perform byte-level output verification to ensure exact output format including trailing newlines.