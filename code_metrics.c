/* Code Metrics Analyzer - Spreadsheet-like Interface
 * Analyzes C source files and displays metrics in tabular format
 * 
 * Version: 1.0.0
 * Author: GitHub Copilot
 * 
 * This code is provided as-is without any warranty. It may be freely used,
 * modified, and distributed for any purpose.
 * 
 * Buffer Safety Documentation:
 * - MAX_LINE_LENGTH (1024): Maximum line length for fgets() buffer.
 *   Lines longer than this will be truncated (fgets behavior).
 * - MAX_FILENAME (256): Maximum filename length with explicit null termination.
 * - All string operations use strncpy() with explicit null termination.
 * - fgets() provides automatic null termination and buffer overflow protection.
 * 
 * String Handling:
 * - filename buffer: Always null-terminated via strncpy() + explicit '\0'
 * - line buffer: Null-terminated by fgets(), size-limited by MAX_LINE_LENGTH
 * - No dynamic allocation: All buffers are stack-allocated with fixed sizes
 * 
 * Edge Case Handling:
 * - is_inside_string(): Detects if a position is within a string literal by
 *   counting unescaped quotes. Handles escaped quotes (\") correctly.
 * - Multi-line comments: Tracks comment blocks across lines with state flag.
 * - Mixed line endings: Works with both LF and CRLF line endings.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_FILENAME 256
#define VERSION "1.0.0"

/* Output format mode */
typedef enum {
    FORMAT_UNICODE,  /* Use Unicode box-drawing characters */
    FORMAT_ASCII     /* Use ASCII characters for compatibility */
} OutputFormat;

/* Global output format setting */
static OutputFormat output_format = FORMAT_UNICODE;

/* Metrics structure to store analysis results */
typedef struct {
    int total_lines;
    int code_lines;           /* Non-blank, non-comment lines */
    int blank_lines;
    int comment_lines;
    int function_count;
    int include_count;
    int printf_statements;
    int max_line_length;
} CodeMetrics;

/* Initialize metrics structure */
void init_metrics(CodeMetrics *metrics) {
    metrics->total_lines = 0;
    metrics->code_lines = 0;
    metrics->blank_lines = 0;
    metrics->comment_lines = 0;
    metrics->function_count = 0;
    metrics->include_count = 0;
    metrics->printf_statements = 0;
    metrics->max_line_length = 0;
}

/* Check if line is blank (only whitespace) */
int is_blank_line(const char *line) {
    while (*line) {
        if (!isspace((unsigned char)*line)) {
            return 0;
        }
        line++;
    }
    return 1;
}

/* Check if a position is inside a string literal
 * Simple heuristic: count quotes before position on same line
 * Returns 1 if inside string, 0 otherwise
 */
int is_inside_string(const char *line, const char *pos) {
    int quote_count = 0;
    int escaped = 0;
    const char *p = line;
    
    while (p < pos && *p) {
        if (*p == '\\' && !escaped) {
            escaped = 1;
        } else {
            if (*p == '"' && !escaped) {
                quote_count++;
            }
            escaped = 0;
        }
        p++;
    }
    
    /* Odd number of quotes means we're inside a string */
    return (quote_count % 2) != 0;
}

/* Check if line is a comment (improved to handle strings) */
int is_comment_line(const char *line) {
    const char *p = line;
    /* Skip leading whitespace */
    while (*p && isspace((unsigned char)*p)) {
        p++;
    }
    
    /* Check for single-line comment (not in string) */
    if (p[0] == '/' && p[1] == '/') {
        if (!is_inside_string(line, p)) {
            return 1;
        }
    }
    
    /* Check for multi-line comment start (not in string) */
    if (p[0] == '/' && p[1] == '*') {
        if (!is_inside_string(line, p)) {
            return 1;
        }
    }
    
    /* Check for multi-line comment continuation */
    if (p[0] == '*') {
        return 1;
    }
    
    return 0;
}

/* Check if line contains a function definition */
int is_function_definition(const char *line) {
    /* Simple heuristic: contains '(' and '{' or starts with known return types */
    const char *paren = strchr(line, '(');
    if (paren) {
        /* Check for common patterns like "type name(" or "main(" */
        const char *p = line;
        while (*p && isspace((unsigned char)*p)) p++;
        
        /* Look for return type keywords */
        if (strncmp(p, "int ", 4) == 0 || 
            strncmp(p, "void ", 5) == 0 ||
            strncmp(p, "char ", 5) == 0 ||
            strncmp(p, "float ", 6) == 0 ||
            strncmp(p, "double ", 7) == 0) {
            return 1;
        }
    }
    return 0;
}

