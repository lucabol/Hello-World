# Hello World

Test repo for JediMaster

## Overview

This repository contains a minimal C program that prints a greeting message. It serves as a basic example for C compilation and execution.

## Quick Start

### Building the Program

```bash
gcc -o hello hello.c
```

### Running the Program

```bash
./hello
```

### Expected Output

When you run the program, it will produce the following exact output:

```
Hello world!
```

**Note:** The program outputs "Hello world!" without a trailing newline and exits with code 0.

## CI Verification

Our Continuous Integration (CI) system performs strict validation to ensure code quality and output consistency:

### Static Analysis
- **Syntax check:** `gcc -fsyntax-only -Wall -Wextra hello.c` validates code syntax before compilation
- **Compilation warnings:** All builds use `-Wall -Wextra` flags to catch potential issues

### Output Format Validation
- **Exact output verification:** CI captures program output and compares it byte-for-byte with the expected format
- **Cross-compiler consistency:** Validates that GCC, Clang, and strict builds all produce identical output
- **Exit code verification:** Ensures the program exits with code 0

The CI fails if:
- Static analysis detects syntax errors
- Program output differs from "Hello world!" (exact match required)
- Program exits with non-zero code
- Any compiler warnings are generated with strict flags

**Why precise formatting matters:** The exact output format is part of the program specification. Any deviation could indicate code changes that break compatibility or introduce unintended behavior changes.