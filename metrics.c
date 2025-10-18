/* Code Metrics Spreadsheet Tool
 * Analyzes hello.c and displays various code metrics in a tabular format
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_FUNCTIONS 50
#define TARGET_FILE "hello.c"

/* Structure to hold file metrics */
typedef struct {
    int total_lines;
    int code_lines;
    int blank_lines;
    int comment_lines;
    int function_count;
    int include_count;
    char functions[MAX_FUNCTIONS][256];
} FileMetrics;

/* Function prototypes */
void analyze_file(const char *filename, FileMetrics *metrics);
void display_spreadsheet(const FileMetrics *metrics);
void display_summary_table(const FileMetrics *metrics);
void display_function_table(const FileMetrics *metrics);
void display_detailed_metrics(const FileMetrics *metrics);
void print_horizontal_line(int width);
void print_table_header(const char *title, int width);
int interactive_menu(void);

/* Main function */
int main(void) {
    FileMetrics metrics = {0};
    int choice;
    
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║          CODE METRICS SPREADSHEET ANALYZER                   ║\n");
    printf("║          Analyzing: %-37s║\n", TARGET_FILE);
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    /* Analyze the target file */
    analyze_file(TARGET_FILE, &metrics);
    
    /* Interactive menu loop */
    do {
        choice = interactive_menu();
        
        switch (choice) {
            case 1:
                display_spreadsheet(&metrics);
                break;
            case 2:
                display_summary_table(&metrics);
                break;
            case 3:
                display_function_table(&metrics);
                break;
            case 4:
                display_detailed_metrics(&metrics);
                break;
            case 5:
                printf("\n✓ Thank you for using Code Metrics Analyzer!\n\n");
                break;
            default:
                printf("\n✗ Invalid choice. Please try again.\n\n");
                break;
        }
    } while (choice != 5);
    
    return 0;
}

/* Analyze the source file and collect metrics */
void analyze_file(const char *filename, FileMetrics *metrics) {
    FILE *file = fopen(filename, "r");
    char line[MAX_LINE_LENGTH];
    int in_multiline_comment = 0;
    
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        exit(1);
    }
    
    while (fgets(line, sizeof(line), file)) {
        metrics->total_lines++;
        
        /* Trim leading whitespace */
        char *trimmed = line;
        while (*trimmed && isspace((unsigned char)*trimmed)) {
            trimmed++;
        }
        
        /* Check for blank lines */
        if (*trimmed == '\0' || *trimmed == '\n') {
            metrics->blank_lines++;
            continue;
        }
        
        /* Check for multi-line comments */
        if (in_multiline_comment) {
            metrics->comment_lines++;
            if (strstr(trimmed, "*/")) {
                in_multiline_comment = 0;
            }
            continue;
        }
        
        /* Check for start of multi-line comment */
        if (strstr(trimmed, "/*")) {
            metrics->comment_lines++;
            in_multiline_comment = 1;
            if (strstr(trimmed, "*/")) {
                in_multiline_comment = 0;
            }
            continue;
        }
        
        /* Check for single-line comments */
        if (trimmed[0] == '/' && trimmed[1] == '/') {
            metrics->comment_lines++;
            continue;
        }
        
        /* Check for #include directives */
        if (strncmp(trimmed, "#include", 8) == 0 || strncmp(trimmed, "# include", 9) == 0) {
            metrics->include_count++;
        }
        
        /* Check for function definitions (simple heuristic) */
        if (strstr(trimmed, "int main") || 
            (strstr(trimmed, "(") && strstr(trimmed, ")") && strstr(trimmed, "{"))) {
            if (metrics->function_count < MAX_FUNCTIONS) {
                /* Extract function name (simplified) */
                char *func_start = trimmed;
                char *paren = strchr(func_start, '(');
                if (paren) {
                    char *name_start = paren - 1;
                    while (name_start > func_start && 
                           (isalnum((unsigned char)*name_start) || *name_start == '_')) {
                        name_start--;
                    }
                    name_start++;
                    int len = (int)(paren - name_start);
                    if (len > 0 && len < 255) {
                        strncpy(metrics->functions[metrics->function_count], name_start, (size_t)len);
                        metrics->functions[metrics->function_count][len] = '\0';
                        metrics->function_count++;
                    }
                }
            }
        }
        
        /* This is a code line */
        metrics->code_lines++;
    }
    
    fclose(file);
}

