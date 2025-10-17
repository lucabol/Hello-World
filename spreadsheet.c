/* Spreadsheet Interface for Code Metrics
 * Interactive tool for analyzing and displaying code metrics in tabular format
 * 
 * Usage:
 *   ./spreadsheet [filename]
 *   
 * If no filename is provided, defaults to hello.c
 * 
 * Features:
 *   - Displays code metrics in a spreadsheet-like table
 *   - Interactive menu for different views
 *   - Detailed breakdowns of code structure
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code_metrics.h"

/* Display the main menu */
void display_menu(void) {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                     CODE METRICS SPREADSHEET MENU                         ║\n");
    printf("╠═══════════════════════════════════════════════════════════════════════════╣\n");
    printf("║  1. Display Full Metrics Table                                           ║\n");
    printf("║  2. Display Line Counts Only                                             ║\n");
    printf("║  3. Display Code Structure Only                                          ║\n");
    printf("║  4. Display Summary Statistics                                           ║\n");
    printf("║  5. Export to CSV Format                                                 ║\n");
    printf("║  6. Re-analyze File                                                      ║\n");
    printf("║  0. Exit                                                                 ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════════════╝\n");
    printf("\nSelect an option (0-6): ");
}

/* Display only line count metrics */
void display_line_counts(const CodeMetrics* metrics, const char* filename) {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                        LINE COUNT METRICS                                 ║\n");
    printf("╠═══════════════════════════════════════════════════════════════════════════╣\n");
    printf("║ File: %-67s ║\n", filename);
    printf("╠═══════════════════════════════════════════════════════════════════════════╣\n");
    printf("║  Total Lines                                         %-20d║\n", metrics->total_lines);
    printf("║  Code Lines                                          %-20d║\n", metrics->code_lines);
    printf("║  Comment Lines                                       %-20d║\n", metrics->comment_lines);
    printf("║  Blank Lines                                         %-20d║\n", metrics->blank_lines);
    printf("╚═══════════════════════════════════════════════════════════════════════════╝\n");
}

/* Display only code structure metrics */
void display_code_structure(const CodeMetrics* metrics, const char* filename) {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                      CODE STRUCTURE METRICS                               ║\n");
    printf("╠═══════════════════════════════════════════════════════════════════════════╣\n");
    printf("║ File: %-67s ║\n", filename);
    printf("╠═══════════════════════════════════════════════════════════════════════════╣\n");
    printf("║  Function Definitions                                %-20d║\n", metrics->function_count);
    printf("║  Function Calls                                      %-20d║\n", metrics->function_calls);
    printf("║  Include Directives                                  %-20d║\n", metrics->include_count);
    printf("║  Variable Declarations                               %-20d║\n", metrics->variable_declarations);
    printf("║  Total Characters                                    %-20d║\n", metrics->total_chars);
    printf("╚═══════════════════════════════════════════════════════════════════════════╝\n");
}

/* Display summary statistics */
void display_summary(const CodeMetrics* metrics, const char* filename) {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                         SUMMARY STATISTICS                                ║\n");
    printf("╠═══════════════════════════════════════════════════════════════════════════╣\n");
    printf("║ File: %-67s ║\n", filename);
    printf("╠═══════════════════════════════════════════════════════════════════════════╣\n");
    
    if (metrics->total_lines > 0) {
        float code_percent = (float)metrics->code_lines * 100.0f / (float)metrics->total_lines;
        float comment_percent = (float)metrics->comment_lines * 100.0f / (float)metrics->total_lines;
        float blank_percent = (float)metrics->blank_lines * 100.0f / (float)metrics->total_lines;
        float avg_chars_per_line = (float)metrics->total_chars / (float)metrics->total_lines;
        
        printf("║  Code Coverage                                       %5.1f%%              ║\n", code_percent);
        printf("║  Comment Coverage                                    %5.1f%%              ║\n", comment_percent);
        printf("║  Blank Line Coverage                                 %5.1f%%              ║\n", blank_percent);
        printf("║  Average Characters per Line                         %5.1f               ║\n", avg_chars_per_line);
        
        if (metrics->code_lines > 0) {
            float functions_per_line = (float)metrics->function_count * 100.0f / (float)metrics->code_lines;
            float calls_per_line = (float)metrics->function_calls * 100.0f / (float)metrics->code_lines;
            printf("║  Function Density (per 100 LOC)                     %5.1f               ║\n", functions_per_line);
            printf("║  Function Call Density (per 100 LOC)                %5.1f               ║\n", calls_per_line);
        }
    }
    
    printf("╚═══════════════════════════════════════════════════════════════════════════╝\n");
}

