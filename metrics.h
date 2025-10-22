#ifndef METRICS_H
#define METRICS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Structure to hold code metrics */
typedef struct {
    int total_lines;
    int code_lines;
    int comment_lines;
    int blank_lines;
    int function_count;
    int include_count;
    char **function_names;
} CodeMetrics;

/* Function declarations */
CodeMetrics* analyze_file(const char* filename);
void display_metrics_spreadsheet(CodeMetrics* metrics, const char* filename);
void free_metrics(CodeMetrics* metrics);
void interactive_menu(CodeMetrics* metrics, const char* filename);

#endif /* METRICS_H */
