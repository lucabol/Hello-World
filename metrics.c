#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1000
#define MAX_FILENAME_LENGTH 256

typedef struct {
    int total_lines;
    int code_lines;
    int empty_lines;
    int comment_lines;
    int include_statements;
    int function_definitions;
    int printf_statements;
    long file_size;
    char filename[MAX_FILENAME_LENGTH];
} CodeMetrics;

typedef enum {
    SORT_BY_NAME = 0,
    SORT_BY_LINES,
    SORT_BY_SIZE,
    SORT_BY_FUNCTIONS
} SortOption;

// Function to check if a line is empty (only whitespace)
int is_empty_line(const char* line) {
    while (*line) {
        if (!isspace(*line)) return 0;
        line++;
    }
    return 1;
}

// Function to check if a line is a comment
int is_comment_line(const char* line) {
    while (*line && isspace(*line)) line++; // Skip leading whitespace
    return (strncmp(line, "//", 2) == 0 || strncmp(line, "/*", 2) == 0);
}

// Function to count occurrences of a substring in a string
int count_occurrences(const char* line, const char* pattern) {
    int count = 0;
    const char* pos = line;
    while ((pos = strstr(pos, pattern)) != NULL) {
        count++;
        pos += strlen(pattern);
    }
    return count;
}

// Function to analyze a C source file
CodeMetrics analyze_file(const char* filename) {
    CodeMetrics metrics = {0};
    strncpy(metrics.filename, filename, MAX_FILENAME_LENGTH - 1);
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return metrics;
    }
    
    char line[MAX_LINE_LENGTH];
    
    // Get file size
    fseek(file, 0, SEEK_END);
    metrics.file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    while (fgets(line, sizeof(line), file)) {
        metrics.total_lines++;
        
        if (is_empty_line(line)) {
            metrics.empty_lines++;
        } else if (is_comment_line(line)) {
            metrics.comment_lines++;
        } else {
            metrics.code_lines++;
        }
        
        // Count include statements (handle both "#include" and "# include")
        if (strstr(line, "#include") || strstr(line, "# include")) {
            metrics.include_statements++;
        }
        
        // Count function definitions (simple heuristic: lines with '{' that aren't includes)
        if (strstr(line, "{") && !strstr(line, "#include")) {
            metrics.function_definitions++;
        }
        
        // Count printf statements
        metrics.printf_statements += count_occurrences(line, "printf");
    }
    
    fclose(file);
    return metrics;
}

// Comparison functions for sorting
int compare_by_name(const void* a, const void* b) {
    const CodeMetrics* ma = (const CodeMetrics*)a;
    const CodeMetrics* mb = (const CodeMetrics*)b;
    return strcmp(ma->filename, mb->filename);
}

int compare_by_lines(const void* a, const void* b) {
    const CodeMetrics* ma = (const CodeMetrics*)a;
    const CodeMetrics* mb = (const CodeMetrics*)b;
    return mb->code_lines - ma->code_lines; // Descending order
}

int compare_by_size(const void* a, const void* b) {
    const CodeMetrics* ma = (const CodeMetrics*)a;
    const CodeMetrics* mb = (const CodeMetrics*)b;
    return (mb->file_size > ma->file_size) - (mb->file_size < ma->file_size); // Descending order
}

int compare_by_functions(const void* a, const void* b) {
    const CodeMetrics* ma = (const CodeMetrics*)a;
    const CodeMetrics* mb = (const CodeMetrics*)b;
    return mb->function_definitions - ma->function_definitions; // Descending order
}

// Function to sort metrics array
void sort_metrics(CodeMetrics* metrics_array, int count, SortOption sort_by) {
    switch (sort_by) {
        case SORT_BY_NAME:
            qsort(metrics_array, (size_t)count, sizeof(CodeMetrics), compare_by_name);
            break;
        case SORT_BY_LINES:
            qsort(metrics_array, (size_t)count, sizeof(CodeMetrics), compare_by_lines);
            break;
        case SORT_BY_SIZE:
            qsort(metrics_array, (size_t)count, sizeof(CodeMetrics), compare_by_size);
            break;
        case SORT_BY_FUNCTIONS:
            qsort(metrics_array, (size_t)count, sizeof(CodeMetrics), compare_by_functions);
            break;
    }
}

