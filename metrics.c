#define _POSIX_C_SOURCE 200809L
#include "metrics.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* Helper function to check if a substring is within a string literal */
static int is_in_string_literal(const char* line, const char* pos) {
    int in_string = 0;
    int escape = 0;
    
    for (const char* p = line; p < pos; p++) {
        if (escape) {
            escape = 0;
            continue;
        }
        if (*p == '\\') {
            escape = 1;
            continue;
        }
        if (*p == '"') {
            in_string = !in_string;
        }
    }
    return in_string;
}

/* Helper function to find a pattern not in string literal or comment */
static int contains_pattern_safe(const char* line, const char* pattern, int in_comment) {
    if (in_comment) {
        return 0; /* Already in comment, don't count */
    }
    
    const char* found = strstr(line, pattern);
    if (!found) {
        return 0;
    }
    
    /* Check if it's in a string literal */
    if (is_in_string_literal(line, found)) {
        return 0;
    }
    
    /* Check if it's after a // comment */
    const char* comment_pos = strstr(line, "//");
    if (comment_pos && comment_pos < found) {
        return 0;
    }
    
    return 1;
}

CodeMetrics analyze_file(const char* filename) {
    CodeMetrics metrics = {0, 0, 0, 0, 0, 0, 0, 0};
    FILE* file = fopen(filename, "r");
    
    if (!file) {
        return metrics;
    }
    
    char* line = NULL;
    size_t line_cap = 0;
    ssize_t line_len;
    int in_multiline_comment = 0;
    
    /* Use getline for handling arbitrarily long lines */
    while ((line_len = getline(&line, &line_cap, file)) != -1) {
        metrics.total_lines++;
        
        /* Track max line length (including newline) */
        if (line_len > metrics.max_line_length) {
            metrics.max_line_length = (int)line_len;
        }
        
        /* Check for blank lines */
        int is_blank = 1;
        for (ssize_t i = 0; i < line_len; i++) {
            if (!isspace((unsigned char)line[i])) {
                is_blank = 0;
                break;
            }
        }
        
        if (is_blank) {
            metrics.blank_lines++;
            continue;
        }
        
        /* Check for comments */
        int is_comment = 0;
        char* ptr = line;
        
        /* Skip leading whitespace */
        while (*ptr && isspace((unsigned char)*ptr)) {
            ptr++;
        }
        
        /* Check for multiline comment start/end */
        if (in_multiline_comment) {
            is_comment = 1;
            if (strstr(ptr, "*/")) {
                in_multiline_comment = 0;
            }
        } else if (strstr(ptr, "/*")) {
            is_comment = 1;
            in_multiline_comment = 1;
            if (strstr(ptr, "*/")) {
                in_multiline_comment = 0;
            }
        } else if (ptr[0] == '/' && ptr[1] == '/') {
            is_comment = 1;
        }
        
        if (is_comment) {
            metrics.comment_lines++;
        } else {
            metrics.code_lines++;
        }
        
        /* Count includes (handles both #include and # include) */
        /* Only count if not in comment or string */
        if (!is_comment) {
            if (contains_pattern_safe(line, "#include", in_multiline_comment) || 
                contains_pattern_safe(line, "# include", in_multiline_comment) ||
                contains_pattern_safe(line, "#  include", in_multiline_comment)) {
                metrics.include_count++;
            }
        }
        
        /* Count function definitions (simple heuristic) */
        /* This is a basic heuristic - see documentation for limitations */
        if (!is_comment && (strstr(line, "int main") || 
            (strstr(line, "void ") && strchr(line, '(')) ||
            (strstr(line, "int ") && strchr(line, '(') && strchr(line, '{')))) {
            metrics.function_count++;
        }
        
        /* Count printf calls (not in comments or strings) */
        if (!is_comment && contains_pattern_safe(line, "printf", in_multiline_comment)) {
            metrics.printf_count++;
        }
    }
    
    free(line);
    fclose(file);
    return metrics;
}

void print_metrics(const CodeMetrics* metrics, const char* filename) {
    printf("Code Metrics for: %s\n", filename);
    printf("=====================================\n");
    printf("Total Lines:      %d\n", metrics->total_lines);
    printf("Code Lines:       %d\n", metrics->code_lines);
    printf("Blank Lines:      %d\n", metrics->blank_lines);
    printf("Comment Lines:    %d\n", metrics->comment_lines);
    printf("Function Count:   %d\n", metrics->function_count);
    printf("Include Count:    %d\n", metrics->include_count);
    printf("Printf Count:     %d\n", metrics->printf_count);
    printf("Max Line Length:  %d\n", metrics->max_line_length);
}
