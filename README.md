# Hello World
Test repo for JediMaster

## Testing

This repository includes automated testing with both C unit tests and shell validation:

### C Unit Tests
- `test/test_hello.c` - Automated unit tests written in C
- Tests program output, exit code, and binary existence
- Run with: `make test-c`

### Shell Validation  
- `test/validate.sh` - Shell script validation (legacy)
- Validates strict compilation and output format
- Run with: `make test-shell`

### Running Tests
- `make test` - Run both C unit tests and shell validation
- `make test-quiet` - Run tests with minimal output (CI-friendly)

### Build Targets
- `make` or `make hello` - Build the program
- `make strict` - Build with strict warnings (CI-like)
- `make test-c` - Build and run C unit tests
- `make clean` - Remove compiled artifacts