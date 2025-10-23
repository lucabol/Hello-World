/*
 * Code Metrics Tool - Header File
 * Part of the lucabol/Hello-World repository
 * 
 * Provides data structures and API for analyzing C source files
 * and collecting code metrics.
 */

#ifndef CODE_METRICS_H
#define CODE_METRICS_H

#include <stdio.h>
#include <stddef.h>

/**
 * Code metrics structure
 * 
 * Holds various metrics collected from analyzing a C source file.
 * All counters are non-negative integers.
 */
typedef struct {
    char filename[256];       /* Source filename (truncated if > 255 chars) */
    int total_lines;          /* Total lines including blank and comments */
    int code_lines;           /* Lines containing executable code */
    int blank_lines;          /* Lines containing only whitespace */
    int comment_lines;        /* Lines that are primarily comments */
    int functions;            /* Detected function definitions */
    int includes;             /* #include directive count */
    int max_line_length;      /* Maximum display length (tabs expanded to 8 spaces) */
} CodeMetrics;

/**
 * Analyze a C source file and collect metrics
 * 
 * @param filename Path to the C source file to analyze
 * @param metrics  Pointer to CodeMetrics structure to populate
 * @return 0 on success, -1 on error (file not found, invalid parameters)
 * 
 * Thread safety: Not thread-safe. Each thread should use separate CodeMetrics.
 * Memory ownership: Caller owns the CodeMetrics structure.
 * 
 * Encoding: Assumes ASCII/UTF-8 text. Handles LF and CRLF line endings.
 * Line length: Uses getline() for unlimited line length support.
 * Tab handling: Tabs expand to 8 spaces for max_line_length calculation.
 */
int analyze_file(const char* filename, CodeMetrics* metrics);

/**
 * Display metrics in a spreadsheet-like table format
 * 
 * @param metrics Array of CodeMetrics structures
 * @param count   Number of elements in the array
 */
void display_metrics_table(CodeMetrics* metrics, int count);

/**
 * Display interactive menu
 */
void display_menu(void);

#endif /* CODE_METRICS_H */
