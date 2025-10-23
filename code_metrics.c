#include "code_metrics.h"
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

/* Tab expansion width for max line length calculation */
#define TAB_WIDTH 8

/* Helper function to safely copy filename */
static void safe_copy_filename(char* dest, size_t dest_size, const char* src) {
    size_t len = strlen(src);
    if (len >= dest_size) {
        len = dest_size - 1;
    }
    memcpy(dest, src, len);
    dest[len] = '\0';
}

/* Calculate display length of line with tab expansion */
static size_t calculate_display_length(const char* line, size_t len) {
    size_t display_len = 0;
    size_t i;
    for (i = 0; i < len; i++) {
        if (line[i] == '\t') {
            /* Expand tab to next multiple of TAB_WIDTH */
            display_len += TAB_WIDTH - (display_len % TAB_WIDTH);
        } else {
            display_len++;
        }
    }
    return display_len;
}

/* Check if a character is within a string or character literal */
static bool is_in_string_or_char(const char* line, size_t pos) {
    bool in_string = false;
    bool in_char = false;
    size_t i;
    
    for (i = 0; i < pos; i++) {
        if (line[i] == '\\' && i + 1 < pos) {
            i++; /* Skip escaped character */
            continue;
        }
        if (line[i] == '"' && !in_char) {
            in_string = !in_string;
        } else if (line[i] == '\'' && !in_string) {
            in_char = !in_char;
        }
    }
    
    return in_string || in_char;
}

/* Check if line contains a comment marker (outside strings) */
static bool has_comment_marker(const char* line, const char* marker, size_t* pos) {
    char* found = strstr(line, marker);
    while (found != NULL) {
        size_t marker_pos = (size_t)(found - line);
        if (!is_in_string_or_char(line, marker_pos)) {
            if (pos) *pos = marker_pos;
            return true;
        }
        found = strstr(found + 1, marker);
    }
    return false;
}

/* Check if line is likely a function definition (not prototype, control statement, etc.) */
static bool is_function_definition(const char* line, FILE* file, long* file_pos) {
    /* Control keywords that should not be counted as functions */
    const char* control_keywords[] = {
        "if", "for", "while", "switch", "return", "sizeof", "typedef"
    };
    size_t i;
    
    /* Skip lines with control keywords */
    for (i = 0; i < sizeof(control_keywords) / sizeof(control_keywords[0]); i++) {
        if (strstr(line, control_keywords[i])) {
            return false;
        }
    }
    
    /* Check for function pointer or prototype (ends with semicolon) */
    if (strstr(line, ";")) {
        return false;
    }
    
    /* Check if line has opening brace */
    if (strstr(line, "{")) {
        /* Must have a return type keyword */
        const char* type_keywords[] = {
            "int", "void", "char", "float", "double", "long", "short", "unsigned", "signed"
        };
        for (i = 0; i < sizeof(type_keywords) / sizeof(type_keywords[0]); i++) {
            if (strstr(line, type_keywords[i])) {
                return true;
            }
        }
    } else {
        /* Look ahead for opening brace on next non-comment line */
        char* next_line = NULL;
        size_t next_len = 0;
        long current_pos = ftell(file);
        
        if (current_pos >= 0 && getline(&next_line, &next_len, file) != -1) {
            bool has_brace = (strstr(next_line, "{") != NULL);
            free(next_line);
            
            if (has_brace) {
                const char* type_keywords[] = {
                    "int", "void", "char", "float", "double", "long", "short", "unsigned", "signed"
                };
                for (i = 0; i < sizeof(type_keywords) / sizeof(type_keywords[0]); i++) {
                    if (strstr(line, type_keywords[i])) {
                        *file_pos = current_pos; /* Restore position later */
                        return true;
                    }
                }
            }
            
            /* Restore file position */
            fseek(file, current_pos, SEEK_SET);
        }
    }
    
    return false;
}

