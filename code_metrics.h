/* Code Metrics - Header file for code analysis functionality
 * Provides structures and functions for analyzing C source files
 */
#ifndef CODE_METRICS_H
#define CODE_METRICS_H

#include <stdio.h>

/* Structure to hold code metrics */
typedef struct {
    int total_lines;
    int code_lines;
    int comment_lines;
    int blank_lines;
    int total_chars;
    int function_count;
    int include_count;
    int variable_declarations;
    int function_calls;
} CodeMetrics;

/* Initialize metrics structure with zeros */
void init_metrics(CodeMetrics* metrics);

/* Analyze a C source file and populate metrics */
int analyze_file(const char* filename, CodeMetrics* metrics);

/* Display metrics in a spreadsheet-like table format */
void display_metrics_table(const CodeMetrics* metrics, const char* filename);

#endif /* CODE_METRICS_H */
