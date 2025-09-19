# Contributing to Hello World

Thank you for your interest in contributing to this project! This document provides guidelines for making changes to the Hello World C program.

## Development Workflow

### Prerequisites

- GCC compiler (version 4.0 or later)
- Clang compiler (optional, for alternative testing)
- Git for version control

### Setting Up Your Development Environment

1. **Clone the repository:**
   ```bash
   git clone https://github.com/lucabol/Hello-World.git
   cd Hello-World
   ```

2. **Verify your compiler setup:**
   ```bash
   gcc --version
   clang --version  # optional
   ```

### Build Commands

#### Standard Development Build
```bash
gcc -Wall -Wextra -o hello hello.c
```
This is the recommended build command for development. It includes essential warnings to catch common issues.

#### Alternative Build Commands

**Basic build (minimal):**
```bash
gcc -o hello hello.c
```

**Debug build:**
```bash
gcc -g -Wall -Wextra -o hello_debug hello.c
```

**Optimized build:**
```bash
gcc -O2 -Wall -Wextra -o hello hello.c
```

**Strict compilation (all warnings):**
```bash
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello hello.c
```

**Using Clang (alternative compiler):**
```bash
clang -o hello_clang hello.c
```

### Testing Your Changes

#### Manual Testing Steps

**ALWAYS run these validation steps after making any changes:**

1. **Compile with warnings:**
   ```bash
   gcc -Wall -Wextra -o hello hello.c
   ```

2. **Run the program:**
   ```bash
   ./hello
   ```

3. **Verify the exact output:**
   The program should output: `Hello world!` (without trailing newline)

4. **Check exit code:**
   ```bash
   echo $?
   ```
   Should return: `0`

5. **Test with strict warnings:**
   ```bash
   gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello hello.c
   ```
   Should compile without any warnings.

6. **Test with alternative compiler:**
   ```bash
   clang -o hello_clang hello.c && ./hello_clang
   ```
   Should produce identical output.

#### Build Timing Expectations

- **GCC compilation:** Under 1 second
- **Clang compilation:** ~4 seconds
- **Program execution:** Instantaneous

### Code Standards

#### Code Style
- Follow existing code formatting in `hello.c`
- Use standard C library functions when possible
- Keep code simple and readable
- Add comments only when necessary to explain complex logic

#### Compilation Requirements
- Code must compile without warnings using `gcc -Wall -Wextra`
- Code must compile successfully with both GCC and Clang
- All builds should complete in under 5 seconds

#### Functional Requirements
- Program must print exactly: `Hello world!`
- Program must exit with code 0
- Program must not require any command-line arguments
- Program must not require any external dependencies beyond standard C library

### Making Changes

#### Before You Start
1. Create a new branch for your changes:
   ```bash
   git checkout -b your-branch-name
   ```

2. Build and test the current code to ensure your environment is working:
   ```bash
   gcc -Wall -Wextra -o hello hello.c && ./hello
   ```

#### Development Process
1. Make your changes to the source code
2. Test your changes using all validation steps above
3. Ensure no new compiler warnings are introduced
4. Verify the program output remains exactly as expected

#### Cleaning Build Artifacts
```bash
rm -f hello hello_debug hello_clang *.exe *.out *.o
```

### Commit Guidelines

#### Commit Message Format
Use clear, descriptive commit messages following this format:

```
<type>: <description>

[optional body]

[optional footer]
```

**Types:**
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code formatting changes
- `refactor`: Code restructuring without functional changes
- `test`: Adding or updating tests
- `chore`: Maintenance tasks

**Examples:**
```
fix: correct typo in hello message

docs: update README with exact output format

feat: add error handling for file operations
```

#### Commit Best Practices
- Make small, focused commits
- Test each commit thoroughly
- Write clear commit messages
- Avoid committing compiled binaries (they're in .gitignore)

### Pull Request Process

1. **Before submitting:**
   - Run all validation steps
   - Update documentation if necessary
   - Test with both GCC and Clang
   - Ensure .gitignore prevents committing binaries

2. **Pull request description should include:**
   - Clear description of changes
   - Reason for the changes
   - Testing steps you've performed
   - Any potential impacts

3. **Review process:**
   - All changes will be reviewed for code quality
   - Automated checks will verify compilation
   - Manual testing will be performed

### Licensing

By contributing to this project, you agree that your contributions will be licensed under the same license as the project. Please ensure you have the right to contribute any code you submit.

### Getting Help

If you have questions about contributing:
- Open an issue for discussion
- Review existing issues and pull requests
- Check this contributing guide for common workflows

### Common Issues

**"Command not found" for gcc:**
- Ensure GCC is installed: `which gcc`
- On Ubuntu/Debian: `sudo apt-get install gcc`
- On macOS: Install Xcode command line tools

**Permission denied when running ./hello:**
- Make sure the file is executable: `chmod +x hello`
- Or run with: `./hello`

**Compilation errors:**
- Check that you're using the correct syntax
- Verify your changes don't introduce syntax errors
- Test with minimal changes first

### Quick Reference Commands

```bash
# Build and test (one-liner)
gcc -Wall -Wextra -o hello hello.c && ./hello

# Clean and rebuild
rm -f hello && gcc -o hello hello.c

# Full validation
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello hello.c && ./hello && echo "Exit code: $?"
```

Thank you for contributing to Hello World!