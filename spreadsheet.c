#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "metrics.h"

/* Global flags */
static int use_colors = 1;
static char csv_output_path[256] = "metrics.csv";

/* ANSI color codes for better visualization */
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

/* Color printing functions */
static void print_color(const char* color, const char* text) {
    if (use_colors) {
        printf("%s%s%s", color, text, RESET);
    } else {
        printf("%s", text);
    }
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
    if (use_colors) printf(BOLD CYAN);
    printf("╔");
    for (int i = 0; i < total_width; i++) printf("═");
    printf("╗\n");
    
    printf("║  CODE METRICS SPREADSHEET");
    for (int i = 0; i < total_width - 26; i++) printf(" ");
    printf("║\n");
    
    printf("╠");
    for (int i = 0; i < total_width; i++) printf("═");
    printf("╣\n");
    if (use_colors) printf(RESET);
    
    /* File info row */
    printf("║ ");
    if (use_colors) printf(BOLD);
    printf("File:");
    if (use_colors) printf(RESET);
    printf(" ");
    print_cell(filename, col1_width + col2_width - 6, 0);
    printf(" ║\n");
    
    if (use_colors) printf(CYAN);
    printf("╠");
    for (int i = 0; i < total_width; i++) printf("═");
    printf("╣\n");
    if (use_colors) printf(RESET);
    
    /* Column headers */
    printf("║ ");
    if (use_colors) printf(BOLD GREEN);
    print_cell("Metric", col1_width, 0);
    if (use_colors) printf(RESET);
    printf("│ ");
    if (use_colors) printf(BOLD GREEN);
    print_cell("Value", col2_width, 1);
    if (use_colors) printf(RESET);
    printf(" ║\n");
    
    if (use_colors) printf(CYAN);
    printf("╠");
    for (int i = 0; i < col1_width + 1; i++) printf("═");
    printf("╪");
    for (int i = 0; i < col2_width + 1; i++) printf("═");
    printf("╣\n");
    if (use_colors) printf(RESET);
    
    /* Data rows */
    char value[20];
    
    /* Row 1: Total Lines */
    snprintf(value, sizeof(value), "%d", metrics->total_lines);
    printf("║ ");
    if (use_colors) printf(YELLOW);
    print_cell("Total Lines", col1_width, 0);
    if (use_colors) printf(RESET);
    printf("│ ");
    print_cell(value, col2_width, 1);
    printf(" ║\n");
    
    /* Row 2: Code Lines */
    snprintf(value, sizeof(value), "%d", metrics->code_lines);
    printf("║ ");
    if (use_colors) printf(YELLOW);
    print_cell("Code Lines", col1_width, 0);
    if (use_colors) printf(RESET);
    printf("│ ");
    print_cell(value, col2_width, 1);
    printf(" ║\n");
    
    /* Row 3: Blank Lines */
    snprintf(value, sizeof(value), "%d", metrics->blank_lines);
    printf("║ ");
    if (use_colors) printf(YELLOW);
    print_cell("Blank Lines", col1_width, 0);
    if (use_colors) printf(RESET);
    printf("│ ");
    print_cell(value, col2_width, 1);
    printf(" ║\n");
    
    /* Row 4: Comment Lines */
    snprintf(value, sizeof(value), "%d", metrics->comment_lines);
    printf("║ ");
    if (use_colors) printf(YELLOW);
    print_cell("Comment Lines", col1_width, 0);
    if (use_colors) printf(RESET);
    printf("│ ");
    print_cell(value, col2_width, 1);
    printf(" ║\n");
    
    if (use_colors) printf(CYAN);
    printf("╟");
    for (int i = 0; i < col1_width + 1; i++) printf("─");
    printf("┼");
    for (int i = 0; i < col2_width + 1; i++) printf("─");
    printf("╢\n");
    if (use_colors) printf(RESET);
    
    /* Row 5: Function Count */
    snprintf(value, sizeof(value), "%d", metrics->function_count);
    printf("║ ");
    if (use_colors) printf(YELLOW);
    print_cell("Function Count", col1_width, 0);
    if (use_colors) printf(RESET);
    printf("│ ");
    print_cell(value, col2_width, 1);
    printf(" ║\n");
    
    /* Row 6: Include Count */
    snprintf(value, sizeof(value), "%d", metrics->include_count);
    printf("║ ");
    if (use_colors) printf(YELLOW);
    print_cell("Include Count", col1_width, 0);
    if (use_colors) printf(RESET);
    printf("│ ");
    print_cell(value, col2_width, 1);
    printf(" ║\n");
    
    /* Row 7: Printf Count */
    snprintf(value, sizeof(value), "%d", metrics->printf_count);
    printf("║ ");
    if (use_colors) printf(YELLOW);
    print_cell("Printf Count", col1_width, 0);
    if (use_colors) printf(RESET);
    printf("│ ");
    print_cell(value, col2_width, 1);
    printf(" ║\n");
    
    if (use_colors) printf(CYAN);
    printf("╟");
    for (int i = 0; i < col1_width + 1; i++) printf("─");
    printf("┼");
    for (int i = 0; i < col2_width + 1; i++) printf("─");
    printf("╢\n");
    if (use_colors) printf(RESET);
    
    /* Row 8: Max Line Length */
    snprintf(value, sizeof(value), "%d", metrics->max_line_length);
    printf("║ ");
    if (use_colors) printf(YELLOW);
    print_cell("Max Line Length", col1_width, 0);
    if (use_colors) printf(RESET);
    printf("│ ");
    print_cell(value, col2_width, 1);
    printf(" ║\n");
    
    /* Footer */
    if (use_colors) printf(CYAN);
    printf("╚");
    for (int i = 0; i < total_width; i++) printf("═");
    printf("╝\n");
    if (use_colors) printf(RESET);
    printf("\n");
}

