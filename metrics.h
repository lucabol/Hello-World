#ifndef METRICS_H
#define METRICS_H

/* Code metrics structure */
typedef struct {
    int total_lines;
    int code_lines;
    int blank_lines;
    int comment_lines;
    int function_count;
    int include_count;
    int printf_count;
    int max_line_length;
} CodeMetrics;

/* Function to analyze a C source file and return metrics */
CodeMetrics analyze_file(const char* filename);

/* Function to print metrics in a basic format */
void print_metrics(const CodeMetrics* metrics, const char* filename);

#endif /* METRICS_H */
