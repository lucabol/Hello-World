# Contributing to Hello World

Thank you for your interest in contributing to this simple C project!

## Development Setup

### Prerequisites

- GCC compiler (recommended) or Clang
- Basic knowledge of C programming

### Building the Project

1. **Standard build:**
   ```bash
   gcc -o hello hello.c
   ```

2. **Development build (with warnings):**
   ```bash
   gcc -Wall -Wextra -o hello hello.c
   ```

3. **Strict build (recommended for contributions):**
   ```bash
   gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello hello.c
   ```

### Testing

After making any changes, always run these validation steps:

1. **Compile with warnings:**
   ```bash
   gcc -Wall -Wextra -o hello hello.c
   ```

2. **Run the program:**
   ```bash
   ./hello
   ```

3. **Verify expected output:**
   ```
   Hello world!
   
   Exit code: 0
   ```

4. **Check exit code:**
   ```bash
   echo $?  # Should return 0
   ```

### Additional Validation

- **Test with alternative compiler:**
  ```bash
  clang -o hello_clang hello.c && ./hello_clang
  ```

- **Test debug build:**
  ```bash
  gcc -g -o hello_debug hello.c && ./hello_debug
  ```

## Coding Standards

### Compilation Requirements

- **All contributions must compile without warnings** using:
  ```bash
  gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello hello.c
  ```

- **Code must work with both GCC and Clang**

### Style Guidelines

- Follow the existing minimal style in `hello.c`
- Use standard C library functions only
- Maintain the exact output format for compatibility

### Output Format

The program must output exactly:
```
Hello world!

Exit code: 0
```

This format includes:
- "Hello world!" message
- One blank line
- "Exit code: 0" message
- Final newline

## Submitting Changes

1. Fork the repository
2. Create a feature branch
3. Make your changes following the coding standards
4. Test your changes using all validation steps above
5. Submit a pull request with a clear description

## Quick Development Workflow

```bash
# Make changes to hello.c
# Then validate:
gcc -Wall -Wextra -o hello hello.c && ./hello
```

The build should complete in under 1 second and produce the expected output.

## Project Goals

This is a minimal "Hello World" program intended to:
- Demonstrate basic C program structure
- Provide a simple example for C beginners
- Serve as a template for basic C projects

Keep contributions simple and focused on these goals.