/* Analyze a source file and gather metrics */
int analyze_file(const char *filename, CodeMetrics *metrics) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    int in_multiline_comment = 0;
    
    while (fgets(line, sizeof(line), file)) {
        int len = (int)strlen(line);
        metrics->total_lines++;
        
        /* Track max line length */
        if (len > metrics->max_line_length) {
            metrics->max_line_length = len;
        }
        
        /* Check for blank lines */
        if (is_blank_line(line)) {
            metrics->blank_lines++;
            continue;
        }
        
        /* Check for multi-line comments (not in strings) */
        const char *comment_start = strstr(line, "/*");
        if (comment_start && !is_inside_string(line, comment_start)) {
            in_multiline_comment = 1;
        }
        if (in_multiline_comment) {
            metrics->comment_lines++;
            const char *comment_end = strstr(line, "*/");
            if (comment_end && !is_inside_string(line, comment_end)) {
                in_multiline_comment = 0;
            }
            continue;
        }
        
        /* Check for single-line comments */
        if (is_comment_line(line)) {
            metrics->comment_lines++;
            continue;
        }
        
        /* Count code lines */
        metrics->code_lines++;
        
        /* Check for includes (handle both #include and # include) */
        if (strstr(line, "#include") || strstr(line, "# include")) {
            metrics->include_count++;
        }
        
        /* Check for function definitions */
        if (is_function_definition(line)) {
            metrics->function_count++;
        }
        
        /* Check for printf statements (not in comments or strings) */
        const char *printf_pos = strstr(line, "printf");
        if (printf_pos && !is_inside_string(line, printf_pos)) {
            metrics->printf_statements++;
        }
    }
    
    fclose(file);
    return 1;
}

/* Display metrics in spreadsheet-like tabular format */
void display_metrics_table(const char *filename, CodeMetrics *metrics) {
    /* Calculate derived metrics first */
    float code_ratio = metrics->total_lines > 0 ? 
        (float)metrics->code_lines / (float)metrics->total_lines * 100.0f : 0.0f;
    float comment_ratio = metrics->total_lines > 0 ? 
        (float)metrics->comment_lines / (float)metrics->total_lines * 100.0f : 0.0f;
    
    /* Complexity indicator based on lines of code
     * Low: < 20 lines of code (simple, single-purpose functions)
     * Medium: 20-50 lines of code (moderate complexity)
     * High: > 50 lines of code (complex, may need refactoring)
     * Note: This is NOT cyclomatic complexity - it's a simple size-based heuristic
     */
    const char *complexity = "Low";
    if (metrics->code_lines > 20) complexity = "Medium";
    if (metrics->code_lines > 50) complexity = "High";
    
    if (output_format == FORMAT_UNICODE) {
        /* Unicode box-drawing characters for attractive display */
        printf("\n");
        printf("╔════════════════════════════════════════════════════════════════╗\n");
        printf("║          CODE METRICS ANALYZER - SPREADSHEET VIEW              ║\n");
        printf("╠════════════════════════════════════════════════════════════════╣\n");
        printf("║ File: %-56s ║\n", filename);
        printf("╠════════════════════════════════════════╦═══════════════════════╣\n");
        printf("║ Metric                                 ║ Value                 ║\n");
        printf("╠════════════════════════════════════════╬═══════════════════════╣\n");
        printf("║ Total Lines                            ║ %-21d ║\n", metrics->total_lines);
        printf("║ Code Lines (executable)                ║ %-21d ║\n", metrics->code_lines);
        printf("║ Blank Lines                            ║ %-21d ║\n", metrics->blank_lines);
        printf("║ Comment Lines                          ║ %-21d ║\n", metrics->comment_lines);
        printf("║ Function Definitions                   ║ %-21d ║\n", metrics->function_count);
        printf("║ Include Statements                     ║ %-21d ║\n", metrics->include_count);
        printf("║ Printf Statements                      ║ %-21d ║\n", metrics->printf_statements);
        printf("║ Maximum Line Length                    ║ %-21d ║\n", metrics->max_line_length);
        printf("╠════════════════════════════════════════╩═══════════════════════╣\n");
        printf("║ DERIVED METRICS:                                               ║\n");
        printf("║ Code-to-Total Ratio                    ║ %.2f%%%-17s║\n", code_ratio, "");
        printf("║ Comment-to-Total Ratio                 ║ %.2f%%%-17s║\n", comment_ratio, "");
        printf("║ Estimated Complexity                   ║ %-21s ║\n", complexity);
        printf("╚════════════════════════════════════════╩═══════════════════════╝\n");
        printf("\n");
    } else {
        /* ASCII format for compatibility with all terminals and pipes */
        printf("\n");
        printf("+----------------------------------------------------------------+\n");
        printf("|          CODE METRICS ANALYZER - SPREADSHEET VIEW              |\n");
        printf("+----------------------------------------------------------------+\n");
        printf("| File: %-56s |\n", filename);
        printf("+----------------------------------------+-----------------------+\n");
        printf("| Metric                                 | Value                 |\n");
        printf("+----------------------------------------+-----------------------+\n");
        printf("| Total Lines                            | %-21d |\n", metrics->total_lines);
        printf("| Code Lines (executable)                | %-21d |\n", metrics->code_lines);
        printf("| Blank Lines                            | %-21d |\n", metrics->blank_lines);
        printf("| Comment Lines                          | %-21d |\n", metrics->comment_lines);
        printf("| Function Definitions                   | %-21d |\n", metrics->function_count);
        printf("| Include Statements                     | %-21d |\n", metrics->include_count);
        printf("| Printf Statements                      | %-21d |\n", metrics->printf_statements);
        printf("| Maximum Line Length                    | %-21d |\n", metrics->max_line_length);
        printf("+----------------------------------------+-----------------------+\n");
        printf("| DERIVED METRICS:                                               |\n");
        printf("| Code-to-Total Ratio                    | %.2f%%%-17s|\n", code_ratio, "");
        printf("| Comment-to-Total Ratio                 | %.2f%%%-17s|\n", comment_ratio, "");
        printf("| Estimated Complexity                   | %-21s |\n", complexity);
        printf("+----------------------------------------------------------------+\n");
        printf("\n");
    }
}

