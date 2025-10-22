/* Code Metrics Analyzer - Spreadsheet-like Interface
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

#define MAX_LINE_LENGTH 1024
#define MAX_FUNCTIONS 100
#define MAX_INCLUDES 50

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
}

/* Check if a line is blank (only whitespace) */
int is_blank_line(const char *line) {
    while (*line) {
        if (!isspace((unsigned char)*line)) {
            return 0;
        }
        line++;
    }
    return 1;
}

/* Check if a line contains a comment */
int is_comment_line(const char *line) {
    const char *p = line;
    while (*p && isspace((unsigned char)*p)) {
        p++;
    }
    return (p[0] == '/' && (p[1] == '/' || p[1] == '*'));
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
    char include_name[256];
    char func_name[256];
    
    while (fgets(line, sizeof(line), file)) {
        int line_len = (int)strlen(line);
        metrics->total_lines++;
        metrics->total_chars += line_len;
        metrics->total_words += count_words(line);
        
        if (line_len > metrics->max_line_length) {
            metrics->max_line_length = line_len;
        }
        
        if (is_blank_line(line)) {
            metrics->blank_lines++;
        } else if (is_comment_line(line)) {
            metrics->comment_lines++;
        } else {
            metrics->code_lines++;
        }
        
        /* Check for includes */
        if (extract_include(line, include_name)) {
            if (metrics->include_count < MAX_INCLUDES) {
                strncpy(metrics->includes[metrics->include_count], include_name, 255);
                metrics->includes[metrics->include_count][255] = '\0';
                metrics->include_count++;
            }
        }
        
        /* Check for function definitions */
        if (is_function_definition(line, func_name)) {
            if (metrics->function_count < MAX_FUNCTIONS) {
                strncpy(metrics->functions[metrics->function_count], func_name, 255);
                metrics->functions[metrics->function_count][255] = '\0';
                metrics->function_count++;
            }
        }
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

/* Main program */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Code Metrics Analyzer - Spreadsheet Interface\n");
        printf("Usage: %s <source_file>\n", argv[0]);
        printf("Example: %s hello.c\n", argv[0]);
        return 1;
    }
    
    const char *filename = argv[1];
    CodeMetrics metrics;
    
    init_metrics(&metrics);
    
    if (!analyze_file(filename, &metrics)) {
        return 1;
    }
    
    display_metrics(filename, &metrics);
    
    return 0;
}
