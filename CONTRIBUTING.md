# Contributing to Hello World

Thank you for your interest in contributing to this project! This document provides guidelines and instructions for contributing.

## Getting Started

### Prerequisites

- GCC or Clang compiler
- Bash shell
- Git

### Cloning the Repository

```bash
git clone https://github.com/lucabol/Hello-World.git
cd Hello-World
```

## Development Workflow

### Building the Project

Use GCC to compile the program:

```bash
gcc -Wall -Wextra -o hello hello.c
```

For development with stricter warnings:

```bash
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -o hello hello.c
```

### Running Tests Locally

Before submitting a pull request, ensure all tests pass:

#### 1. Validation Tests

Run the validation script that CI uses:

```bash
./test/validate.sh
```

Or for CI-style quiet output:

```bash
./test/validate.sh --quiet
```

#### 2. Unit Tests

Run unit tests using the test runner:

```bash
./test/run_unit_tests.sh
```

Or compile and run manually:

```bash
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -I. -c -o hello_lib.o hello.c -DUNIT_TEST
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -Werror -std=c99 -I. -o test_hello_runner test/test_hello.c hello_lib.o
./test_hello_runner
rm -f hello_lib.o test_hello_runner
```

### Adding New Tests

#### Adding Validation Tests

The validation script (`test/validate.sh`) tests the complete program behavior:

1. Modify `test/validate.sh` to add new validation checks
2. Follow the existing pattern using the helper functions (`print_success`, `print_error`, `print_info`)
3. Ensure your tests work in both normal and `--quiet` modes
4. Test your changes locally before submitting

#### Adding Unit Tests

Unit tests use a lightweight testing framework defined in `test/simple_test.h`:

1. Add new test functions to `test/test_hello.c`
2. Follow the naming convention: `test_<feature>_<behavior>`
3. Use the provided assertion macros:
   - `TEST_ASSERT_EQUAL_STRING(expected, actual)` - Compare strings
   - `TEST_ASSERT_NOT_NULL(ptr)` - Verify pointer is not NULL
   - `TEST_ASSERT_EQUAL_INT(expected, actual)` - Compare integers
4. Register your test in the `main()` function using `RUN_TEST(test_function_name)`
5. Run tests locally to verify they pass

Example test:

```c
void test_my_feature(void) {
    const char* result = my_function();
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("expected value", result);
}
```

## Coding Style

### General Guidelines

- Use consistent indentation (spaces or tabs, but be consistent with the file)
- Keep lines under 80-100 characters when practical
- Use meaningful variable names
- Add comments for complex logic

### C-Specific Conventions

- **Return statements**: Always use explicit `return 0;` from `main()` for success
- **Headers**: Include standard library headers in alphabetical order
- **Function definitions**: Opening brace on same line as function signature
- **Error handling**: Check return values and handle errors appropriately

### Example Code Style

```c
#include <stdio.h>

int main(void) {
    printf("Hello world!\n");
    return 0;
}
```

## Test Output

### Controlling Colorized Output

Colors in test output can be controlled:

#### Validation Script

- **Normal mode** (with colors): `./test/validate.sh`
- **Quiet mode** (no colors): `./test/validate.sh --quiet`

#### Unit Tests

Colors are automatically disabled when:
- Output is redirected to a file or pipe
- Environment variable `SIMPLE_TEST_NO_COLOR` is set to `1`

To explicitly disable colors:

```bash
SIMPLE_TEST_NO_COLOR=1 ./test_hello_runner
```

## Pull Request Process

1. **Fork** the repository and create your branch from `main`
2. **Make your changes** following the coding style guidelines
3. **Add tests** if you're adding functionality
4. **Run all tests** locally and ensure they pass:
   ```bash
   ./test/validate.sh
   ./test/run_unit_tests.sh
   ```
5. **Test with both GCC and Clang** if possible
6. **Update documentation** if you're changing behavior
7. **Commit your changes** with a clear and descriptive commit message
8. **Push** to your fork and submit a pull request
9. **Respond to feedback** during code review

### Commit Message Guidelines

- Use present tense ("Add feature" not "Added feature")
- Use imperative mood ("Move cursor to..." not "Moves cursor to...")
- Keep the first line under 50 characters
- Add detailed description after a blank line if needed

Good examples:
```
Add validation for empty input

- Check for NULL pointers before processing
- Add test case for empty string input
- Update documentation with new behavior
```

## Reporting Issues

When reporting issues, please include:

1. **Description**: Clear description of the issue
2. **Steps to reproduce**: Exact commands or steps to reproduce the problem
3. **Expected behavior**: What you expected to happen
4. **Actual behavior**: What actually happened
5. **Environment**: OS, compiler version (output of `gcc --version`)
6. **Error messages**: Complete error messages or stack traces

## Code Review Process

All submissions require review before being merged:

1. At least one maintainer must approve the PR
2. All CI checks must pass
3. Code must follow the style guidelines
4. Tests must be included for new functionality
5. Documentation must be updated if behavior changes

## Questions?

If you have questions not covered here, feel free to:
- Open an issue for discussion
- Review existing issues and pull requests
- Check the README.md for additional documentation

## Code of Conduct

### Our Standards

- Be respectful and inclusive
- Welcome newcomers and help them learn
- Focus on what is best for the community
- Show empathy towards other community members

### Unacceptable Behavior

- Harassment or discriminatory language
- Personal attacks or trolling
- Publishing others' private information
- Other conduct which could reasonably be considered inappropriate

Thank you for contributing!
