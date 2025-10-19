#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "metrics.h"

/* ANSI color codes for better visualization */
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

/* Function to print a horizontal line */
void print_line(int width) {
    for (int i = 0; i < width; i++) {
        printf("─");
    }
    printf("\n");
}

/* Function to print a cell with padding */
void print_cell(const char* content, int width, int align_right) {
    int len = (int)strlen(content);
    int padding = width - len;
    
    if (align_right) {
        for (int i = 0; i < padding; i++) printf(" ");
        printf("%s", content);
    } else {
        printf("%s", content);
        for (int i = 0; i < padding; i++) printf(" ");
    }
}

/* Function to display metrics in spreadsheet format */
void display_spreadsheet(const CodeMetrics* metrics, const char* filename) {
    const int col1_width = 25;
    const int col2_width = 12;
    const int total_width = col1_width + col2_width + 7;
    
    /* Header */
    printf("\n");
    printf(BOLD CYAN);
    printf("╔");
    for (int i = 0; i < total_width; i++) printf("═");
    printf("╗\n");
    
    printf("║  CODE METRICS SPREADSHEET");
    for (int i = 0; i < total_width - 26; i++) printf(" ");
    printf("║\n");
    
    printf("╠");
    for (int i = 0; i < total_width; i++) printf("═");
    printf("╣\n");
    printf(RESET);
    
    /* File info row */
    printf("║ " BOLD "File:" RESET " ");
    print_cell(filename, col1_width + col2_width - 6, 0);
    printf(" ║\n");
    
    printf(CYAN "╠");
    for (int i = 0; i < total_width; i++) printf("═");
    printf("╣\n" RESET);
    
    /* Column headers */
    printf("║ " BOLD GREEN);
    print_cell("Metric", col1_width, 0);
    printf(RESET "│ " BOLD GREEN);
    print_cell("Value", col2_width, 1);
    printf(RESET " ║\n");
    
    printf(CYAN "╠");
    for (int i = 0; i < col1_width + 1; i++) printf("═");
    printf("╪");
    for (int i = 0; i < col2_width + 1; i++) printf("═");
    printf("╣\n" RESET);
    
    /* Data rows */
    char value[20];
    
    /* Row 1: Total Lines */
    sprintf(value, "%d", metrics->total_lines);
    printf("║ " YELLOW);
    print_cell("Total Lines", col1_width, 0);
    printf(RESET "│ ");
    print_cell(value, col2_width, 1);
    printf(" ║\n");
    
    /* Row 2: Code Lines */
    sprintf(value, "%d", metrics->code_lines);
    printf("║ " YELLOW);
    print_cell("Code Lines", col1_width, 0);
    printf(RESET "│ ");
    print_cell(value, col2_width, 1);
    printf(" ║\n");
    
    /* Row 3: Blank Lines */
    sprintf(value, "%d", metrics->blank_lines);
    printf("║ " YELLOW);
    print_cell("Blank Lines", col1_width, 0);
    printf(RESET "│ ");
    print_cell(value, col2_width, 1);
    printf(" ║\n");
    
    /* Row 4: Comment Lines */
    sprintf(value, "%d", metrics->comment_lines);
    printf("║ " YELLOW);
    print_cell("Comment Lines", col1_width, 0);
    printf(RESET "│ ");
    print_cell(value, col2_width, 1);
    printf(" ║\n");
    
    printf(CYAN "╟");
    for (int i = 0; i < col1_width + 1; i++) printf("─");
    printf("┼");
    for (int i = 0; i < col2_width + 1; i++) printf("─");
    printf("╢\n" RESET);
    
    /* Row 5: Function Count */
    sprintf(value, "%d", metrics->function_count);
    printf("║ " YELLOW);
    print_cell("Function Count", col1_width, 0);
    printf(RESET "│ ");
    print_cell(value, col2_width, 1);
    printf(" ║\n");
    
    /* Row 6: Include Count */
    sprintf(value, "%d", metrics->include_count);
    printf("║ " YELLOW);
    print_cell("Include Count", col1_width, 0);
    printf(RESET "│ ");
    print_cell(value, col2_width, 1);
    printf(" ║\n");
    
    /* Row 7: Printf Count */
    sprintf(value, "%d", metrics->printf_count);
    printf("║ " YELLOW);
    print_cell("Printf Count", col1_width, 0);
    printf(RESET "│ ");
    print_cell(value, col2_width, 1);
    printf(" ║\n");
    
    printf(CYAN "╟");
    for (int i = 0; i < col1_width + 1; i++) printf("─");
    printf("┼");
    for (int i = 0; i < col2_width + 1; i++) printf("─");
    printf("╢\n" RESET);
    
    /* Row 8: Max Line Length */
    sprintf(value, "%d", metrics->max_line_length);
    printf("║ " YELLOW);
    print_cell("Max Line Length", col1_width, 0);
    printf(RESET "│ ");
    print_cell(value, col2_width, 1);
    printf(" ║\n");
    
    /* Footer */
    printf(CYAN "╚");
    for (int i = 0; i < total_width; i++) printf("═");
    printf("╝\n" RESET);
    printf("\n");
}

