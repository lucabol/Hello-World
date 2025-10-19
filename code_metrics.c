/* Code Metrics Analyzer - Spreadsheet-like Interface
 * Analyzes C source files and displays code metrics in a tabular format
 * 
 * Features:
 * - Line counting (total, code, blank, comments)
 * - Function detection and counting
 * - Variable detection and counting
 * - Character and word counting
 * - Interactive tabular display
 * 
 * Usage: ./code_metrics [filename]
 *        Default: analyzes hello.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Metric structure to hold analyzed data */
typedef struct {
    const char* metric_name;
    int value;
    const char* description;
} Metric;

/* Analysis results structure */
typedef struct {
    int total_lines;
    int code_lines;
    int blank_lines;
    int comment_lines;
    int functions;
    int variables;
    int total_chars;
    int total_words;
    int includes;
} AnalysisResults;

/* Function prototypes */
void analyze_file(const char* filename, AnalysisResults* results);
void display_spreadsheet(AnalysisResults* results, const char* filename);
void print_separator(int width);
void print_row(const char* col1, const char* col2, const char* col3, int width);
void print_header_row(const char* col1, const char* col2, const char* col3, int width);
int is_blank_line(const char* line);
int is_comment_line(const char* line);
int contains_function_def(const char* line);
int count_variables_in_line(const char* line);
int count_words_in_line(const char* line);

/* Main entry point */
int main(int argc, char* argv[]) {
    const char* filename = "hello.c";
    AnalysisResults results = {0};
    
    /* Allow user to specify a different file */
    if (argc > 1) {
        filename = argv[1];
    }
    
    /* Analyze the file */
    analyze_file(filename, &results);
    
    /* Display results in spreadsheet format */
    display_spreadsheet(&results, filename);
    
    return 0;
}

/* Analyze a C source file and collect metrics */
void analyze_file(const char* filename, AnalysisResults* results) {
    FILE* file = fopen(filename, "r");
    char line[1024];
    int in_multiline_comment = 0;
    
    if (!file) {
        fprintf(stderr, "Error: Unable to open file '%s'\n", filename);
        exit(1);
    }
    
    /* Reset results */
    memset(results, 0, sizeof(AnalysisResults));
    
    /* Process file line by line */
    while (fgets(line, sizeof(line), file)) {
        results->total_lines++;
        results->total_chars += (int)strlen(line);
        
        /* Check for blank lines */
        if (is_blank_line(line)) {
            results->blank_lines++;
            continue;
        }
        
        /* Track multiline comments */
        if (strstr(line, "/*")) {
            in_multiline_comment = 1;
        }
        if (strstr(line, "*/")) {
            in_multiline_comment = 0;
            results->comment_lines++;
            continue;
        }
        
        /* Check for comment lines */
        if (in_multiline_comment || is_comment_line(line)) {
            results->comment_lines++;
            continue;
        }
        
        /* Count actual code lines */
        results->code_lines++;
        
        /* Count words */
        results->total_words += count_words_in_line(line);
        
        /* Detect functions */
        if (contains_function_def(line)) {
            results->functions++;
        }
        
        /* Detect includes (handle both #include and # include) */
        if (strstr(line, "#include") || strstr(line, "# include")) {
            results->includes++;
        }
        
        /* Count variable declarations (simple heuristic) */
        results->variables += count_variables_in_line(line);
    }
    
    fclose(file);
}

