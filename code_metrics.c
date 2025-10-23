/* Code Metrics Analyzer - Spreadsheet-like Interface
 * SPDX-License-Identifier: MIT
 * 
 * Analyzes C source files and displays various code metrics in a tabular format
 * 
 * Features:
 * - Line count analysis (total, code, blank, comment lines)
 * - Function count and complexity metrics
 * - Character and word count statistics
 * - Include file dependencies
 * - Interactive tabular display with formatting
 * 
 * Usage: ./code_metrics <source_file>
 * Example: ./code_metrics hello.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Configuration limits - centralized for easy modification
 * These values are chosen to handle typical C source files while maintaining
 * reasonable memory usage and performance.
 * 
 * MAX_LINE_LENGTH: Maximum characters per line. Lines longer than this will
 *                  trigger a warning and be handled safely without overflow.
 * MAX_FUNCTIONS:   Maximum number of function definitions to track.
 * MAX_INCLUDES:    Maximum number of include directives to track.
 * 
 * If you need to analyze larger files, increase these values and recompile.
 */
#define MAX_LINE_LENGTH 1024
#define MAX_FUNCTIONS 100
#define MAX_INCLUDES 50
#define VERSION "1.0.0"

/* Structure to hold code metrics */
typedef struct {
    int total_lines;
    int code_lines;
    int blank_lines;
    int comment_lines;
    int function_count;
    int include_count;
    int total_chars;
    int total_words;
    int max_line_length;
    int brace_depth;
    int in_block_comment;  /* Track if we're inside a block comment */
    char functions[MAX_FUNCTIONS][256];
    char includes[MAX_INCLUDES][256];
} CodeMetrics;

/* Initialize metrics structure */
void init_metrics(CodeMetrics *metrics) {
    metrics->total_lines = 0;
    metrics->code_lines = 0;
    metrics->blank_lines = 0;
    metrics->comment_lines = 0;
    metrics->function_count = 0;
    metrics->include_count = 0;
    metrics->total_chars = 0;
    metrics->total_words = 0;
    metrics->max_line_length = 0;
    metrics->brace_depth = 0;
    metrics->in_block_comment = 0;
}

/* Analyze line content to determine if it contains code, comments, or both
 * Returns: 0 = blank, 1 = code only, 2 = comment only, 3 = code with comment
 * Also updates in_block_comment state for multi-line block comments
 */
int analyze_line_content(const char *line, int *in_block_comment) {
    const char *p = line;
    int has_code = 0;
    int has_comment = 0;
    int in_string = 0;
    int in_char = 0;
    
    /* Skip leading whitespace to check if line is blank */
    const char *non_ws = p;
    while (*non_ws && isspace((unsigned char)*non_ws)) {
        non_ws++;
    }
    if (*non_ws == '\0') {
        return 0; /* Blank line */
    }
    
    /* If we're starting in a block comment, this line is a comment */
    if (*in_block_comment) {
        has_comment = 1;
    }
    
    /* Parse the line character by character */
    while (*p) {
        /* Handle escape sequences in strings/chars */
        if ((in_string || in_char) && *p == '\\' && *(p+1)) {
            p += 2;
            continue;
        }
        
        /* Handle string literals */
        if (*p == '"' && !in_char && !*in_block_comment) {
            if (in_string) {
                in_string = 0;
            } else {
                in_string = 1;
                has_code = 1;
            }
            p++;
            continue;
        }
        
        /* Handle character literals */
        if (*p == '\'' && !in_string && !*in_block_comment) {
            if (in_char) {
                in_char = 0;
            } else {
                in_char = 1;
                has_code = 1;
            }
            p++;
            continue;
        }
        
        /* Don't process comment markers inside strings or chars */
        if (in_string || in_char) {
            p++;
            continue;
        }
        
        /* Handle block comment start */
        if (!*in_block_comment && *p == '/' && *(p+1) == '*') {
            *in_block_comment = 1;
            has_comment = 1;
            p += 2;
            continue;
        }
        
        /* Handle block comment end */
        if (*in_block_comment && *p == '*' && *(p+1) == '/') {
            *in_block_comment = 0;
            p += 2;
            continue;
        }
        
        /* Handle line comment */
        if (!*in_block_comment && *p == '/' && *(p+1) == '/') {
            has_comment = 1;
            break; /* Rest of line is comment */
        }
        
        /* If we're not in a comment and found non-whitespace, it's code */
        if (!*in_block_comment && !isspace((unsigned char)*p)) {
            has_code = 1;
        }
        
        p++;
    }
    
    /* Determine the return value */
    if (has_comment && !has_code) {
        return 2; /* Comment only */
    } else if (has_code) {
        return 1; /* Code (possibly with inline comment) */
    } else if (*in_block_comment) {
        return 2; /* Empty line inside block comment */
    }
    
    return 0; /* Shouldn't reach here */
}