/* Interactive menu */
void show_menu(void) {
    printf(BOLD "\nInteractive Menu:\n" RESET);
    printf("  1. Refresh metrics\n");
    printf("  2. Export to CSV\n");
    printf("  3. Show detailed view\n");
    printf("  4. Exit\n");
    printf("\nEnter choice (1-4): ");
}

/* Export metrics to CSV */
void export_to_csv(const CodeMetrics* metrics, const char* filename) {
    FILE* csv = fopen("metrics.csv", "w");
    if (!csv) {
        printf(YELLOW "Warning: Could not create CSV file\n" RESET);
        return;
    }
    
    fprintf(csv, "Metric,Value\n");
    fprintf(csv, "File,%s\n", filename);
    fprintf(csv, "Total Lines,%d\n", metrics->total_lines);
    fprintf(csv, "Code Lines,%d\n", metrics->code_lines);
    fprintf(csv, "Blank Lines,%d\n", metrics->blank_lines);
    fprintf(csv, "Comment Lines,%d\n", metrics->comment_lines);
    fprintf(csv, "Function Count,%d\n", metrics->function_count);
    fprintf(csv, "Include Count,%d\n", metrics->include_count);
    fprintf(csv, "Printf Count,%d\n", metrics->printf_count);
    fprintf(csv, "Max Line Length,%d\n", metrics->max_line_length);
    
    fclose(csv);
    printf(GREEN "✓ Metrics exported to metrics.csv\n" RESET);
}

int main(int argc, char* argv[]) {
    const char* filename = "hello.c";
    
    /* Allow custom filename from command line */
    if (argc > 1) {
        filename = argv[1];
    }
    
    printf(BOLD BLUE "╔══════════════════════════════════════════════╗\n");
    printf("║  Code Metrics Spreadsheet Tool              ║\n");
    printf("╚══════════════════════════════════════════════╝\n" RESET);
    
    int running = 1;
    while (running) {
        /* Analyze the file */
        CodeMetrics metrics = analyze_file(filename);
        
        if (metrics.total_lines == 0) {
            printf(YELLOW "Error: File not found or empty: %s\n" RESET, filename);
            return 1;
        }
        
        /* Display spreadsheet */
        display_spreadsheet(&metrics, filename);
        
        /* Interactive menu */
        show_menu();
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            /* Clear input buffer */
            while (getchar() != '\n');
            printf(YELLOW "Invalid input. Please enter a number.\n" RESET);
            continue;
        }
        
        /* Clear input buffer */
        while (getchar() != '\n');
        
        switch (choice) {
            case 1:
                printf(GREEN "Refreshing metrics...\n" RESET);
                break;
            case 2:
                export_to_csv(&metrics, filename);
                break;
            case 3:
                printf("\n" BOLD "Detailed Metrics View:\n" RESET);
                print_metrics(&metrics, filename);
                printf("\nPress Enter to continue...");
                getchar();
                break;
            case 4:
                running = 0;
                printf(GREEN "Goodbye!\n" RESET);
                break;
            default:
                printf(YELLOW "Invalid choice. Please try again.\n" RESET);
        }
    }
    
    return 0;
}
