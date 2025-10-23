/*
 * Code Metrics Spreadsheet Tool - Main Program
 * Part of the lucabol/Hello-World repository
 * 
 * Interactive and non-interactive interface for displaying code metrics
 * in spreadsheet-like format.
 */

#include "code_metrics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <stdbool.h>

#define TOOL_VERSION "1.0.1"

/* Global flag for signal handling */
static volatile sig_atomic_t interrupted = 0;

/* Signal handler for graceful shutdown */
static void handle_signal(int sig) {
    (void)sig; /* Unused parameter */
    interrupted = 1;
}

/* Display version information */
static void display_version(void) {
    printf("Code Metrics Spreadsheet Tool v%s\n", TOOL_VERSION);
    printf("A tool for analyzing C source files and displaying code metrics.\n");
}

/* Display help information */
static void display_help(const char* program_name) {
    printf("Usage: %s [OPTIONS] [FILE]\n\n", program_name);
    printf("Analyze C source files and display code metrics in a spreadsheet-like format.\n\n");
    printf("Options:\n");
    printf("  -h, --help       Display this help message and exit\n");
    printf("  -v, --version    Display version information and exit\n");
    printf("  -n, --non-interactive\n");
    printf("                   Run in non-interactive mode (display metrics and exit)\n");
    printf("  -c, --csv        Output in CSV format (implies --non-interactive)\n");
    printf("\n");
    printf("Arguments:\n");
    printf("  FILE             C source file to analyze (default: hello.c)\n");
    printf("\n");
    printf("Interactive commands:\n");
    printf("  r - Refresh metrics (re-analyze the file)\n");
    printf("  s - Show detailed statistics\n");
    printf("  h - Display help menu\n");
    printf("  q - Quit\n");
    printf("\n");
    printf("Examples:\n");
    printf("  %s                      # Analyze hello.c interactively\n", program_name);
    printf("  %s myfile.c             # Analyze myfile.c interactively\n", program_name);
    printf("  %s -n myfile.c          # Analyze myfile.c non-interactively\n", program_name);
    printf("  %s -c myfile.c          # Output metrics in CSV format\n", program_name);
}

/* Display metrics in CSV format */
static void display_csv(CodeMetrics* metrics) {
    printf("Filename,Total Lines,Code Lines,Blank Lines,Comment Lines,Functions,Includes,Max Line Length\n");
    printf("%s,%d,%d,%d,%d,%d,%d,%d\n",
           metrics->filename,
           metrics->total_lines,
           metrics->code_lines,
           metrics->blank_lines,
           metrics->comment_lines,
           metrics->functions,
           metrics->includes,
           metrics->max_line_length);
}

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
    bool non_interactive = false;
    bool csv_output = false;
    int i;
    
    /* Install signal handlers for graceful shutdown */
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    
    /* Parse command line arguments */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            display_help(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            display_version();
            return 0;
        } else if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--non-interactive") == 0) {
            non_interactive = true;
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--csv") == 0) {
            csv_output = true;
            non_interactive = true;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
            return 1;
        } else {
            filename = argv[i];
        }
    }
    
    /* Analyze the file */
    result = analyze_file(filename, &metrics);
    if (result != 0) {
        fprintf(stderr, "Error: Failed to analyze file '%s'\n", filename);
        fprintf(stderr, "Make sure the file exists and is readable.\n");
        return 1;
    }
    
    /* Handle output based on mode */
    if (csv_output) {
        display_csv(&metrics);
        return 0;
    }
    
    if (non_interactive) {
        printf("\n");
        printf("════════════════════════════════════════════════════════════════════════════════\n");
        printf("                   CODE METRICS SPREADSHEET TOOL v%s\n", TOOL_VERSION);
        printf("════════════════════════════════════════════════════════════════════════════════\n");
        printf("\nAnalyzing: %s\n", filename);
        display_metrics_table(&metrics, 1);
        display_detailed_stats(&metrics);
        return 0;
    }
    
    /* Interactive mode */
    printf("\n");
    printf("════════════════════════════════════════════════════════════════════════════════\n");
    printf("                   CODE METRICS SPREADSHEET TOOL v%s\n", TOOL_VERSION);
    printf("════════════════════════════════════════════════════════════════════════════════\n");
    printf("\nAnalyzing: %s\n", filename);
    
    /* Display initial metrics */
    display_metrics_table(&metrics, 1);
    
    /* Interactive mode */
    display_menu();
    
    printf("\nEnter command (h for help): ");
    fflush(stdout);
    
    while (!interrupted && fgets(command, sizeof(command), stdin)) {
        /* Remove newline */
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }
        
        /* Check for interrupt signal */
        if (interrupted) {
            break;
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
    
    if (interrupted) {
        printf("\n\nReceived interrupt signal. Exiting gracefully...\n\n");
    } else {
        printf("\n");
    }
    
    return 0;
}
