/* Code Metrics - Implementation of code analysis functionality
 * Analyzes C source files and provides various code metrics
 */
#include "code_metrics.h"
#include <string.h>
#include <ctype.h>

/* Initialize metrics structure with zeros */
void init_metrics(CodeMetrics* metrics) {
    metrics->total_lines = 0;
    metrics->code_lines = 0;
    metrics->comment_lines = 0;
    metrics->blank_lines = 0;
    metrics->total_chars = 0;
    metrics->function_count = 0;
    metrics->include_count = 0;
    metrics->variable_declarations = 0;
    metrics->function_calls = 0;
}

/* Check if a line is blank (contains only whitespace) */
static int is_blank_line(const char* line) {
    while (*line != '\0' && *line != '\n') {
        if (!isspace((unsigned char)*line)) {
            return 0;
        }
        line++;
    }
    return 1;
}

/* Check if a line contains a comment */
static int is_comment_line(const char* line) {
    const char* p = line;
    /* Skip leading whitespace */
    while (*p != '\0' && isspace((unsigned char)*p)) {
        p++;
    }
    /* Check for single-line comment */
    if (p[0] == '/' && p[1] == '/') {
        return 1;
    }
    /* Check for multi-line comment start */
    if (p[0] == '/' && p[1] == '*') {
        return 1;
    }
    /* Check for multi-line comment continuation */
    if (p[0] == '*') {
        return 1;
    }
    return 0;
}

/* Check if a line contains an include statement */
static int is_include_line(const char* line) {
    const char* p = line;
    /* Skip leading whitespace */
    while (*p != '\0' && isspace((unsigned char)*p)) {
        p++;
    }
    /* Check for #include */
    if (p[0] == '#' && strncmp(p + 1, "include", 7) == 0) {
        return 1;
    }
    return 0;
}

/* Simple heuristic to detect function definitions */
static int contains_function_definition(const char* line) {
    /* Look for pattern like "type name(" or "type name (" */
    /* This is a simple heuristic - not perfect but works for basic cases */
    const char* p = line;
    int has_paren = 0;
    int has_open_brace = 0;
    
    /* Skip leading whitespace */
    while (*p != '\0' && isspace((unsigned char)*p)) {
        p++;
    }
    
    /* Skip lines that start with # (preprocessor directives) */
    if (*p == '#') {
        return 0;
    }
    
    /* Look for opening parenthesis and check context */
    while (*p != '\0') {
        if (*p == '(') {
            has_paren = 1;
        }
        if (*p == '{') {
            has_open_brace = 1;
        }
        p++;
    }
    
    /* Simple heuristic: line with both ( and { might be a function definition */
    /* but only if it doesn't start with common control structures */
    if (has_paren && (has_open_brace || strstr(line, "){"))) {
        /* Check it's not a control structure */
        if (strstr(line, "if(") || strstr(line, "if (") ||
            strstr(line, "while(") || strstr(line, "while (") ||
            strstr(line, "for(") || strstr(line, "for (") ||
            strstr(line, "switch(") || strstr(line, "switch (")) {
            return 0;
        }
        return 1;
    }
    
    return 0;
}

/* Count function calls in a line (simple heuristic) */
static int count_function_calls(const char* line) {
    int count = 0;
    const char* p = line;
    
    /* Skip comments */
    if (is_comment_line(line)) {
        return 0;
    }
    
    /* Look for common function calls like printf, scanf, etc. */
    while (*p != '\0') {
        /* Look for identifier followed by '(' */
        if (isalpha((unsigned char)*p) || *p == '_') {
            const char* start = p;
            /* Read identifier */
            while (isalnum((unsigned char)*p) || *p == '_') {
                p++;
            }
            /* Skip whitespace */
            while (*p != '\0' && isspace((unsigned char)*p)) {
                p++;
            }
            /* Check if followed by '(' */
            if (*p == '(') {
                /* Check it's not a function definition */
                int len = (int)(p - start);
                if (len > 0) {
                    /* Skip control keywords */
                    if (!(len == 2 && strncmp(start, "if", 2) == 0) &&
                        !(len == 5 && strncmp(start, "while", 5) == 0) &&
                        !(len == 3 && strncmp(start, "for", 3) == 0) &&
                        !(len == 6 && strncmp(start, "switch", 6) == 0) &&
                        !(len == 6 && strncmp(start, "return", 6) == 0) &&
                        !(len == 6 && strncmp(start, "sizeof", 6) == 0)) {
                        count++;
                    }
                }
                p++;
            }
        } else {
            p++;
        }
    }
    
    return count;
}

