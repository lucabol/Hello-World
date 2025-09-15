#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structure to hold code metrics
typedef struct {
    int total_lines;
    int code_lines;
    int blank_lines;
    int comment_lines;
    int include_count;
    int function_count;
    int variable_count;
    int printf_count;
    long file_size;
} CodeMetrics;

// Function to analyze a C source file
CodeMetrics analyze_file(const char* filename) {
    CodeMetrics metrics = {0};
    FILE* file = fopen(filename, "r");
    
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return metrics;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    metrics.file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char line[1024];
    int in_multiline_comment = 0;
    
    while (fgets(line, sizeof(line), file)) {
        metrics.total_lines++;
        
        // Remove leading whitespace for analysis
        char* trimmed = line;
        while (isspace(*trimmed)) trimmed++;
        
        // Check for blank lines
        if (strlen(trimmed) == 0 || *trimmed == '\n') {
            metrics.blank_lines++;
            continue;
        }
        
        // Check for comments
        if (strstr(trimmed, "//") == trimmed) {
            metrics.comment_lines++;
            continue;
        }
        
        // Check for multi-line comments
        if (strstr(trimmed, "/*")) {
            in_multiline_comment = 1;
            metrics.comment_lines++;
            if (strstr(trimmed, "*/")) {
                in_multiline_comment = 0;
            }
            continue;
        }
        
        if (in_multiline_comment) {
            metrics.comment_lines++;
            if (strstr(trimmed, "*/")) {
                in_multiline_comment = 0;
            }
            continue;
        }
        
        // Count as code line
        metrics.code_lines++;
        
        // Count includes (handle both #include and # include)
        if (strstr(trimmed, "#include") || strstr(trimmed, "# include")) {
            metrics.include_count++;
        }
        
        // Count function definitions (simplified)
        if (strstr(trimmed, "int main") || strstr(trimmed, "void ") || 
            strstr(trimmed, "char ") || strstr(trimmed, "float ") ||
            strstr(trimmed, "double ")) {
            if (strstr(trimmed, "(") && strstr(trimmed, ")")) {
                metrics.function_count++;
            }
        }
        
        // Count variable declarations (simplified)
        if ((strstr(trimmed, "int ") || strstr(trimmed, "char ") || 
             strstr(trimmed, "float ") || strstr(trimmed, "double ")) &&
            !strstr(trimmed, "(") && strstr(trimmed, ";")) {
            metrics.variable_count++;
        }
        
        // Count printf statements
        if (strstr(trimmed, "printf")) {
            metrics.printf_count++;
        }
    }
    
    fclose(file);
    return metrics;
}

// Function to display metrics in spreadsheet-like format
void display_metrics_table(const char* filename, CodeMetrics metrics) {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                          CODE METRICS ANALYSIS REPORT                        ║\n");
    printf("╠═══════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║ File: %-71s ║\n", filename);
    printf("╠═══════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║                                  METRICS                                     ║\n");
    printf("╠═══════════════════════════════════════════╤═══════════════════════════════════╣\n");
    printf("║ Metric                                    │ Value                             ║\n");
    printf("╠═══════════════════════════════════════════╪═══════════════════════════════════╣\n");
    printf("║ Total Lines                               │ %-33d ║\n", metrics.total_lines);
    printf("║ Code Lines (non-blank, non-comment)      │ %-33d ║\n", metrics.code_lines);
    printf("║ Blank Lines                               │ %-33d ║\n", metrics.blank_lines);
    printf("║ Comment Lines                             │ %-33d ║\n", metrics.comment_lines);
    printf("║ Include Statements                        │ %-33d ║\n", metrics.include_count);
    printf("║ Function Definitions                      │ %-33d ║\n", metrics.function_count);
    printf("║ Variable Declarations                     │ %-33d ║\n", metrics.variable_count);
    printf("║ Printf Statements                         │ %-33d ║\n", metrics.printf_count);
    printf("║ File Size (bytes)                         │ %-33ld ║\n", metrics.file_size);
    printf("╚═══════════════════════════════════════════╧═══════════════════════════════════╝\n");
    printf("\n");
}

// Function to display interactive menu
void display_menu() {
    printf("╔══════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                        INTERACTIVE METRICS ANALYZER                         ║\n");
    printf("╠══════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║ Options:                                                                     ║\n");
    printf("║ 1. Analyze hello.c                                                          ║\n");
    printf("║ 2. Display detailed breakdown                                               ║\n");
    printf("║ 3. Export metrics to text                                                   ║\n");
    printf("║ 4. Exit                                                                     ║\n");
    printf("╚══════════════════════════════════════════════════════════════════════════════╝\n");
    printf("Enter your choice (1-4): ");
}

