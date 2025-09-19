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

Note: The program outputs "Hello world!" without a trailing newline character, so the shell prompt appears immediately after the text.

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

1. Compile with warnings enabled:
   ```bash
   gcc -Wall -Wextra -o hello hello.c
   ```

2. Run the program:
   ```bash
   ./hello
   ```

3. Check that output is exactly "Hello world!" (no trailing newline)

4. Verify exit code is 0:
   ```bash
   echo $?
   ```

The compilation should complete without warnings, and the program should run successfully with exit code 0.