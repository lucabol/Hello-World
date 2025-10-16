/* Code Metrics Spreadsheet Tool
 * Analyzes C source files and displays metrics in a tabular format
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_FILENAME 256

/* Structure to hold code metrics */
typedef struct {
    int total_lines;
    int blank_lines;
    int comment_lines;
    int code_lines;
    int function_count;
    int include_count;
    int brace_pairs;
    int semicolons;
} CodeMetrics;

/* Initialize metrics structure */
void init_metrics(CodeMetrics *metrics) {
    metrics->total_lines = 0;
    metrics->blank_lines = 0;
    metrics->comment_lines = 0;
    metrics->code_lines = 0;
    metrics->function_count = 0;
    metrics->include_count = 0;
    metrics->brace_pairs = 0;
    metrics->semicolons = 0;
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
    
    /* Check if line continues a multi-line comment */
    if (p[0] == '*') {
        return 1;
    }
    
    return 0;
}

/* Analyze a source file and gather metrics */
int analyze_file(const char *filename, CodeMetrics *metrics) {
    FILE *file = fopen(filename, "r");
    char line[MAX_LINE_LENGTH];
    
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return -1;
    }
    
    init_metrics(metrics);
    
    while (fgets(line, sizeof(line), file)) {
        metrics->total_lines++;
        
        /* Count blank lines */
        if (is_blank_line(line)) {
            metrics->blank_lines++;
            continue;
        }
        
        /* Count comment lines */
        if (is_comment_line(line)) {
            metrics->comment_lines++;
        } else {
            metrics->code_lines++;
        }
        
        /* Count includes (handle both "#include" and "# include") */
        if (strstr(line, "include") && (strstr(line, "#") || strstr(line, "# "))) {
            metrics->include_count++;
        }
        
        /* Count function definitions (simplified: look for "int main" or "void func" patterns) */
        if (strstr(line, "int main") || strstr(line, "void ") || strstr(line, "char *")) {
            if (strchr(line, '(') && strchr(line, ')')) {
                metrics->function_count++;
            }
        }
        
        /* Count braces */
        for (const char *p = line; *p; p++) {
            if (*p == '{' || *p == '}') {
                metrics->brace_pairs++;
            }
            if (*p == ';') {
                metrics->semicolons++;
            }
        }
    }
    
    /* Normalize brace pairs */
    metrics->brace_pairs /= 2;
    
    fclose(file);
    return 0;
}

/* Display metrics in a spreadsheet-like table format */
void display_metrics_table(const char *filename, CodeMetrics *metrics) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════╗\n");
    printf("║           CODE METRICS SPREADSHEET ANALYSIS                    ║\n");
    printf("╠════════════════════════════════════════════════════════════════╣\n");
    printf("║ File: %-56s ║\n", filename);
    printf("╠══════════════════════════════════╦═════════════════════════════╣\n");
    printf("║ Metric                           ║ Value                       ║\n");
    printf("╠══════════════════════════════════╬═════════════════════════════╣\n");
    printf("║ Total Lines                      ║ %-27d ║\n", metrics->total_lines);
    printf("║ Blank Lines                      ║ %-27d ║\n", metrics->blank_lines);
    printf("║ Comment Lines                    ║ %-27d ║\n", metrics->comment_lines);
    printf("║ Code Lines                       ║ %-27d ║\n", metrics->code_lines);
    printf("╠══════════════════════════════════╬═════════════════════════════╣\n");
    printf("║ Include Statements               ║ %-27d ║\n", metrics->include_count);
    printf("║ Functions                        ║ %-27d ║\n", metrics->function_count);
    printf("║ Brace Pairs                      ║ %-27d ║\n", metrics->brace_pairs);
    printf("║ Semicolons                       ║ %-27d ║\n", metrics->semicolons);
    printf("╠══════════════════════════════════╬═════════════════════════════╣\n");
    printf("║ Code Density                     ║ %-26.1f%% ║\n", 
           (float)metrics->code_lines / metrics->total_lines * 100);
    printf("╚══════════════════════════════════╩═════════════════════════════╝\n");
    printf("\n");
}

/* Display interactive menu */
void display_menu(void) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════╗\n");
    printf("║              INTERACTIVE METRICS MENU                          ║\n");
    printf("╠════════════════════════════════════════════════════════════════╣\n");
    printf("║ 1. Analyze hello.c                                             ║\n");
    printf("║ 2. Display summary                                             ║\n");
    printf("║ 3. Export to CSV                                               ║\n");
    printf("║ 4. Exit                                                        ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    printf("\nSelect option: ");
}

