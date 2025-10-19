#include "metrics.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

CodeMetrics analyze_file(const char* filename) {
    CodeMetrics metrics = {0, 0, 0, 0, 0, 0, 0, 0};
    FILE* file = fopen(filename, "r");
    
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return metrics;
    }
    
    char line[1024];
    int in_multiline_comment = 0;
    
    while (fgets(line, sizeof(line), file)) {
        metrics.total_lines++;
        int len = (int)strlen(line);
        
        /* Track max line length */
        if (len > metrics.max_line_length) {
            metrics.max_line_length = len;
        }
        
        /* Check for blank lines */
        int is_blank = 1;
        for (int i = 0; i < len; i++) {
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
        if (strstr(line, "#include") || strstr(line, "# include")) {
            metrics.include_count++;
        }
        
        /* Count function definitions (simple heuristic) */
        if (strstr(line, "int main") || strstr(line, "void ") || 
            (strstr(line, "int ") && strchr(line, '('))) {
            metrics.function_count++;
        }
        
        /* Count printf calls */
        if (strstr(line, "printf")) {
            metrics.printf_count++;
        }
    }
    
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