/* Analyze a C source file and populate metrics */
int analyze_file(const char* filename, CodeMetrics* metrics) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }
    
    char line[1024];
    int in_multiline_comment = 0;
    
    while (fgets(line, sizeof(line), file) != NULL) {
        metrics->total_lines++;
        metrics->total_chars += (int)strlen(line);
        
        /* Check for multi-line comment transitions */
        if (strstr(line, "/*") != NULL) {
            in_multiline_comment = 1;
        }
        
        if (is_blank_line(line)) {
            metrics->blank_lines++;
        } else if (is_comment_line(line) || in_multiline_comment) {
            metrics->comment_lines++;
        } else {
            metrics->code_lines++;
        }
        
        /* Check for multi-line comment end */
        if (strstr(line, "*/") != NULL) {
            in_multiline_comment = 0;
        }
        
        /* Count specific code elements */
        if (is_include_line(line)) {
            metrics->include_count++;
        }
        
        if (contains_function_definition(line)) {
            metrics->function_count++;
        }
        
        /* Count function calls */
        metrics->function_calls += count_function_calls(line);
        
        /* Simple heuristic for variable declarations */
        /* Look for lines with 'int', 'char', 'float', 'double', etc. */
        if (!is_comment_line(line) && !in_multiline_comment) {
            if ((strstr(line, "int ") || strstr(line, "char ") || 
                 strstr(line, "float ") || strstr(line, "double ") ||
                 strstr(line, "void ") || strstr(line, "long ")) &&
                !contains_function_definition(line) &&
                !is_include_line(line) &&
                strchr(line, ';') != NULL) {
                metrics->variable_declarations++;
            }
        }
    }
    
    fclose(file);
    return 0;
}

/* Display metrics in a spreadsheet-like table format */
void display_metrics_table(const CodeMetrics* metrics, const char* filename) {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                      CODE METRICS SPREADSHEET                             ║\n");
    printf("╠═══════════════════════════════════════════════════════════════════════════╣\n");
    printf("║ File: %-67s ║\n", filename);
    printf("╠═══════════════════════════════════════════════════════════════════════════╣\n");
    printf("║                                                                           ║\n");
    printf("║  METRIC                                              VALUE                ║\n");
    printf("║                                                                           ║\n");
    printf("╠═══════════════════════════════════════════════════════════════════════════╣\n");
    printf("║  Lines of Code (Total)                               %-20d║\n", metrics->total_lines);
    printf("║  Lines of Code (Code only)                           %-20d║\n", metrics->code_lines);
    printf("║  Lines of Code (Comments)                            %-20d║\n", metrics->comment_lines);
    printf("║  Lines of Code (Blank)                               %-20d║\n", metrics->blank_lines);
    printf("╠═══════════════════════════════════════════════════════════════════════════╣\n");
    printf("║  Total Characters                                    %-20d║\n", metrics->total_chars);
    printf("╠═══════════════════════════════════════════════════════════════════════════╣\n");
    printf("║  Function Definitions                                %-20d║\n", metrics->function_count);
    printf("║  Function Calls                                      %-20d║\n", metrics->function_calls);
    printf("╠═══════════════════════════════════════════════════════════════════════════╣\n");
    printf("║  Include Directives                                  %-20d║\n", metrics->include_count);
    printf("║  Variable Declarations                               %-20d║\n", metrics->variable_declarations);
    printf("╠═══════════════════════════════════════════════════════════════════════════╣\n");
    printf("║                          SUMMARY STATISTICS                               ║\n");
    printf("╠═══════════════════════════════════════════════════════════════════════════╣\n");
    
    /* Calculate percentages */
    if (metrics->total_lines > 0) {
        float code_percent = (float)metrics->code_lines * 100.0f / (float)metrics->total_lines;
        float comment_percent = (float)metrics->comment_lines * 100.0f / (float)metrics->total_lines;
        float blank_percent = (float)metrics->blank_lines * 100.0f / (float)metrics->total_lines;
        
        printf("║  Code Coverage                                       %5.1f%%              ║\n", code_percent);
        printf("║  Comment Coverage                                    %5.1f%%              ║\n", comment_percent);
        printf("║  Blank Line Coverage                                 %5.1f%%              ║\n", blank_percent);
    }
    
    printf("╚═══════════════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}