/* Analyze a C source file and collect metrics */
int analyze_file(const char* filename, CodeMetrics* metrics) {
    FILE* file;
    char* line = NULL;
    size_t line_buffer_size = 0;
    ssize_t line_length;
    bool in_multiline_comment = false;
    
    if (!filename || !metrics) {
        return -1;
    }
    
    /* Initialize metrics */
    safe_copy_filename(metrics->filename, sizeof(metrics->filename), filename);
    metrics->total_lines = 0;
    metrics->code_lines = 0;
    metrics->blank_lines = 0;
    metrics->comment_lines = 0;
    metrics->functions = 0;
    metrics->includes = 0;
    metrics->max_line_length = 0;
    
    file = fopen(filename, "r");
    if (!file) {
        return -1;
    }
    
    while ((line_length = getline(&line, &line_buffer_size, file)) != -1) {
        size_t len = (size_t)line_length;
        bool is_blank = true;
        bool is_comment = false;
        size_t i;
        size_t comment_pos = 0;
        
        metrics->total_lines++;
        
        /* Remove trailing newline/CR for length calculation */
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            len--;
        }
        
        /* Track max line length with tab expansion */
        size_t display_len = calculate_display_length(line, len);
        if (display_len > (size_t)metrics->max_line_length) {
            metrics->max_line_length = (int)display_len;
        }
        
        /* Check for multiline comment continuation */
        if (in_multiline_comment) {
            metrics->comment_lines++;
            if (has_comment_marker(line, "*/", NULL)) {
                in_multiline_comment = false;
            }
            continue;
        }
        
        /* Check if line is blank */
        for (i = 0; i < len; i++) {
            if (!isspace((unsigned char)line[i])) {
                is_blank = false;
                break;
            }
        }
        
        if (is_blank) {
            metrics->blank_lines++;
            continue;
        }
        
        /* Check for single-line comment (outside strings) */
        if (has_comment_marker(line, "//", &comment_pos)) {
            /* Check if there's code before the comment */
            bool has_code_before = false;
            for (i = 0; i < comment_pos; i++) {
                if (!isspace((unsigned char)line[i])) {
                    has_code_before = true;
                    break;
                }
            }
            if (!has_code_before) {
                metrics->comment_lines++;
                is_comment = true;
            } else {
                /* Line has both code and comment */
                metrics->code_lines++;
                continue;
            }
        }
        
        /* Check for multiline comment start (outside strings) */
        if (!is_comment && has_comment_marker(line, "/*", &comment_pos)) {
            /* Check if there's code before the comment */
            bool has_code_before = false;
            for (i = 0; i < comment_pos; i++) {
                if (!isspace((unsigned char)line[i])) {
                    has_code_before = true;
                    break;
                }
            }
            
            if (!has_code_before) {
                metrics->comment_lines++;
                is_comment = true;
                if (!has_comment_marker(line, "*/", NULL)) {
                    in_multiline_comment = true;
                }
            } else {
                /* Line has code before comment */
                metrics->code_lines++;
                if (!has_comment_marker(line, "*/", NULL)) {
                    in_multiline_comment = true;
                }
                continue;
            }
        }
        
        /* Skip further processing if this is a comment line */
        if (is_comment) {
            continue;
        }
        
        /* Count includes (only actual #include directives, not in comments/strings) */
        if (!is_in_string_or_char(line, len)) {
            /* Trim leading whitespace to find # */
            size_t start = 0;
            while (start < len && isspace((unsigned char)line[start])) {
                start++;
            }
            if (start < len && line[start] == '#') {
                /* Check for "include" after optional whitespace */
                start++;
                while (start < len && isspace((unsigned char)line[start])) {
                    start++;
                }
                if (start + 7 <= len && strncmp(&line[start], "include", 7) == 0) {
                    metrics->includes++;
                }
            }
        }
        
        /* Count functions (improved heuristic) */
        if (strstr(line, "(") && strstr(line, ")")) {
            long saved_pos = 0;
            if (is_function_definition(line, file, &saved_pos)) {
                metrics->functions++;
                if (saved_pos > 0) {
                    /* We read ahead, so skip that line */
                    free(line);
                    line = NULL;
                    line_buffer_size = 0;
                    if (getline(&line, &line_buffer_size, file) != -1) {
                        metrics->total_lines++;
                    }
                }
            }
        }
        
        /* Count as code line if not blank or comment */
        if (!is_blank && !is_comment) {
            metrics->code_lines++;
        }
    }
    
    free(line);
    fclose(file);
    return 0;
}

/* Display metrics in a spreadsheet-like table format */
void display_metrics_table(CodeMetrics* metrics, int count) {
    int i;
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                         CODE METRICS SPREADSHEET                               ║\n");
    printf("╠═══════════════╦═════════╦═════════╦═════════╦═════════╦═════════╦═════════════╣\n");
    printf("║ Filename      ║ Total   ║ Code    ║ Blank   ║ Comment ║ Funcs   ║ Max Line    ║\n");
    printf("║               ║ Lines   ║ Lines   ║ Lines   ║ Lines   ║         ║ Length      ║\n");
    printf("╠═══════════════╬═════════╬═════════╬═════════╬═════════╬═════════╬═════════════╣\n");
    
    for (i = 0; i < count; i++) {
        printf("║ %-13s ║ %7d ║ %7d ║ %7d ║ %7d ║ %7d ║ %11d ║\n",
               metrics[i].filename,
               metrics[i].total_lines,
               metrics[i].code_lines,
               metrics[i].blank_lines,
               metrics[i].comment_lines,
               metrics[i].functions,
               metrics[i].max_line_length);
    }
    
    printf("╚═══════════════╩═════════╩═════════╩═════════╩═════════╩═════════╩═════════════╝\n");
    printf("\n");
    
    /* Display includes information */
    printf("Additional Metrics:\n");
    for (i = 0; i < count; i++) {
        printf("  %s: %d #include directive(s)\n", metrics[i].filename, metrics[i].includes);
    }
    printf("\n");
}

/* Display interactive menu */
void display_menu(void) {
    printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                     INTERACTIVE CODE METRICS TOOL                              ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║ Commands:                                                                      ║\n");
    printf("║   r - Refresh metrics                                                          ║\n");
    printf("║   s - Show detailed statistics                                                 ║\n");
    printf("║   h - Display this help menu                                                   ║\n");
    printf("║   q - Quit                                                                     ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
}