/* Export metrics to CSV format */
void export_to_csv(const CodeMetrics* metrics, const char* filename) {
    char csv_filename[256];
    snprintf(csv_filename, sizeof(csv_filename), "%s_metrics.csv", filename);
    
    FILE* csv_file = fopen(csv_filename, "w");
    if (csv_file == NULL) {
        printf("Error: Could not create CSV file '%s'\n", csv_filename);
        return;
    }
    
    fprintf(csv_file, "Metric,Value\n");
    fprintf(csv_file, "File,%s\n", filename);
    fprintf(csv_file, "Total Lines,%d\n", metrics->total_lines);
    fprintf(csv_file, "Code Lines,%d\n", metrics->code_lines);
    fprintf(csv_file, "Comment Lines,%d\n", metrics->comment_lines);
    fprintf(csv_file, "Blank Lines,%d\n", metrics->blank_lines);
    fprintf(csv_file, "Total Characters,%d\n", metrics->total_chars);
    fprintf(csv_file, "Function Definitions,%d\n", metrics->function_count);
    fprintf(csv_file, "Function Calls,%d\n", metrics->function_calls);
    fprintf(csv_file, "Include Directives,%d\n", metrics->include_count);
    fprintf(csv_file, "Variable Declarations,%d\n", metrics->variable_declarations);
    
    if (metrics->total_lines > 0) {
        float code_percent = (float)metrics->code_lines * 100.0f / (float)metrics->total_lines;
        float comment_percent = (float)metrics->comment_lines * 100.0f / (float)metrics->total_lines;
        float blank_percent = (float)metrics->blank_lines * 100.0f / (float)metrics->total_lines;
        fprintf(csv_file, "Code Coverage (%%),%.1f\n", code_percent);
        fprintf(csv_file, "Comment Coverage (%%),%.1f\n", comment_percent);
        fprintf(csv_file, "Blank Coverage (%%),%.1f\n", blank_percent);
    }
    
    fclose(csv_file);
    printf("\n✓ Metrics exported to '%s'\n", csv_filename);
}

int main(int argc, char* argv[]) {
    const char* filename = "hello.c";
    
    /* Check if filename provided as argument */
    if (argc > 1) {
        filename = argv[1];
    }
    
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                 CODE METRICS SPREADSHEET TOOL                             ║\n");
    printf("║                      Version 1.0                                          ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════════════╝\n");
    printf("\nAnalyzing file: %s\n", filename);
    
    /* Initialize and analyze file */
    CodeMetrics metrics;
    init_metrics(&metrics);
    
    if (analyze_file(filename, &metrics) != 0) {
        printf("Error: Could not open file '%s'\n", filename);
        printf("Usage: %s [filename]\n", argv[0]);
        return 1;
    }
    
    printf("✓ Analysis complete!\n");
    
    /* Interactive menu loop */
    int choice;
    int running = 1;
    
    while (running) {
        display_menu();
        
        if (scanf("%d", &choice) != 1) {
            /* Clear input buffer */
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input. Please enter a number (0-6).\n");
            continue;
        }
        
        /* Clear input buffer */
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        switch (choice) {
            case 1:
                display_metrics_table(&metrics, filename);
                break;
            case 2:
                display_line_counts(&metrics, filename);
                break;
            case 3:
                display_code_structure(&metrics, filename);
                break;
            case 4:
                display_summary(&metrics, filename);
                break;
            case 5:
                export_to_csv(&metrics, filename);
                break;
            case 6:
                printf("\nRe-analyzing file: %s\n", filename);
                init_metrics(&metrics);
                if (analyze_file(filename, &metrics) != 0) {
                    printf("Error: Could not re-analyze file '%s'\n", filename);
                } else {
                    printf("✓ Analysis complete!\n");
                }
                break;
            case 0:
                printf("\nExiting Code Metrics Spreadsheet Tool. Goodbye!\n\n");
                running = 0;
                break;
            default:
                printf("Invalid option. Please select 0-6.\n");
                break;
        }
    }
    
    return 0;
}
