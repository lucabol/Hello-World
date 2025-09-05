#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char metric[32];
    long value;
    char description[64];
} Metric;

void print_separator(int width) {
    for (int i = 0; i < width; i++) {
        printf("-");
    }
    printf("\n");
}

void print_table_header() {
    printf("\n");
    printf("┌────────────────────────────────────────────────────────────┐\n");
    printf("│                    CODE METRICS ANALYZER                   │\n");
    printf("└────────────────────────────────────────────────────────────┘\n");
    printf("\n");
    printf("File: hello.c\n");
    printf("\n");
    print_separator(62);
    printf("| %-20s | %-6s | %-28s |\n", "METRIC", "VALUE", "DESCRIPTION");
    print_separator(62);
}

void print_metric_row(const char* metric, long value, const char* description) {
    printf("| %-20s | %-6ld | %-28s |\n", metric, value, description);
}

void print_table_footer() {
    print_separator(62);
    printf("\n");
}

int analyze_file(const char* filename, Metric metrics[], int max_metrics) {
    (void)max_metrics; // Suppress unused parameter warning
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return 0;
    }

    int total_lines = 0;
    int code_lines = 0;
    int blank_lines = 0;
    int comment_lines = 0;
    int include_count = 0;
    int function_count = 0;
    int printf_calls = 0;
    int return_statements = 0;
    size_t total_chars = 0;

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        total_lines++;
        total_chars += strlen(line);
        
        // Remove leading whitespace for analysis
        char* trimmed = line;
        while (isspace(*trimmed)) trimmed++;
        
        if (strlen(trimmed) == 0) {
            blank_lines++;
        } else if (strncmp(trimmed, "//", 2) == 0 || strncmp(trimmed, "/*", 2) == 0) {
            comment_lines++;
        } else {
            code_lines++;
            
            // Count specific code elements
            if (strstr(line, "#include")) include_count++;
            if (strstr(line, "int main") || strstr(line, "void ") || strstr(line, "char ")) function_count++;
            if (strstr(line, "printf")) printf_calls++;
            if (strstr(line, "return")) return_statements++;
        }
    }
    
    fclose(file);

    // Store metrics
    int metric_idx = 0;
    strcpy(metrics[metric_idx].metric, "Total Lines");
    metrics[metric_idx].value = total_lines;
    strcpy(metrics[metric_idx].description, "All lines in file");
    metric_idx++;

    strcpy(metrics[metric_idx].metric, "Code Lines");
    metrics[metric_idx].value = code_lines;
    strcpy(metrics[metric_idx].description, "Non-blank, non-comment lines");
    metric_idx++;

    strcpy(metrics[metric_idx].metric, "Blank Lines");
    metrics[metric_idx].value = blank_lines;
    strcpy(metrics[metric_idx].description, "Empty or whitespace-only");
    metric_idx++;

    strcpy(metrics[metric_idx].metric, "Comment Lines");
    metrics[metric_idx].value = comment_lines;
    strcpy(metrics[metric_idx].description, "Lines with comments");
    metric_idx++;

    strcpy(metrics[metric_idx].metric, "Include Statements");
    metrics[metric_idx].value = include_count;
    strcpy(metrics[metric_idx].description, "Header file inclusions");
    metric_idx++;

    strcpy(metrics[metric_idx].metric, "Functions");
    metrics[metric_idx].value = function_count;
    strcpy(metrics[metric_idx].description, "Function definitions");
    metric_idx++;

    strcpy(metrics[metric_idx].metric, "Printf Calls");
    metrics[metric_idx].value = printf_calls;
    strcpy(metrics[metric_idx].description, "Output function calls");
    metric_idx++;

    strcpy(metrics[metric_idx].metric, "Return Statements");
    metrics[metric_idx].value = return_statements;
    strcpy(metrics[metric_idx].description, "Function return points");
    metric_idx++;

    strcpy(metrics[metric_idx].metric, "Total Characters");
    metrics[metric_idx].value = (long)total_chars;
    strcpy(metrics[metric_idx].description, "All characters including spaces");
    metric_idx++;

    return metric_idx;
}

int main() {
    const char* filename = "hello.c";
    Metric metrics[20];
    
    int metric_count = analyze_file(filename, metrics, 20);
    
    if (metric_count == 0) {
        return 1;
    }

    print_table_header();
    
    for (int i = 0; i < metric_count; i++) {
        print_metric_row(metrics[i].metric, metrics[i].value, metrics[i].description);
    }
    
    print_table_footer();
    
    printf("Analysis complete. Spreadsheet-like view of code metrics above.\n");
    printf("Run with: gcc -o metrics metrics.c && ./metrics\n");
    
    return 0;
}