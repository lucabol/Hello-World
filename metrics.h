#ifndef METRICS_H
#define METRICS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Initial capacity for function names array */
#define INITIAL_FUNCTION_CAPACITY 8
/* Maximum function name length */
#define MAX_FUNCTION_NAME_LENGTH 256

/* Structure to hold code metrics */
typedef struct {
    size_t total_lines;
    size_t code_lines;
    size_t comment_lines;
    size_t blank_lines;
    size_t function_count;
    size_t function_capacity;
    size_t include_count;
    char **function_names;
} CodeMetrics;

/* Function declarations */
CodeMetrics* analyze_file(const char* filename);
void display_metrics_spreadsheet(CodeMetrics* metrics, const char* filename);
void free_metrics(CodeMetrics* metrics);
void interactive_menu(CodeMetrics* metrics, const char* filename);

#endif /* METRICS_H */
