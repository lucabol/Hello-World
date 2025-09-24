# Hello World
Test repo for JediMaster

A simple C "Hello World" program that demonstrates basic C programming concepts.

## Quick Start

### Build and Run
```bash
# Basic build
gcc -o hello hello.c

# Run the program  
./hello
```

### Code Metrics Analysis
Analyze hello.c with an interactive spreadsheet interface:
```bash
# Generate metrics spreadsheet
./analyze_metrics.sh

# Open code_metrics_spreadsheet.html in your browser
```

Features:
- 📊 **18 different code metrics** (lines, functions, complexity, etc.)
- 🖥️ **Interactive web interface** with sorting and filtering
- 📋 **CSV export** for external analysis  
- 📱 **Responsive design** for mobile and desktop

See [METRICS_README.md](METRICS_README.md) for detailed documentation.

## Expected Output

When you run the program, it will produce the following exact output:

```
Hello world!
```

**Important:** The program intentionally outputs "Hello world!" **without a trailing newline** and exits with code 0.