/* Export metrics to CSV format */
void export_to_csv(const char *filename, CodeMetrics *metrics) {
    char csv_filename[MAX_FILENAME];
    snprintf(csv_filename, sizeof(csv_filename), "metrics_%s.csv", filename);
    
    /* Replace .c extension with .csv */
    char *dot = strrchr(csv_filename, '.');
    if (dot && strcmp(dot, ".c.csv") == 0) {
        strcpy(dot, ".csv");
    }
    
    FILE *csv = fopen(csv_filename, "w");
    if (!csv) {
        fprintf(stderr, "Error: Cannot create CSV file\n");
        return;
    }
    
    fprintf(csv, "Metric,Value\n");
    fprintf(csv, "Source File,%s\n", filename);
    fprintf(csv, "Total Lines,%d\n", metrics->total_lines);
    fprintf(csv, "Blank Lines,%d\n", metrics->blank_lines);
    fprintf(csv, "Comment Lines,%d\n", metrics->comment_lines);
    fprintf(csv, "Code Lines,%d\n", metrics->code_lines);
    fprintf(csv, "Include Statements,%d\n", metrics->include_count);
    fprintf(csv, "Functions,%d\n", metrics->function_count);
    fprintf(csv, "Brace Pairs,%d\n", metrics->brace_pairs);
    fprintf(csv, "Semicolons,%d\n", metrics->semicolons);
    fprintf(csv, "Code Density,%.1f%%\n", (float)metrics->code_lines / metrics->total_lines * 100);
    
    fclose(csv);
    printf("\n✓ Metrics exported to: %s\n", csv_filename);
}

/* Display summary in compact format */
void display_summary(const char *filename, CodeMetrics *metrics) {
    printf("\nSUMMARY for %s:\n", filename);
    printf("  Lines: %d (Code: %d, Blank: %d, Comments: %d)\n", 
           metrics->total_lines, metrics->code_lines, metrics->blank_lines, metrics->comment_lines);
    printf("  Functions: %d, Includes: %d, Statements: %d\n", 
           metrics->function_count, metrics->include_count, metrics->semicolons);
}

int main(int argc, char *argv[]) {
    CodeMetrics metrics;
    const char *default_file = "hello.c";
    const char *target_file = (argc > 1) ? argv[1] : default_file;
    
    /* Non-interactive mode: analyze and display */
    if (argc > 1 && strcmp(argv[1], "-h") == 0) {
        printf("Usage: %s [filename] [--interactive]\n", argv[0]);
        printf("  filename       - C source file to analyze (default: hello.c)\n");
        printf("  --interactive  - Run in interactive mode\n");
        printf("  -h             - Show this help\n");
        return 0;
    }
    
    /* Interactive mode */
    if (argc > 1 && strcmp(argv[1], "--interactive") == 0) {
        int choice;
        int metrics_loaded = 0;
        
        printf("\n╔════════════════════════════════════════════════════════════════╗\n");
        printf("║         SPREADSHEET-LIKE CODE METRICS ANALYZER                ║\n");
        printf("╚════════════════════════════════════════════════════════════════╝\n");
        
        while (1) {
            display_menu();
            
            if (scanf("%d", &choice) != 1) {
                /* Clear input buffer */
                while (getchar() != '\n');
                printf("Invalid input. Please enter a number.\n");
                continue;
            }
            
            switch (choice) {
                case 1:
                    if (analyze_file(default_file, &metrics) == 0) {
                        display_metrics_table(default_file, &metrics);
                        metrics_loaded = 1;
                    }
                    break;
                    
                case 2:
                    if (metrics_loaded) {
                        display_summary(default_file, &metrics);
                    } else {
                        printf("Please analyze a file first (option 1).\n");
                    }
                    break;
                    
                case 3:
                    if (metrics_loaded) {
                        export_to_csv(default_file, &metrics);
                    } else {
                        printf("Please analyze a file first (option 1).\n");
                    }
                    break;
                    
                case 4:
                    printf("\nExiting metrics analyzer. Goodbye!\n");
                    return 0;
                    
                default:
                    printf("Invalid option. Please choose 1-4.\n");
            }
        }
    }
    
    /* Default mode: analyze and display table */
    if (analyze_file(target_file, &metrics) == 0) {
        display_metrics_table(target_file, &metrics);
        printf("Tip: Run with --interactive flag for interactive mode\n");
        printf("     or use: %s <filename> to analyze a specific file\n", argv[0]);
    }
    
    return 0;
}
