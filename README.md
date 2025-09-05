# Hello-World
Test repo for JediMaster

## Code Metrics Spreadsheet Interface

This repository includes a code metrics analyzer that displays analysis results in a spreadsheet-like tabular format.

### Usage

1. Compile the metrics analyzer:
   ```bash
   gcc -Wall -Wextra -o metrics metrics.c
   ```

2. Analyze C source files:
   ```bash
   ./metrics hello.c
   ./metrics hello.c metrics.c    # Multiple files
   ```

3. The tool displays metrics in a formatted table including:
   - Total lines, code lines, empty lines, comment lines
   - Include statements, function definitions, printf calls
   - File size and summary statistics

### Building the Hello World Program

```bash
gcc -o hello hello.c
./hello
```
