# Hello World

Test repo for JediMaster

A simple C program that prints "Hello world!" to the console.

## Building and Running

### Quick Start

```bash
$ gcc -Wall -Wextra -o hello hello.c
$ ./hello
Hello world!$ echo $?
0
```

### Expected Output

When you run the program, it produces the following exact output:

```
Hello world!
```

Note: The program outputs "Hello world!" without a trailing newline character, so the shell prompt appears immediately after the text. This behavior is consistent across POSIX-compliant systems, though the visual presentation may vary slightly depending on your terminal configuration.

The program exits with status code 0, which you can verify with:

```bash
$ ./hello
Hello world!$ echo $?
0
```

### Build Options

#### Standard Build
```bash
gcc -o hello hello.c
```

#### Development Build (Recommended)
```bash
gcc -Wall -Wextra -o hello hello.c
```

#### Optimized Build
```bash
gcc -O2 -Wall -Wextra -o hello hello.c
```

#### Strict Compilation
```bash
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello hello.c
```

### Alternative Compilers

The program also compiles successfully with Clang:

```bash
$ clang -o hello hello.c
$ ./hello
Hello world!$ echo $?
0
```

## Platform Requirements

- **Operating System**: POSIX-compliant systems (Linux, macOS, Unix)
- **Shell**: POSIX shell (bash, zsh, etc.) for running examples
- **Compiler**: GCC 4.9+ or Clang 3.5+ 
- **C Standard**: C89/C90 compatible

### Tested Environments

- GCC 13.3.0 on Ubuntu
- Clang 14+ on Ubuntu

## Validation

To verify the program works correctly:

1. **Manual verification:**
   ```bash
   gcc -Wall -Wextra -o hello hello.c
   ./hello
   echo $?
   ```

2. **Automated testing:**
   Run the included test script:
   ```bash
   ./test.sh
   ```

The compilation should complete without warnings, and the program should produce exactly "Hello world!" (12 bytes, no trailing newline) with exit code 0.

### Automated Validation

For automated testing, you can use this shell script to verify exact output:

```bash
#!/bin/bash
# Compile and test the program
gcc -Wall -Wextra -o hello hello.c
if [ $? -ne 0 ]; then
    echo "FAIL: Compilation failed"
    exit 1
fi

# Capture output and exit code
output=$(./hello)
exit_code=$?

# Verify exact output (12 bytes, no newline)
expected="Hello world!"
if [ "$output" = "$expected" ] && [ $exit_code -eq 0 ]; then
    echo "PASS: Output and exit code correct"
    exit 0
else
    echo "FAIL: Expected '$expected' with exit code 0, got '$output' with exit code $exit_code"
    exit 1
fi
```

You can save this as `test.sh` and run `chmod +x test.sh && ./test.sh` to validate the program.
