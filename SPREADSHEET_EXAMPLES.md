# Code Metrics Spreadsheet Tool - Example Output

This document shows example output from the Code Metrics Spreadsheet Tool when analyzing hello.c.

## Initial Screen

```
╔═══════════════════════════════════════════════════════════════════════════╗
║                 CODE METRICS SPREADSHEET TOOL                             ║
║                      Version 1.0                                          ║
╚═══════════════════════════════════════════════════════════════════════════╝

Analyzing file: hello.c
✓ Analysis complete!
```

## Interactive Menu

```
╔═══════════════════════════════════════════════════════════════════════════╗
║                     CODE METRICS SPREADSHEET MENU                         ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  1. Display Full Metrics Table                                           ║
║  2. Display Line Counts Only                                             ║
║  3. Display Code Structure Only                                          ║
║  4. Display Summary Statistics                                           ║
║  5. Export to CSV Format                                                 ║
║  6. Re-analyze File                                                      ║
║  0. Exit                                                                 ║
╚═══════════════════════════════════════════════════════════════════════════╝

Select an option (0-6):
```

## Option 1: Full Metrics Table

```
╔═══════════════════════════════════════════════════════════════════════════╗
║                      CODE METRICS SPREADSHEET                             ║
╠═══════════════════════════════════════════════════════════════════════════╣
║ File: hello.c                                                             ║
╠═══════════════════════════════════════════════════════════════════════════╣
║                                                                           ║
║  METRIC                                              VALUE                ║
║                                                                           ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  Lines of Code (Total)                               5                   ║
║  Lines of Code (Code only)                           4                   ║
║  Lines of Code (Comments)                            0                   ║
║  Lines of Code (Blank)                               1                   ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  Total Characters                                    63                  ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  Function Definitions                                1                   ║
║  Function Calls                                      2                   ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  Include Directives                                  0                   ║
║  Variable Declarations                               0                   ║
╠═══════════════════════════════════════════════════════════════════════════╣
║                          SUMMARY STATISTICS                               ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  Code Coverage                                        80.0%              ║
║  Comment Coverage                                      0.0%              ║
║  Blank Line Coverage                                  20.0%              ║
╚═══════════════════════════════════════════════════════════════════════════╝
```

## Option 2: Line Counts Only

```
╔═══════════════════════════════════════════════════════════════════════════╗
║                        LINE COUNT METRICS                                 ║
╠═══════════════════════════════════════════════════════════════════════════╣
║ File: hello.c                                                             ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  Total Lines                                         5                   ║
║  Code Lines                                          4                   ║
║  Comment Lines                                       0                   ║
║  Blank Lines                                         1                   ║
╚═══════════════════════════════════════════════════════════════════════════╝
```

## Option 3: Code Structure Only

```
╔═══════════════════════════════════════════════════════════════════════════╗
║                      CODE STRUCTURE METRICS                               ║
╠═══════════════════════════════════════════════════════════════════════════╣
║ File: hello.c                                                             ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  Function Definitions                                1                   ║
║  Function Calls                                      2                   ║
║  Include Directives                                  0                   ║
║  Variable Declarations                               0                   ║
║  Total Characters                                    63                  ║
╚═══════════════════════════════════════════════════════════════════════════╝
```

## Option 4: Summary Statistics

```
╔═══════════════════════════════════════════════════════════════════════════╗
║                         SUMMARY STATISTICS                                ║
╠═══════════════════════════════════════════════════════════════════════════╣
║ File: hello.c                                                             ║
╠═══════════════════════════════════════════════════════════════════════════╣
║  Code Coverage                                        80.0%              ║
║  Comment Coverage                                      0.0%              ║
║  Blank Line Coverage                                  20.0%              ║
║  Average Characters per Line                          12.6               ║
║  Function Density (per 100 LOC)                      25.0               ║
║  Function Call Density (per 100 LOC)                 50.0               ║
╚═══════════════════════════════════════════════════════════════════════════╝
```

## Option 5: CSV Export

```
✓ Metrics exported to 'hello.c_metrics.csv'
```

**CSV file contents (hello.c_metrics.csv):**

```csv
Metric,Value
File,hello.c
Total Lines,5
Code Lines,4
Comment Lines,0
Blank Lines,1
Total Characters,63
Function Definitions,1
Function Calls,2
Include Directives,0
Variable Declarations,0
Code Coverage (%),80.0
Comment Coverage (%),0.0
Blank Coverage (%),20.0
```

This CSV file can be opened in any spreadsheet application like Microsoft Excel, Google Sheets, or LibreOffice Calc for further analysis and visualization.

## Exit Message

```
Exiting Code Metrics Spreadsheet Tool. Goodbye!
```