// Function to display detailed breakdown
void display_detailed_breakdown(CodeMetrics metrics) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                             DETAILED BREAKDOWN                               ║\n");
    printf("╠══════════════════════════════════════════════════════════════════════════════╣\n");
    
    // Calculate percentages
    double code_percentage = (metrics.total_lines > 0) ? 
        (double)metrics.code_lines / metrics.total_lines * 100 : 0;
    double blank_percentage = (metrics.total_lines > 0) ? 
        (double)metrics.blank_lines / metrics.total_lines * 100 : 0;
    double comment_percentage = (metrics.total_lines > 0) ? 
        (double)metrics.comment_lines / metrics.total_lines * 100 : 0;
    
    printf("║ Line Type Distribution:                                                      ║\n");
    printf("║ ┌─ Code Lines:    %3d (%5.1f%%)                                            ║\n", 
           metrics.code_lines, code_percentage);
    printf("║ ├─ Blank Lines:   %3d (%5.1f%%)                                            ║\n", 
           metrics.blank_lines, blank_percentage);
    printf("║ └─ Comment Lines: %3d (%5.1f%%)                                            ║\n", 
           metrics.comment_lines, comment_percentage);
    printf("║                                                                              ║\n");
    printf("║ Code Complexity Indicators:                                                  ║\n");
    printf("║ ┌─ Functions per file:     %d                                               ║\n", 
           metrics.function_count);
    printf("║ ├─ Variables per function: %.1f                                             ║\n", 
           metrics.function_count > 0 ? (double)metrics.variable_count / metrics.function_count : 0);
    printf("║ └─ Lines per function:     %.1f                                             ║\n", 
           metrics.function_count > 0 ? (double)metrics.code_lines / metrics.function_count : 0);
    printf("╚══════════════════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

// Function to export metrics to text file
void export_metrics(const char* filename, CodeMetrics metrics) {
    FILE* output = fopen("metrics_report.txt", "w");
    if (!output) {
        printf("Error: Could not create output file\n");
        return;
    }
    
    fprintf(output, "CODE METRICS ANALYSIS REPORT\n");
    fprintf(output, "===========================\n\n");
    fprintf(output, "File: %s\n\n", filename);
    fprintf(output, "METRICS:\n");
    fprintf(output, "--------\n");
    fprintf(output, "Total Lines: %d\n", metrics.total_lines);
    fprintf(output, "Code Lines: %d\n", metrics.code_lines);
    fprintf(output, "Blank Lines: %d\n", metrics.blank_lines);
    fprintf(output, "Comment Lines: %d\n", metrics.comment_lines);
    fprintf(output, "Include Statements: %d\n", metrics.include_count);
    fprintf(output, "Function Definitions: %d\n", metrics.function_count);
    fprintf(output, "Variable Declarations: %d\n", metrics.variable_count);
    fprintf(output, "Printf Statements: %d\n", metrics.printf_count);
    fprintf(output, "File Size: %ld bytes\n", metrics.file_size);
    
    fclose(output);
    printf("✓ Metrics exported to metrics_report.txt\n");
}

int main() {
    const char* filename = "hello.c";
    CodeMetrics metrics;
    int choice;
    
    printf("╔══════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║               WELCOME TO CODE METRICS SPREADSHEET ANALYZER                  ║\n");
    printf("╚══════════════════════════════════════════════════════════════════════════════╝\n");
    
    while (1) {
        display_menu();
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // Clear input buffer
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        switch (choice) {
            case 1:
                printf("\nAnalyzing %s...\n", filename);
                metrics = analyze_file(filename);
                if (metrics.total_lines > 0) {
                    display_metrics_table(filename, metrics);
                    printf("✓ Analysis complete!\n");
                } else {
                    printf("✗ Analysis failed or file is empty.\n");
                }
                break;
                
            case 2:
                if (metrics.total_lines > 0) {
                    display_detailed_breakdown(metrics);
                } else {
                    printf("Please run analysis first (option 1).\n");
                }
                break;
                
            case 3:
                if (metrics.total_lines > 0) {
                    export_metrics(filename, metrics);
                } else {
                    printf("Please run analysis first (option 1).\n");
                }
                break;
                
            case 4:
                printf("Thank you for using Code Metrics Analyzer!\n");
                exit(0);
                
            default:
                printf("Invalid choice. Please select 1-4.\n");
        }
        
        printf("\nPress Enter to continue...");
        getchar(); // Consume remaining newline
        getchar(); // Wait for user input
    }
    
    return 0;
}