/* Interactive menu */
void show_menu(void) {
    if (use_colors) printf(BOLD);
    printf("\nInteractive Menu:\n");
    if (use_colors) printf(RESET);
    printf("  1. Refresh metrics\n");
    printf("  2. Export to CSV\n");
    printf("  3. Show detailed view\n");
    printf("  4. Exit\n");
    printf("\nEnter choice (1-4): ");
    fflush(stdout);
}

/* Export metrics to CSV */
int export_to_csv(const CodeMetrics* metrics, const char* filename, const char* output_path) {
    FILE* csv = fopen(output_path, "w");
    if (!csv) {
        print_color(YELLOW, "Error: Could not create CSV file: ");
        printf("%s\n", output_path);
        perror("  Reason");
        return 1;
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
    
    if (fclose(csv) != 0) {
        print_color(YELLOW, "Warning: Error closing CSV file\n");
        return 1;
    }
    
    print_color(GREEN, "✓ Metrics exported to ");
    printf("%s\n", output_path);
    return 0;
}

/* Display usage information */
void print_usage(const char* program_name) {
    printf("Usage: %s [OPTIONS] [FILE]\n", program_name);
    printf("\nOptions:\n");
    printf("  --plain, --no-color   Disable colored output and use plain text\n");
    printf("  --csv PATH            Set CSV output path (default: metrics.csv)\n");
    printf("  --help                Display this help message\n");
    printf("\nArguments:\n");
    printf("  FILE                  C source file to analyze (default: hello.c)\n");
    printf("\nExamples:\n");
    printf("  %s                    # Analyze hello.c with colors\n", program_name);
    printf("  %s myfile.c           # Analyze myfile.c\n", program_name);
    printf("  %s --plain hello.c    # Analyze without colors\n", program_name);
    printf("  %s --csv out.csv      # Export to custom CSV file\n", program_name);
}

int main(int argc, char* argv[]) {
    const char* filename = "hello.c";
    int i;
    
    /* Parse command line arguments */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--plain") == 0 || strcmp(argv[i], "--no-color") == 0) {
            use_colors = 0;
        } else if (strcmp(argv[i], "--csv") == 0) {
            if (i + 1 < argc) {
                strncpy(csv_output_path, argv[++i], sizeof(csv_output_path) - 1);
                csv_output_path[sizeof(csv_output_path) - 1] = '\0';
            } else {
                fprintf(stderr, "Error: --csv requires a path argument\n");
                return 1;
            }
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Error: Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        } else {
            filename = argv[i];
        }
    }
    
    /* Check if file exists and is readable */
    if (access(filename, R_OK) != 0) {
        fprintf(stderr, "Error: Cannot access file '%s'\n", filename);
        perror("  Reason");
        return 1;
    }
    
    if (use_colors) printf(BOLD BLUE);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║  Code Metrics Spreadsheet Tool              ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    if (use_colors) printf(RESET);
    
    int running = 1;
    while (running) {
        /* Analyze the file */
        CodeMetrics metrics = analyze_file(filename);
        
        if (metrics.total_lines == 0) {
            fprintf(stderr, "Error: File is empty or could not be read: %s\n", filename);
            return 1;
        }
        
        /* Display spreadsheet */
        display_spreadsheet(&metrics, filename);
        
        /* Interactive menu */
        show_menu();
        
        int choice = 0;
        int scan_result = scanf("%d", &choice);
        
        /* Handle EOF (Ctrl-D) */
        if (scan_result == EOF) {
            printf("\n");
            print_color(GREEN, "Goodbye!\n");
            running = 0;
            continue;
        }
        
        /* Clear input buffer */
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        if (scan_result != 1) {
            print_color(YELLOW, "Invalid input. Please enter a number.\n");
            continue;
        }
        
        switch (choice) {
            case 1:
                print_color(GREEN, "Refreshing metrics...\n");
                break;
            case 2:
                export_to_csv(&metrics, filename, csv_output_path);
                break;
            case 3:
                printf("\n");
                if (use_colors) printf(BOLD);
                printf("Detailed Metrics View:\n");
                if (use_colors) printf(RESET);
                print_metrics(&metrics, filename);
                printf("\nPress Enter to continue...");
                while ((c = getchar()) != '\n' && c != EOF);
                break;
            case 4:
                running = 0;
                print_color(GREEN, "Goodbye!\n");
                break;
            default:
                print_color(YELLOW, "Invalid choice. Please try again.\n");
        }
    }
    
    return 0;
}