// Function to print metrics in a spreadsheet-like table format
void print_metrics_table(CodeMetrics* metrics_array, int count) {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                   CODE METRICS SPREADSHEET                           ║\n");
    printf("╠═══════════════════════════════════════════════════════════════════════════════════════╣\n");
    
    // Header row
    printf("║ %-12s │ %-8s │ %-8s │ %-8s │ %-8s │ %-8s │ %-8s │ %-8s │ %-8s ║\n",
           "File", "Total", "Code", "Empty", "Comments", "Includes", "Functions", "Printf", "Size(B)");
    printf("║              │ Lines    │ Lines    │ Lines    │ Lines    │ Stmts    │ Defs     │ Calls    │          ║\n");
    printf("╠══════════════╪══════════╪══════════╪══════════╪══════════╪══════════╪══════════╪══════════╪══════════╣\n");
    
    // Data rows
    for (int i = 0; i < count; i++) {
        printf("║ %-12s │ %-8d │ %-8d │ %-8d │ %-8d │ %-8d │ %-8d │ %-8d │ %-8ld ║\n",
               metrics_array[i].filename,
               metrics_array[i].total_lines,
               metrics_array[i].code_lines,
               metrics_array[i].empty_lines,
               metrics_array[i].comment_lines,
               metrics_array[i].include_statements,
               metrics_array[i].function_definitions,
               metrics_array[i].printf_statements,
               metrics_array[i].file_size);
    }
    
    printf("╚══════════════╧══════════╧══════════╧══════════╧══════════╧══════════╧══════════╧══════════╧══════════╝\n");
    
    // Summary statistics
    if (count > 0) {
        int total_code_lines = 0, total_functions = 0;
        long total_size = 0;
        for (int i = 0; i < count; i++) {
            total_code_lines += metrics_array[i].code_lines;
            total_functions += metrics_array[i].function_definitions;
            total_size += metrics_array[i].file_size;
        }
        
        printf("\n");
        printf("╔══════════════════════════════════════════════════════════════════════════════════════╗\n");
        printf("║                                    SUMMARY STATISTICS                               ║\n");
        printf("╠══════════════════════════════════════════════════════════════════════════════════════╣\n");
        printf("║ Total Files Analyzed    : %-8d                                                   ║\n", count);
        printf("║ Total Lines of Code     : %-8d                                                   ║\n", total_code_lines);
        printf("║ Total Function Defs     : %-8d                                                   ║\n", total_functions);
        printf("║ Total Size (bytes)      : %-8ld                                                   ║\n", total_size);
        printf("║ Average Lines per File  : %-8.1f                                                   ║\n", 
               count > 0 ? (double)total_code_lines / (double)count : 0.0);
        printf("╚══════════════════════════════════════════════════════════════════════════════════════╝\n");
    }
}

int main(int argc, char* argv[]) {
    printf("Code Metrics Analyzer - Spreadsheet Interface\n");
    printf("==============================================\n");
    
    if (argc < 2) {
        printf("Usage: %s <file1.c> [file2.c] [...]\n", argv[0]);
        printf("Example: %s hello.c\n", argv[0]);
        printf("Options: Add --sort=[name|lines|size|functions] to sort results\n");
        printf("         %s hello.c --sort=lines\n", argv[0]);
        return 1;
    }
    
    // Parse command line arguments for sort option
    SortOption sort_by = SORT_BY_NAME;
    int file_count = 0;
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--sort=", 7) == 0) {
            char* sort_type = argv[i] + 7;
            if (strcmp(sort_type, "lines") == 0) {
                sort_by = SORT_BY_LINES;
            } else if (strcmp(sort_type, "size") == 0) {
                sort_by = SORT_BY_SIZE;
            } else if (strcmp(sort_type, "functions") == 0) {
                sort_by = SORT_BY_FUNCTIONS;
            } else if (strcmp(sort_type, "name") == 0) {
                sort_by = SORT_BY_NAME;
            } else {
                printf("Warning: Unknown sort option '%s', using default (name)\n", sort_type);
            }
        } else {
            file_count++;
        }
    }
    
    if (file_count == 0) {
        printf("Error: No files specified for analysis\n");
        return 1;
    }
    
    CodeMetrics* metrics_array = malloc((size_t)argc * sizeof(CodeMetrics));
    if (!metrics_array) {
        printf("Error: Memory allocation failed\n");
        return 1;
    }
    
    int valid_files = 0;
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--sort=", 7) != 0) {
            printf("Analyzing file: %s...\n", argv[i]);
            CodeMetrics metrics = analyze_file(argv[i]);
            if (metrics.total_lines > 0 || metrics.file_size > 0) {
                metrics_array[valid_files++] = metrics;
            }
        }
    }
    
    if (valid_files == 0) {
        printf("No valid files found to analyze.\n");
        free(metrics_array);
        return 1;
    }
    
    // Sort the results based on user preference
    sort_metrics(metrics_array, valid_files, sort_by);
    
    // Show sort information
    const char* sort_names[] = {"name", "lines of code", "file size", "function count"};
    printf("Results sorted by: %s\n", sort_names[sort_by]);
    
    print_metrics_table(metrics_array, valid_files);
    
    free(metrics_array);
    return 0;
}