/* Check if line contains an include directive */
int extract_include(const char *line, char *include_name) {
    const char *p = line;
    while (*p && isspace((unsigned char)*p)) {
        p++;
    }
    
    if (*p == '#') {
        p++;
        while (*p && isspace((unsigned char)*p)) {
            p++;
        }
        
        if (strncmp(p, "include", 7) == 0) {
            p += 7;
            while (*p && (isspace((unsigned char)*p) || *p == '<' || *p == '"')) {
                p++;
            }
            
            int i = 0;
            while (*p && *p != '>' && *p != '"' && *p != '\n' && i < 255) {
                include_name[i++] = *p++;
            }
            include_name[i] = '\0';
            return 1;
        }
    }
    return 0;
}

/* Count words in a line */
int count_words(const char *line) {
    int count = 0;
    int in_word = 0;
    
    while (*line) {
        if (isspace((unsigned char)*line)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            count++;
        }
        line++;
    }
    return count;
}

/* Check if a name is a C keyword */
int is_c_keyword(const char *name) {
    const char *keywords[] = {
        "auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "enum", "extern", "float", "for", "goto", "if",
        "int", "long", "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
    };
    int num_keywords = 32;
    
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(name, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

/* Check if line contains a function definition */
int is_function_definition(const char *line, char *func_name) {
    /* Simple heuristic: look for pattern "type name(" with '{' or "){" */
    const char *p = line;
    
    /* Skip leading whitespace */
    while (*p && isspace((unsigned char)*p)) {
        p++;
    }
    
    /* Skip if it's a comment or preprocessor directive */
    if (*p == '/' || *p == '#') {
        return 0;
    }
    
    /* Skip control structures and keywords at start of line */
    if (strncmp(p, "if", 2) == 0 || strncmp(p, "while", 5) == 0 || 
        strncmp(p, "for", 3) == 0 || strncmp(p, "switch", 6) == 0 ||
        strncmp(p, "do", 2) == 0) {
        return 0;
    }
    
    /* Look for opening parenthesis */
    const char *paren = strchr(p, '(');
    if (!paren) {
        return 0;
    }
    
    /* Look for closing parenthesis */
    const char *close_paren = strchr(paren, ')');
    if (!close_paren) {
        return 0;
    }
    
    /* Check if there's a '{' after ')' - strong indicator of function definition */
    /* Must be on same line after ')' for better accuracy */
    const char *after_paren = close_paren + 1;
    while (*after_paren && isspace((unsigned char)*after_paren)) {
        after_paren++;
    }
    if (*after_paren != '{') {
        return 0;
    }
    
    /* Extract function name (word before '(') */
    const char *name_end = paren - 1;
    while (name_end > p && isspace((unsigned char)*name_end)) {
        name_end--;
    }
    
    const char *name_start = name_end;
    while (name_start > p && (isalnum((unsigned char)*name_start) || *name_start == '_')) {
        name_start--;
    }
    name_start++;
    
    int len = (int)(name_end - name_start + 1);
    if (len > 0 && len < 256) {
        strncpy(func_name, name_start, (size_t)len);
        func_name[len] = '\0';
        
        /* Reject if it's a C keyword */
        if (is_c_keyword(func_name)) {
            return 0;
        }
        
        /* Must be a valid identifier (at least 1 char, starting with letter or underscore) */
        if (len >= 1 && (isalpha((unsigned char)func_name[0]) || func_name[0] == '_')) {
            return 1;
        }
    }
    
    return 0;
}

/* Analyze a C source file and collect metrics */
int analyze_file(const char *filename, CodeMetrics *metrics) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    char prev_line[MAX_LINE_LENGTH] = "";
    char include_name[256];
    char func_name[256];
    int prev_might_be_func = 0;
    int functions_limit_warned = 0;
    int includes_limit_warned = 0;
    int long_line_warned = 0;
    
    while (fgets(line, sizeof(line), file)) {
        int line_len = (int)strlen(line);
        
        /* Check if line was truncated (no newline at end and buffer is full) */
        if (line_len == MAX_LINE_LENGTH - 1 && line[line_len - 1] != '\n') {
            if (!long_line_warned) {
                fprintf(stderr, "Warning: Line %d exceeds maximum length (%d chars). Line will be truncated.\n",
                        metrics->total_lines + 1, MAX_LINE_LENGTH - 1);
                fprintf(stderr, "         Consider increasing MAX_LINE_LENGTH and recompiling if needed.\n");
                long_line_warned = 1;
            }
            /* Consume the rest of the line */
            int c;
            while ((c = fgetc(file)) != EOF && c != '\n') {
                line_len++;
            }
        }
        
        metrics->total_lines++;
        metrics->total_chars += line_len;
        metrics->total_words += count_words(line);
        
        if (line_len > metrics->max_line_length) {
            metrics->max_line_length = line_len;
        }
        
        /* Analyze line content */
        int content_type = analyze_line_content(line, &metrics->in_block_comment);
        
        if (content_type == 0) {
            metrics->blank_lines++;
        } else if (content_type == 2) {
            metrics->comment_lines++;
        } else {
            /* content_type == 1 or 3: contains code */
            metrics->code_lines++;
        }
        
        /* Check for includes (only on code lines or lines not in block comments) */
        if (content_type == 1 && extract_include(line, include_name)) {
            if (metrics->include_count < MAX_INCLUDES) {
                strncpy(metrics->includes[metrics->include_count], include_name, 255);
                metrics->includes[metrics->include_count][255] = '\0';
                metrics->include_count++;
            } else if (!includes_limit_warned) {
                fprintf(stderr, "Warning: Maximum number of includes (%d) reached. Additional includes will not be tracked.\n",
                        MAX_INCLUDES);
                includes_limit_warned = 1;
            }
        }
        
        /* Check for function definitions (only on code lines) */
        if (content_type == 1) {
            /* Check if current line has a function with opening brace */
            if (is_function_definition(line, func_name)) {
                if (metrics->function_count < MAX_FUNCTIONS) {
                    strncpy(metrics->functions[metrics->function_count], func_name, 255);
                    metrics->functions[metrics->function_count][255] = '\0';
                    metrics->function_count++;
                } else if (!functions_limit_warned) {
                    fprintf(stderr, "Warning: Maximum number of functions (%d) reached. Additional functions will not be tracked.\n",
                            MAX_FUNCTIONS);
                    functions_limit_warned = 1;
                }
                prev_might_be_func = 0;
            }
            /* Check if this line is just an opening brace and prev line had a function signature */
            else if (prev_might_be_func) {
                const char *p = line;
                while (*p && isspace((unsigned char)*p)) p++;
                if (*p == '{') {
                    /* Previous line was a function definition, brace is on this line */
                    if (metrics->function_count < MAX_FUNCTIONS) {
                        strncpy(metrics->functions[metrics->function_count], func_name, 255);
                        metrics->functions[metrics->function_count][255] = '\0';
                        metrics->function_count++;
                    } else if (!functions_limit_warned) {
                        fprintf(stderr, "Warning: Maximum number of functions (%d) reached. Additional functions will not be tracked.\n",
                                MAX_FUNCTIONS);
                        functions_limit_warned = 1;
                    }
                }
                prev_might_be_func = 0;
            }
            /* Check if this line might be a function declaration waiting for brace on next line */
            else {
                /* Look for pattern: name(...) without { or ; */
                const char *p = line;
                while (*p && isspace((unsigned char)*p)) p++;
                if (*p != '/' && *p != '#') {
                    const char *paren = strchr(p, '(');
                    const char *close_paren = paren ? strchr(paren, ')') : NULL;
                    if (close_paren) {
                        const char *after = close_paren + 1;
                        while (*after && isspace((unsigned char)*after)) after++;
                        if (*after != '{' && *after != ';' && *after != '\0' && *after != '\n') {
                            prev_might_be_func = 0;
                        } else if (*after == '\0' || *after == '\n') {
                            /* Might be function def with brace on next line */
                            /* Extract the name */
                            const char *name_end = paren - 1;
                            while (name_end > p && isspace((unsigned char)*name_end)) name_end--;
                            const char *name_start = name_end;
                            while (name_start > p && (isalnum((unsigned char)*name_start) || *name_start == '_')) {
                                name_start--;
                            }
                            name_start++;
                            int len = (int)(name_end - name_start + 1);
                            if (len > 0 && len < 256) {
                                strncpy(func_name, name_start, (size_t)len);
                                func_name[len] = '\0';
                                if (!is_c_keyword(func_name) && 
                                    (isalpha((unsigned char)func_name[0]) || func_name[0] == '_')) {
                                    prev_might_be_func = 1;
                                }
                            }
                        }
                    }
                }
            }
        } else {
            prev_might_be_func = 0;
        }
        
        strncpy(prev_line, line, MAX_LINE_LENGTH - 1);
        prev_line[MAX_LINE_LENGTH - 1] = '\0';
    }
    
    fclose(file);
    return 1;
}

/* Print a horizontal line for table formatting */
void print_table_line(int width) {
    printf("+");
    for (int i = 0; i < width - 2; i++) {
        printf("-");
    }
    printf("+\n");
}

/* Print a table row with label and value */
void print_table_row(const char *label, const char *value, int width) {
    int label_len = (int)strlen(label);
    int value_len = (int)strlen(value);
    int padding = width - label_len - value_len - 7;
    
    printf("| %s:", label);
    for (int i = 0; i < padding; i++) {
        printf(" ");
    }
    printf("%s |\n", value);
}

/* Display metrics in spreadsheet-like table format */
void display_metrics(const char *filename, const CodeMetrics *metrics) {
    const int table_width = 70;
    char value_buf[64];
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║          CODE METRICS ANALYZER - SPREADSHEET VIEW                  ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("File: %s\n\n", filename);
    
    /* Line Statistics Section */
    print_table_line(table_width);
    printf("|%-68s|\n", "  LINE STATISTICS");
    print_table_line(table_width);
    
    snprintf(value_buf, sizeof(value_buf), "%d", metrics->total_lines);
    print_table_row("Total Lines", value_buf, table_width);
    
    snprintf(value_buf, sizeof(value_buf), "%d", metrics->code_lines);
    print_table_row("Code Lines", value_buf, table_width);
    
    snprintf(value_buf, sizeof(value_buf), "%d", metrics->blank_lines);
    print_table_row("Blank Lines", value_buf, table_width);
    
    snprintf(value_buf, sizeof(value_buf), "%d", metrics->comment_lines);
    print_table_row("Comment Lines", value_buf, table_width);
    
    print_table_line(table_width);
    
    /* Character and Word Statistics */
    printf("\n");
    print_table_line(table_width);
    printf("|%-68s|\n", "  CHARACTER & WORD STATISTICS");
    print_table_line(table_width);
    
    snprintf(value_buf, sizeof(value_buf), "%d", metrics->total_chars);
    print_table_row("Total Characters", value_buf, table_width);
    
    snprintf(value_buf, sizeof(value_buf), "%d", metrics->total_words);
    print_table_row("Total Words", value_buf, table_width);
    
    snprintf(value_buf, sizeof(value_buf), "%d", metrics->max_line_length);
    print_table_row("Max Line Length", value_buf, table_width);
    
    print_table_line(table_width);
    
    /* Code Structure Section */
    printf("\n");
    print_table_line(table_width);
    printf("|%-68s|\n", "  CODE STRUCTURE");
    print_table_line(table_width);
    
    snprintf(value_buf, sizeof(value_buf), "%d", metrics->function_count);
    print_table_row("Function Count", value_buf, table_width);
    
    snprintf(value_buf, sizeof(value_buf), "%d", metrics->include_count);
    print_table_row("Include Count", value_buf, table_width);
    
    print_table_line(table_width);
    
    /* Functions List */
    if (metrics->function_count > 0) {
        printf("\n");
        print_table_line(table_width);
        printf("|%-68s|\n", "  FUNCTIONS");
        print_table_line(table_width);
        
        for (int i = 0; i < metrics->function_count; i++) {
            printf("| %2d. %-63s |\n", i + 1, metrics->functions[i]);
        }
        print_table_line(table_width);
    }
    
    /* Include Files List */
    if (metrics->include_count > 0) {
        printf("\n");
        print_table_line(table_width);
        printf("|%-68s|\n", "  INCLUDE FILES");
        print_table_line(table_width);
        
        for (int i = 0; i < metrics->include_count; i++) {
            printf("| %2d. %-63s |\n", i + 1, metrics->includes[i]);
        }
        print_table_line(table_width);
    }
    
    /* Summary */
    printf("\n");
    print_table_line(table_width);
    printf("|%-68s|\n", "  SUMMARY");
    print_table_line(table_width);
    
    float code_percentage = metrics->total_lines > 0 ? 
        ((float)metrics->code_lines / (float)metrics->total_lines) * 100.0f : 0.0f;
    snprintf(value_buf, sizeof(value_buf), "%.1f%%", (double)code_percentage);
    print_table_row("Code Density", value_buf, table_width);
    
    float avg_line_length = metrics->code_lines > 0 ? 
        ((float)metrics->total_chars / (float)metrics->code_lines) : 0.0f;
    snprintf(value_buf, sizeof(value_buf), "%.1f chars", (double)avg_line_length);
    print_table_row("Avg Line Length", value_buf, table_width);
    
    print_table_line(table_width);
    printf("\n");
}

/* Print usage information */
void print_usage(const char *program_name) {
    printf("Code Metrics Analyzer - Spreadsheet Interface v%s\n", VERSION);
    printf("Analyzes C source files and displays code metrics in tabular format.\n\n");
    printf("Usage: %s [OPTIONS] <source_file>\n\n", program_name);
    printf("Options:\n");
    printf("  -h, --help     Show this help message and exit\n");
    printf("  -v, --version  Show version information and exit\n\n");
    printf("Examples:\n");
    printf("  %s hello.c              Analyze hello.c\n", program_name);
    printf("  %s src/myfile.c         Analyze src/myfile.c\n", program_name);
    printf("\nExit codes:\n");
    printf("  0  Success\n");
    printf("  1  Error (file not found, parse error, or invalid arguments)\n");
}

/* Print version information */
void print_version(void) {
    printf("Code Metrics Analyzer v%s\n", VERSION);
    printf("SPDX-License-Identifier: MIT\n");
}

/* Main program */
int main(int argc, char *argv[]) {
    /* Handle no arguments */
    if (argc < 2) {
        fprintf(stderr, "Error: No input file specified\n\n");
        print_usage(argv[0]);
        return 1;
    }
    
    /* Handle flags */
    const char *filename = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            print_version();
            return 0;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Error: Unknown option '%s'\n\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        } else {
            if (filename != NULL) {
                fprintf(stderr, "Error: Multiple files not supported (got '%s' and '%s')\n", filename, argv[i]);
                fprintf(stderr, "Only single-file analysis is currently supported.\n");
                return 1;
            }
            filename = argv[i];
        }
    }
    
    if (filename == NULL) {
        fprintf(stderr, "Error: No input file specified\n\n");
        print_usage(argv[0]);
        return 1;
    }
    
    CodeMetrics metrics;
    
    init_metrics(&metrics);
    
    if (!analyze_file(filename, &metrics)) {
        return 1;
    }
    
    display_metrics(filename, &metrics);
    
    return 0;
}