/* Display metrics in a spreadsheet-like table */
void display_spreadsheet(AnalysisResults* results, const char* filename) {
    const int width = 70;
    char value_str[20];
    
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════════════╗\n");
    printf("║           CODE METRICS ANALYZER - SPREADSHEET VIEW                  ║\n");
    printf("╚══════════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("File: %s\n", filename);
    printf("\n");
    
    print_separator(width);
    print_header_row("METRIC", "VALUE", "DESCRIPTION", width);
    print_separator(width);
    
    /* Line metrics */
    snprintf(value_str, sizeof(value_str), "%d", results->total_lines);
    print_row("Total Lines", value_str, "All lines in file", width);
    
    snprintf(value_str, sizeof(value_str), "%d", results->code_lines);
    print_row("Code Lines", value_str, "Lines with actual code", width);
    
    snprintf(value_str, sizeof(value_str), "%d", results->blank_lines);
    print_row("Blank Lines", value_str, "Empty or whitespace-only lines", width);
    
    snprintf(value_str, sizeof(value_str), "%d", results->comment_lines);
    print_row("Comment Lines", value_str, "Lines with comments", width);
    
    print_separator(width);
    
    /* Code structure metrics */
    snprintf(value_str, sizeof(value_str), "%d", results->functions);
    print_row("Functions", value_str, "Function definitions detected", width);
    
    snprintf(value_str, sizeof(value_str), "%d", results->variables);
    print_row("Variables", value_str, "Variable declarations (approx)", width);
    
    snprintf(value_str, sizeof(value_str), "%d", results->includes);
    print_row("Include Files", value_str, "#include directives", width);
    
    print_separator(width);
    
    /* Content metrics */
    snprintf(value_str, sizeof(value_str), "%d", results->total_chars);
    print_row("Total Characters", value_str, "All characters in file", width);
    
    snprintf(value_str, sizeof(value_str), "%d", results->total_words);
    print_row("Total Words", value_str, "Word count (identifiers, keywords)", width);
    
    print_separator(width);
    
    /* Summary statistics */
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════════════╗\n");
    printf("║                           SUMMARY STATISTICS                         ║\n");
    printf("╚══════════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    if (results->total_lines > 0) {
        printf("  Code Density:      %.1f%% (code lines / total lines)\n", 
               (float)results->code_lines * 100.0f / (float)results->total_lines);
        printf("  Comment Ratio:     %.1f%% (comment lines / total lines)\n", 
               (float)results->comment_lines * 100.0f / (float)results->total_lines);
        printf("  Avg Line Length:   %.1f characters\n", 
               (float)results->total_chars / (float)results->total_lines);
    }
    
    if (results->code_lines > 0) {
        printf("  Avg Words/Line:    %.1f words per code line\n", 
               (float)results->total_words / (float)results->code_lines);
    }
    
    printf("\n");
}

/* Print a table separator line */
void print_separator(int width) {
    (void)width; /* Unused parameter */
    printf("+----------------------+----------+-------------------------------------+\n");
}

/* Print a table header row */
void print_header_row(const char* col1, const char* col2, const char* col3, int width) {
    (void)width; /* Unused parameter */
    printf("| %-20s | %-8s | %-35s |\n", col1, col2, col3);
}

/* Print a table data row */
void print_row(const char* col1, const char* col2, const char* col3, int width) {
    (void)width; /* Unused parameter */
    printf("| %-20s | %-8s | %-35s |\n", col1, col2, col3);
}

/* Check if a line is blank (only whitespace) */
int is_blank_line(const char* line) {
    while (*line) {
        if (!isspace((unsigned char)*line)) {
            return 0;
        }
        line++;
    }
    return 1;
}

/* Check if a line is a single-line comment */
int is_comment_line(const char* line) {
    const char* p = line;
    
    /* Skip leading whitespace */
    while (*p && isspace((unsigned char)*p)) {
        p++;
    }
    
    /* Check if line starts with // */
    if (p[0] == '/' && p[1] == '/') {
        return 1;
    }
    
    return 0;
}

/* Check if a line contains a function definition */
int contains_function_def(const char* line) {
    const char* p = line;
    
    /* Skip leading whitespace */
    while (*p && isspace((unsigned char)*p)) {
        p++;
    }
    
    /* Look for pattern: type name( or type name () */
    /* Simple heuristic: lines with '(' followed by ')' and ending with '{' or containing ')' */
    if (strstr(line, "main") && strchr(line, '(')) {
        return 1;
    }
    
    /* Look for other function patterns */
    if (strchr(line, '(') && strchr(line, ')') && 
        (strchr(line, '{') || strstr(line, "void") || strstr(line, "int") || strstr(line, "char"))) {
        /* Exclude function calls (those typically have ';' or are indented) */
        if (!strchr(line, ';') && p == line) {
            return 1;
        }
    }
    
    return 0;
}

/* Count variable declarations in a line (simple heuristic) */
int count_variables_in_line(const char* line) {
    int count = 0;
    
    /* Look for common type declarations */
    if ((strstr(line, "int ") || strstr(line, "char ") || strstr(line, "float ") || 
         strstr(line, "double ") || strstr(line, "long ")) && 
        !strstr(line, "(") && strchr(line, '=')) {
        count = 1;
    }
    
    return count;
}

/* Count words in a line (identifiers and keywords) */
int count_words_in_line(const char* line) {
    int count = 0;
    int in_word = 0;
    
    while (*line) {
        if (isalnum((unsigned char)*line) || *line == '_') {
            if (!in_word) {
                count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        line++;
    }
    
    return count;
}
