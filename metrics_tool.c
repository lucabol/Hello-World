#include "code_metrics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Display detailed statistics */
void display_detailed_stats(CodeMetrics* metrics) {
    float code_percentage;
    float comment_percentage;
    float blank_percentage;
    
    printf("\n╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                        DETAILED STATISTICS - %s%-26s║\n", metrics->filename, "");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    
    if (metrics->total_lines > 0) {
        code_percentage = (float)metrics->code_lines * 100.0f / (float)metrics->total_lines;
        comment_percentage = (float)metrics->comment_lines * 100.0f / (float)metrics->total_lines;
        blank_percentage = (float)metrics->blank_lines * 100.0f / (float)metrics->total_lines;
        
        printf("║ Total Lines:          %7d                                                 ║\n", metrics->total_lines);
        printf("║ Code Lines:           %7d  (%5.1f%%)                                        ║\n", 
               metrics->code_lines, code_percentage);
        printf("║ Comment Lines:        %7d  (%5.1f%%)                                        ║\n", 
               metrics->comment_lines, comment_percentage);
        printf("║ Blank Lines:          %7d  (%5.1f%%)                                        ║\n", 
               metrics->blank_lines, blank_percentage);
        printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
        printf("║ Functions:            %7d                                                 ║\n", metrics->functions);
        printf("║ Include Directives:   %7d                                                 ║\n", metrics->includes);
        printf("║ Max Line Length:      %7d characters                                      ║\n", metrics->max_line_length);
        
        /* Calculate average lines per function if there are functions */
        if (metrics->functions > 0) {
            float avg_lines = (float)metrics->code_lines / (float)metrics->functions;
            printf("║ Avg Lines/Function:   %7.1f                                                ║\n", avg_lines);
        }
    } else {
        printf("║ File is empty                                                                  ║\n");
    }
    
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n\n");
}

int main(int argc, char* argv[]) {
    CodeMetrics metrics;
    char command[10];
    int result;
    const char* filename = "hello.c";
    
    /* Allow specifying a different file */
    if (argc > 1) {
        filename = argv[1];
    }
    
    printf("\n");
    printf("════════════════════════════════════════════════════════════════════════════════\n");
    printf("                   CODE METRICS SPREADSHEET TOOL v1.0                          \n");
    printf("════════════════════════════════════════════════════════════════════════════════\n");
    printf("\nAnalyzing: %s\n", filename);
    
    /* Initial analysis */
    result = analyze_file(filename, &metrics);
    if (result != 0) {
        fprintf(stderr, "Failed to analyze file: %s\n", filename);
        return 1;
    }
    
    /* Display initial metrics */
    display_metrics_table(&metrics, 1);
    
    /* Interactive mode */
    display_menu();
    
    printf("\nEnter command (h for help): ");
    fflush(stdout);
    
    while (fgets(command, sizeof(command), stdin)) {
        /* Remove newline */
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }
        
        /* Skip empty input */
        if (strlen(command) == 0) {
            printf("Enter command (h for help): ");
            fflush(stdout);
            continue;
        }
        
        /* Process command */
        char cmd = (char)tolower((unsigned char)command[0]);
        
        switch (cmd) {
            case 'r':
                /* Refresh metrics */
                printf("\nRefreshing metrics for %s...\n", filename);
                result = analyze_file(filename, &metrics);
                if (result == 0) {
                    display_metrics_table(&metrics, 1);
                } else {
                    fprintf(stderr, "Failed to refresh metrics\n");
                }
                break;
                
            case 's':
                /* Show detailed statistics */
                display_detailed_stats(&metrics);
                break;
                
            case 'h':
                /* Display help */
                display_menu();
                break;
                
            case 'q':
                /* Quit */
                printf("\nExiting Code Metrics Tool. Goodbye!\n\n");
                return 0;
                
            default:
                printf("Unknown command: %s\n", command);
                printf("Type 'h' for help.\n");
                break;
        }
        
        printf("\nEnter command (h for help): ");
        fflush(stdout);
    }
    
    printf("\n");
    return 0;
}
