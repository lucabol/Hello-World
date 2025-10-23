#ifndef CODE_METRICS_H
#define CODE_METRICS_H

#include <stdio.h>

/* Code metrics structure */
typedef struct {
    char filename[256];
    int total_lines;
    int code_lines;
    int blank_lines;
    int comment_lines;
    int functions;
    int includes;
    int max_line_length;
} CodeMetrics;

/* Function to analyze a C source file and collect metrics */
int analyze_file(const char* filename, CodeMetrics* metrics);

/* Function to display metrics in a spreadsheet-like format */
void display_metrics_table(CodeMetrics* metrics, int count);

/* Function to display interactive menu */
void display_menu(void);

#endif /* CODE_METRICS_H */
