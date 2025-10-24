/* Code Metrics Analyzer - Spreadsheet-like Interface
 * Analyzes C source files and displays metrics in tabular format
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_FILENAME 256

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

/* Check if line is a comment */
int is_comment_line(const char *line) {
    const char *p = line;
    /* Skip leading whitespace */
    while (*p && isspace((unsigned char)*p)) {
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
        
        /* Check for multi-line comments */
        if (strstr(line, "/*")) {
            in_multiline_comment = 1;
        }
        if (in_multiline_comment) {
            metrics->comment_lines++;
            if (strstr(line, "*/")) {
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
        
        /* Check for printf statements */
        if (strstr(line, "printf")) {
            metrics->printf_statements++;
        }
    }
    
    fclose(file);
    return 1;
}

/* Display metrics in spreadsheet-like tabular format */
void display_metrics_table(const char *filename, CodeMetrics *metrics) {
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
    
    /* Calculate some derived metrics */
    float code_ratio = metrics->total_lines > 0 ? 
        (float)metrics->code_lines / metrics->total_lines * 100.0f : 0.0f;
    float comment_ratio = metrics->total_lines > 0 ? 
        (float)metrics->comment_lines / metrics->total_lines * 100.0f : 0.0f;
    
    printf("║ DERIVED METRICS:                                               ║\n");
    printf("║ Code-to-Total Ratio                    ║ %.2f%%%-17s║\n", code_ratio, "");
    printf("║ Comment-to-Total Ratio                 ║ %.2f%%%-17s║\n", comment_ratio, "");
    
    /* Complexity indicator */
    const char *complexity = "Low";
    if (metrics->code_lines > 20) complexity = "Medium";
    if (metrics->code_lines > 50) complexity = "High";
    printf("║ Estimated Complexity                   ║ %-21s ║\n", complexity);
    
    printf("╚════════════════════════════════════════╩═══════════════════════╝\n");
    printf("\n");
}

/* Display help information */
void display_help(void) {
    printf("Code Metrics Analyzer - Spreadsheet-like Interface\n");
    printf("Usage: code_metrics [filename]\n");
    printf("       If no filename provided, analyzes 'hello.c' by default\n");
    printf("\n");
    printf("Displays code metrics in a tabular spreadsheet format including:\n");
    printf("  - Line counts (total, code, blank, comments)\n");
    printf("  - Function definitions\n");
    printf("  - Include statements\n");
    printf("  - Printf statements\n");
    printf("  - Code complexity indicators\n");
}

int main(int argc, char *argv[]) {
    char filename[MAX_FILENAME] = "hello.c";
    CodeMetrics metrics;
    
    /* Parse command line arguments */
    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            display_help();
            return 0;
        }
        strncpy(filename, argv[1], MAX_FILENAME - 1);
        filename[MAX_FILENAME - 1] = '\0';
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
