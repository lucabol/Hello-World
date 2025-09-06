# Hello World
Test repo for JediMaster

## Code Metrics Analyzer

This repository now includes a spreadsheet-like code metrics analyzer that can analyze C source files and display various metrics in a tabular format.

### Features

- **Lines Analysis**: Total lines, code lines, blank lines, comment lines
- **Structure Analysis**: Function count, include statements, brace count
- **Content Analysis**: File size, character count, keyword occurrences
- **Coverage Metrics**: Code coverage percentage, comment coverage percentage
- **Interactive Table**: Clean, formatted output with box-drawing characters

### Usage

```bash
# Build both programs
./build.sh

# Run the Hello World program
./hello

# Analyze hello.c metrics (default)
./metrics

# Analyze any C file
./metrics filename.c
```

### Example Output

```
╔══════════════════════════════════════════════════════════════╗
║                     CODE METRICS ANALYZER                   ║
╠══════════════════════════════════════════════════════════════╣
║ File: hello.c                                             ║
╠═══════════════════════════════════╤══════════════════════════╣
║ Metric                            │ Value                    ║
╠═══════════════════════════════════╪══════════════════════════╣
║ Total Lines                       │ 5                        ║
║ Code Lines                        │ 4                        ║
║ Blank Lines                       │ 1                        ║
║ Comment Lines                     │ 0                        ║
║ Include Statements               │ 1                        ║
║ Functions                         │ 1                        ║
║ File Size (bytes)                 │ 63                       ║
║ Character Count                   │ 63                       ║
║ Keyword Occurrences              │ 4                        ║
║ Brace Count                       │ 2                        ║
╠═══════════════════════════════════╪══════════════════════════╣
║ Code Coverage                     │ 80.0                  % ║
║ Comment Coverage                  │ 0.0                   % ║
╚═══════════════════════════════════╧══════════════════════════╝
```