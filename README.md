# Hello World
 Test repo for JediMaster

## Getting Started

This is a simple C "Hello World" program. To build and run:

```bash
gcc -o hello hello.c
./hello
```

Or use the Makefile:

```bash
make
./hello
```

## Troubleshooting

This section addresses common issues you might encounter when building and running the Hello World program.

### Command not found for gcc

**Problem:** When running `gcc -o hello hello.c`, you get an error like:
```
bash: gcc: command not found
```

**Solutions:**

1. **Check if GCC is installed:**
   ```bash
   which gcc
   gcc --version
   ```

2. **Install GCC on Ubuntu/Debian:**
   ```bash
   sudo apt update
   sudo apt install gcc
   ```

3. **Install GCC on CentOS/RHEL/Fedora:**
   ```bash
   # For CentOS/RHEL
   sudo yum install gcc
   # For Fedora
   sudo dnf install gcc
   ```

4. **Install GCC on macOS:**
   ```bash
   # Install Xcode Command Line Tools
   xcode-select --install
   # Or install via Homebrew
   brew install gcc
   ```

5. **Alternative: Use Clang (if available):**
   ```bash
   clang -o hello hello.c
   ```

### Permission denied for ./hello

**Problem:** When running `./hello`, you get an error like:
```
bash: ./hello: Permission denied
```

**Solutions:**

1. **Make the file executable:**
   ```bash
   chmod +x hello
   ./hello
   ```

2. **Alternative: Run with explicit interpreter:**
   ```bash
   # This shouldn't be needed for compiled binaries, but if chmod doesn't work:
   /path/to/hello
   ```

3. **Check file permissions:**
   ```bash
   ls -l hello
   # Should show executable permissions like: -rwxr-xr-x
   ```

4. **Rebuild and ensure proper compilation:**
   ```bash
   gcc -o hello hello.c
   chmod +x hello
   ./hello
   ```

### No such file or directory errors

**Problem:** You get errors like:
```
bash: ./hello: No such file or directory
```
or
```
gcc: hello.c: No such file or directory
```

**Solutions:**

1. **Ensure you're in the correct directory:**
   ```bash
   pwd
   ls -la
   # Should show hello.c in the current directory
   ```

2. **Verify source file exists:**
   ```bash
   ls -la hello.c
   # Should show the source file
   ```

3. **Check if binary was created after compilation:**
   ```bash
   gcc -o hello hello.c
   ls -la hello
   # Should show the compiled binary
   ```

4. **Use absolute paths if relative paths fail:**
   ```bash
   gcc -o /full/path/to/hello /full/path/to/hello.c
   /full/path/to/hello
   ```

### Compilation errors

**Problem:** GCC reports compilation errors or warnings.

**Solutions:**

1. **Basic compilation with warnings:**
   ```bash
   gcc -Wall -Wextra -o hello hello.c
   ```

2. **Check for syntax errors in hello.c:**
   ```bash
   gcc -Wall -Wextra -Wpedantic -o hello hello.c
   ```

3. **Clean and rebuild:**
   ```bash
   make clean
   make
   ```

4. **Debug build for troubleshooting:**
   ```bash
   gcc -g -Wall -Wextra -o hello_debug hello.c
   ```

### Build verification

**To verify everything is working correctly:**

```bash
# Clean any existing binaries
make clean

# Compile with warnings
gcc -Wall -Wextra -o hello hello.c

# Run the program
./hello

# Check exit code (should be 0)
echo $?

# Expected output: "Hello world!"
```

### Alternative build methods

If the standard `gcc` command doesn't work, try these alternatives:

1. **Using Make:**
   ```bash
   make          # Standard build
   make dev      # Development build with warnings
   make test     # Build and run
   make help     # Show all available targets
   ```

2. **Using Clang:**
   ```bash
   clang -o hello hello.c
   ./hello
   ```

3. **Strict compilation (maximum warnings):**
   ```bash
   make strict
   ./hello_strict
   ```

If you continue to experience issues, ensure you have a C compiler installed and that you're running the commands from the directory containing `hello.c`.