/* Display the main spreadsheet view */
void display_spreadsheet(const FileMetrics *metrics) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                        METRICS SPREADSHEET VIEW                          ║\n");
    printf("╠══════════════════════════════════════════════════════════════════════════╣\n");
    printf("║ Metric Category          │ Value      │ Percentage │ Notes              ║\n");
    printf("╠══════════════════════════════════════════════════════════════════════════╣\n");
    
    double code_pct = metrics->total_lines > 0 ? 
        (double)metrics->code_lines * 100.0 / (double)metrics->total_lines : 0.0;
    double blank_pct = metrics->total_lines > 0 ? 
        (double)metrics->blank_lines * 100.0 / (double)metrics->total_lines : 0.0;
    double comment_pct = metrics->total_lines > 0 ? 
        (double)metrics->comment_lines * 100.0 / (double)metrics->total_lines : 0.0;
    
    printf("║ Total Lines              │ %-10d │   100.00%% │ All lines          ║\n", 
           metrics->total_lines);
    printf("║ Code Lines               │ %-10d │   %6.2f%% │ Executable code    ║\n", 
           metrics->code_lines, code_pct);
    printf("║ Blank Lines              │ %-10d │   %6.2f%% │ Empty lines        ║\n", 
           metrics->blank_lines, blank_pct);
    printf("║ Comment Lines            │ %-10d │   %6.2f%% │ Documentation      ║\n", 
           metrics->comment_lines, comment_pct);
    printf("╠══════════════════════════════════════════════════════════════════════════╣\n");
    printf("║ Functions                │ %-10d │      N/A   │ Function defs      ║\n", 
           metrics->function_count);
    printf("║ Include Statements       │ %-10d │      N/A   │ Header includes    ║\n", 
           metrics->include_count);
    printf("╚══════════════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

/* Display summary table */
void display_summary_table(const FileMetrics *metrics) {
    printf("\n");
    printf("╔═══════════════════════════════════════╗\n");
    printf("║        SUMMARY METRICS TABLE          ║\n");
    printf("╠═══════════════════════════════════════╣\n");
    printf("║ Metric            │ Value             ║\n");
    printf("╠═══════════════════════════════════════╣\n");
    printf("║ Total Lines       │ %-17d ║\n", metrics->total_lines);
    printf("║ Code Lines        │ %-17d ║\n", metrics->code_lines);
    printf("║ Blank Lines       │ %-17d ║\n", metrics->blank_lines);
    printf("║ Comment Lines     │ %-17d ║\n", metrics->comment_lines);
    printf("║ Functions         │ %-17d ║\n", metrics->function_count);
    printf("║ Includes          │ %-17d ║\n", metrics->include_count);
    printf("╚═══════════════════════════════════════╝\n");
    printf("\n");
}

/* Display function table */
void display_function_table(const FileMetrics *metrics) {
    int i;
    
    printf("\n");
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║          FUNCTION INVENTORY TABLE             ║\n");
    printf("╠═══════════════════════════════════════════════╣\n");
    printf("║ #  │ Function Name                            ║\n");
    printf("╠═══════════════════════════════════════════════╣\n");
    
    if (metrics->function_count == 0) {
        printf("║    │ (No functions detected)                  ║\n");
    } else {
        for (i = 0; i < metrics->function_count; i++) {
            printf("║ %-2d │ %-40s ║\n", i + 1, metrics->functions[i]);
        }
    }
    
    printf("╠═══════════════════════════════════════════════╣\n");
    printf("║ Total Functions: %-28d ║\n", metrics->function_count);
    printf("╚═══════════════════════════════════════════════╝\n");
    printf("\n");
}

/* Display detailed metrics with additional calculations */
void display_detailed_metrics(const FileMetrics *metrics) {
    double code_to_comment_ratio;
    double avg_code_per_function;
    
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║                   DETAILED METRICS ANALYSIS                   ║\n");
    printf("╠═══════════════════════════════════════════════════════════════╣\n");
    
    /* Basic metrics */
    printf("║                                                               ║\n");
    printf("║ Basic Line Metrics:                                           ║\n");
    printf("║   • Total Lines:          %-35d║\n", metrics->total_lines);
    printf("║   • Code Lines:           %-35d║\n", metrics->code_lines);
    printf("║   • Blank Lines:          %-35d║\n", metrics->blank_lines);
    printf("║   • Comment Lines:        %-35d║\n", metrics->comment_lines);
    printf("║                                                               ║\n");
    
    /* Code composition */
    printf("║ Code Composition:                                             ║\n");
    if (metrics->total_lines > 0) {
        printf("║   • Code Percentage:      %.2f%%%-30s║\n", 
               (double)metrics->code_lines * 100.0 / (double)metrics->total_lines, "");
        printf("║   • Comment Percentage:   %.2f%%%-30s║\n", 
               (double)metrics->comment_lines * 100.0 / (double)metrics->total_lines, "");
        printf("║   • Blank Percentage:     %.2f%%%-30s║\n", 
               (double)metrics->blank_lines * 100.0 / (double)metrics->total_lines, "");
    }
    printf("║                                                               ║\n");
    
    /* Structural metrics */
    printf("║ Structural Metrics:                                           ║\n");
    printf("║   • Functions:            %-35d║\n", metrics->function_count);
    printf("║   • Include Statements:   %-35d║\n", metrics->include_count);
    
    if (metrics->function_count > 0) {
        avg_code_per_function = (double)metrics->code_lines / (double)metrics->function_count;
        printf("║   • Avg Code per Function: %-34.2f║\n", avg_code_per_function);
    }
    
    if (metrics->comment_lines > 0) {
        code_to_comment_ratio = (double)metrics->code_lines / (double)metrics->comment_lines;
        printf("║   • Code/Comment Ratio:   %-34.2f║\n", code_to_comment_ratio);
    } else {
        printf("║   • Code/Comment Ratio:   N/A (no comments)%-18s║\n", "");
    }
    
    printf("║                                                               ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

/* Interactive menu */
int interactive_menu(void) {
    int choice;
    
    printf("╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║                      INTERACTIVE MENU                         ║\n");
    printf("╠═══════════════════════════════════════════════════════════════╣\n");
    printf("║  1. Display Full Spreadsheet View                             ║\n");
    printf("║  2. Display Summary Table                                     ║\n");
    printf("║  3. Display Function Inventory                                ║\n");
    printf("║  4. Display Detailed Metrics                                  ║\n");
    printf("║  5. Exit                                                      ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n");
    printf("\nEnter your choice (1-5): ");
    
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n') {
            /* Clear input buffer */
        }
        return -1;
    }
    
    while (getchar() != '\n') {
        /* Clear remaining input */
    }
    
    return choice;
}
