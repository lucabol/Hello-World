# Hello World

A simple C "Hello World" program perfect for beginners learning C programming.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Installation](#installation)
  - [Windows](#windows)
  - [macOS](#macos)
  - [Linux](#linux)
- [Quick Start](#quick-start)
- [Compilation Options](#compilation-options)
- [Running the Program](#running-the-program)
- [Troubleshooting](#troubleshooting)
- [Development Tips](#development-tips)

## Prerequisites

To compile and run this program, you'll need:
- A C compiler (GCC or Clang recommended)
- A terminal/command prompt
- Basic familiarity with command-line operations

## Installation

### Windows

#### Option 1: Using MinGW-w64
1. Download MinGW-w64 from [https://www.mingw-w64.org/downloads/](https://www.mingw-w64.org/downloads/)
2. Install it following the installer instructions
3. Add the `bin` directory to your PATH environment variable
4. Open Command Prompt and verify: `gcc --version`

#### Option 2: Using Visual Studio
1. Download Visual Studio Community from [https://visualstudio.microsoft.com/](https://visualstudio.microsoft.com/)
2. During installation, select "Desktop development with C++"
3. Use the "Developer Command Prompt" to access the compiler

#### Option 3: Using WSL (Windows Subsystem for Linux)
1. Install WSL following Microsoft's guide
2. Install Ubuntu from Microsoft Store
3. Follow the Linux instructions below in your WSL terminal

### macOS

#### Option 1: Using Xcode Command Line Tools (Recommended)
```bash
xcode-select --install
```

#### Option 2: Using Homebrew
```bash
# First install Homebrew if you haven't already
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Then install GCC
brew install gcc
```

### Linux

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential
```

#### Red Hat/CentOS/Fedora
```bash
# For newer versions (dnf)
sudo dnf groupinstall "Development Tools"

# For older versions (yum)
sudo yum groupinstall "Development Tools"
```

#### Arch Linux
```bash
sudo pacman -S base-devel
```

## Quick Start

1. **Clone or download this repository**
   ```bash
   git clone https://github.com/lucabol/Hello-World.git
   cd Hello-World
   ```

2. **Compile the program**
   ```bash
   gcc -o hello hello.c
   ```

3. **Run the program**
   ```bash
   ./hello        # On Linux/macOS
   hello.exe      # On Windows
   ```

4. **Expected output**
   ```
   Hello world!
   ```

## Compilation Options

### Basic Compilation
```bash
gcc -o hello hello.c
```

### Compilation with Warnings (Recommended for Development)
```bash
gcc -Wall -Wextra -o hello hello.c
```

### Optimized Compilation (for Production)
```bash
gcc -O2 -Wall -Wextra -o hello hello.c
```

### Debug Compilation
```bash
gcc -g -Wall -Wextra -o hello_debug hello.c
```

### Strict Compilation (Maximum Warnings)
```bash
gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello hello.c
```

### Using Clang (Alternative Compiler)
```bash
clang -o hello hello.c
clang -Wall -Wextra -o hello hello.c  # With warnings
```

## Running the Program

### Linux/macOS
```bash
./hello
```

### Windows
```bash
hello.exe
# or simply
hello
```

### Checking Exit Status
After running the program, you can check if it completed successfully:

**Linux/macOS:**
```bash
./hello
echo $?    # Should output: 0
```

**Windows:**
```cmd
hello.exe
echo %ERRORLEVEL%    # Should output: 0
```

## Troubleshooting

### Common Issues and Solutions

#### "gcc: command not found" or "gcc is not recognized"
**Problem:** GCC compiler is not installed or not in PATH.

**Solution:**
- **Windows:** Ensure MinGW-w64 is installed and added to PATH
- **macOS:** Install Xcode Command Line Tools: `xcode-select --install`
- **Linux:** Install build tools: `sudo apt install build-essential` (Ubuntu/Debian)

#### "Permission denied" when running ./hello
**Problem:** The compiled program doesn't have execute permissions.

**Solution:**
```bash
chmod +x hello
./hello
```

#### "No such file or directory" when running ./hello
**Problem:** Either compilation failed or you're in the wrong directory.

**Solution:**
1. Check if compilation was successful (no error messages)
2. Verify the binary exists: `ls -la hello`
3. Ensure you're in the correct directory
4. Try absolute path: `/full/path/to/hello`

#### Compilation errors about missing headers
**Problem:** Standard C library headers are not found.

**Solution:**
- Ensure you have a complete C development environment installed
- On Linux: Install `libc6-dev` package
- On Windows: Reinstall MinGW-w64 with all components

#### "undefined reference" errors
**Problem:** Linker cannot find required libraries (rare for this simple program).

**Solution:**
- Try compiling with: `gcc -static -o hello hello.c`
- Ensure you have complete development tools installed

### Verifying Your Installation

Run these commands to verify your setup:

```bash
# Check compiler version
gcc --version

# Check if clang is available (optional)
clang --version

# Compile and run a quick test
echo '#include <stdio.h>
int main() { printf("Setup OK!\n"); return 0; }' > test.c
gcc -o test test.c && ./test
rm test test.c  # Clean up
```

## Development Tips

### Best Practices
1. **Always compile with warnings:** Use `-Wall -Wextra` flags
2. **Test immediately:** Run your program after each compilation
3. **Use version control:** Keep track of your changes with git
4. **Clean builds:** Remove old binaries before rebuilding

### Useful Commands
```bash
# Clean compiled binaries
rm -f hello hello.exe hello_debug

# Compile and run in one line
gcc -Wall -Wextra -o hello hello.c && ./hello

# Time your compilation (for fun)
time gcc -o hello hello.c
```

### Editor Recommendations
- **Visual Studio Code** with C/C++ extension
- **CLion** (JetBrains)
- **Code::Blocks**
- **Dev-C++** (Windows)
- **Vim/Neovim** with appropriate plugins

### Next Steps
Once you've successfully compiled and run this program:
1. Try modifying the output message
2. Add more printf statements
3. Experiment with variables and user input
4. Learn about command-line arguments
5. Explore more complex C programs

---

**Happy coding!** 🚀

If you encounter any issues not covered in this guide, please feel free to open an issue on this repository.