/* Display help information */
void display_help(void) {
    printf("Code Metrics Analyzer - Spreadsheet-like Interface v%s\n", VERSION);
    printf("Usage: code_metrics [OPTIONS] [filename]\n");
    printf("       If no filename provided, analyzes 'hello.c' by default\n");
    printf("\n");
    printf("Options:\n");
    printf("  -h, --help       Display this help message\n");
    printf("  -v, --version    Display version information\n");
    printf("  -p, --plain      Use plain ASCII output (no Unicode box characters)\n");
    printf("  -a, --ascii      Same as --plain\n");
    printf("\n");
    printf("Displays code metrics in a tabular spreadsheet format including:\n");
    printf("  - Line counts (total, code, blank, comments)\n");
    printf("  - Function definitions\n");
    printf("  - Include statements\n");
    printf("  - Printf statements\n");
    printf("  - Code complexity indicators\n");
    printf("\n");
    printf("Exit codes:\n");
    printf("  0  Success\n");
    printf("  1  Error (file not found, cannot open file, etc.)\n");
}

/* Display version information */
void display_version(void) {
    printf("Code Metrics Analyzer version %s\n", VERSION);
    printf("A spreadsheet-like interface for C source code analysis\n");
}

int main(int argc, char *argv[]) {
    char filename[MAX_FILENAME] = "hello.c";
    CodeMetrics metrics;
    
    /* Parse command line arguments */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            display_help();
            return 0;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            display_version();
            return 0;
        } else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--plain") == 0 || 
                   strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--ascii") == 0) {
            output_format = FORMAT_ASCII;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Error: Unknown option '%s'\n", argv[i]);
            fprintf(stderr, "Try 'code_metrics --help' for more information.\n");
            return 1;
        } else {
            /* This is the filename argument */
            strncpy(filename, argv[i], MAX_FILENAME - 1);
            filename[MAX_FILENAME - 1] = '\0';  /* Explicit null termination for safety */
        }
    }
    
    /* Initialize metrics */
    init_metrics(&metrics);
    
    /* Analyze the file */
    if (!analyze_file(filename, &metrics)) {
        return 1;
    }
    
    /* Display results in spreadsheet format */
    display_metrics_table(filename, &metrics);
    
    return 0;
}
