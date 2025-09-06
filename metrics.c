#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_FILENAME 256

// Structure to hold code metrics
typedef struct {
    int total_lines;
    int blank_lines;
    int comment_lines;
    int code_lines;
    int total_chars;
    int functions;
    int includes;
    int variables;
    long file_size;
} CodeMetrics;

// Function prototypes
void analyze_file(const char* filename, CodeMetrics* metrics);
void display_metrics_table(const CodeMetrics* metrics, const char* filename);
void display_menu();
int get_user_choice();
void clear_screen();

int main() {
    CodeMetrics metrics = {0};
    char filename[MAX_FILENAME] = "hello.c";
    int choice;
    
    printf("=== Code Metrics Spreadsheet Tool ===\n\n");
    
    // Analyze the hello.c file
    analyze_file(filename, &metrics);
    
    do {
        display_menu();
        choice = get_user_choice();
        
        switch(choice) {
            case 1:
                clear_screen();
                display_metrics_table(&metrics, filename);
                break;
            case 2:
                printf("\nEnter filename to analyze: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0; // Remove newline
                memset(&metrics, 0, sizeof(metrics)); // Reset metrics
                analyze_file(filename, &metrics);
                printf("Analysis complete for: %s\n", filename);
                break;
            case 3:
                printf("\nExiting Code Metrics Tool. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
        
        if (choice != 3) {
            printf("\nPress Enter to continue...");
            getchar();
        }
        
    } while (choice != 3);
    
    return 0;
}

void analyze_file(const char* filename, CodeMetrics* metrics) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file '%s'\n", filename);
        return;
    }
    
    char line[MAX_LINE_LENGTH];
    int in_multiline_comment = 0;
    
    // Get file size
    fseek(file, 0, SEEK_END);
    metrics->file_size = ftell(file);
    rewind(file);
    
    while (fgets(line, sizeof(line), file)) {
        metrics->total_lines++;
        metrics->total_chars += strlen(line);
        
        // Remove trailing whitespace for analysis
        char* trimmed = line;
        while (isspace(*trimmed)) trimmed++;
        
        // Check for blank line
        if (strlen(trimmed) == 0) {
            metrics->blank_lines++;
            continue;
        }
        
        // Check for includes (handle spaces after #)
        if (strstr(trimmed, "#include") == trimmed || 
            strstr(trimmed, "# include") == trimmed) {
            metrics->includes++;
        }
        
        // Check for function definitions (simple heuristic)
        if (strstr(trimmed, "int main") || 
            (strstr(trimmed, "(") && strstr(trimmed, ")") && strstr(trimmed, "{"))) {
            metrics->functions++;
        }
        
        // Check for variable declarations (simple heuristic)
        if ((strstr(trimmed, "int ") || strstr(trimmed, "char ") || 
             strstr(trimmed, "float ") || strstr(trimmed, "double ")) &&
            !strstr(trimmed, "#include") && !strstr(trimmed, "printf")) {
            metrics->variables++;
        }
        
        // Check for comments
        if (strstr(trimmed, "//") || strstr(trimmed, "/*") || in_multiline_comment) {
            metrics->comment_lines++;
            if (strstr(trimmed, "/*") && !strstr(trimmed, "*/")) {
                in_multiline_comment = 1;
            }
            if (strstr(trimmed, "*/")) {
                in_multiline_comment = 0;
            }
        } else {
            metrics->code_lines++;
        }
    }
    
    fclose(file);
}

void display_metrics_table(const CodeMetrics* metrics, const char* filename) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════╗\n");
    printf("║                      CODE METRICS REPORT                      ║\n");
    printf("╠════════════════════════════════════════════════════════════════╣\n");
    printf("║ File: %-56s ║\n", filename);
    printf("╠═══════════════════════════════════╤════════════════════════════╣\n");
    printf("║ METRIC                            │ VALUE                      ║\n");
    printf("╠═══════════════════════════════════╪════════════════════════════╣\n");
    printf("║ Total Lines                       │ %26d ║\n", metrics->total_lines);
    printf("║ Code Lines                        │ %26d ║\n", metrics->code_lines);
    printf("║ Blank Lines                       │ %26d ║\n", metrics->blank_lines);
    printf("║ Comment Lines                     │ %26d ║\n", metrics->comment_lines);
    printf("╠═══════════════════════════════════╪════════════════════════════╣\n");
    printf("║ Total Characters                  │ %26d ║\n", metrics->total_chars);
    printf("║ File Size (bytes)                 │ %26ld ║\n", metrics->file_size);
    printf("╠═══════════════════════════════════╪════════════════════════════╣\n");
    printf("║ Number of Functions               │ %26d ║\n", metrics->functions);
    printf("║ Number of Variables               │ %26d ║\n", metrics->variables);
    printf("║ Number of Includes                │ %26d ║\n", metrics->includes);
    printf("╚═══════════════════════════════════╧════════════════════════════╝\n");
    
    // Additional calculated metrics
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════╗\n");
    printf("║                    CALCULATED METRICS                         ║\n");
    printf("╠═══════════════════════════════════╤════════════════════════════╣\n");
    
    if (metrics->total_lines > 0) {
        float code_percentage = (float)metrics->code_lines / metrics->total_lines * 100;
        float comment_percentage = (float)metrics->comment_lines / metrics->total_lines * 100;
        float blank_percentage = (float)metrics->blank_lines / metrics->total_lines * 100;
        
        printf("║ Code Coverage (%%)                 │ %25.1f%% ║\n", code_percentage);
        printf("║ Comment Coverage (%%)              │ %25.1f%% ║\n", comment_percentage);
        printf("║ Blank Line Coverage (%%)           │ %25.1f%% ║\n", blank_percentage);
    }
    
    if (metrics->code_lines > 0) {
        float avg_chars_per_line = (float)metrics->total_chars / metrics->total_lines;
        printf("║ Average Characters per Line       │ %25.1f ║\n", avg_chars_per_line);
    }
    
    printf("╚═══════════════════════════════════╧════════════════════════════╝\n");
}

void display_menu() {
    clear_screen();
    printf("╔════════════════════════════════════════════════════════════════╗\n");
    printf("║                  CODE METRICS SPREADSHEET TOOL                ║\n");
    printf("╠════════════════════════════════════════════════════════════════╣\n");
    printf("║                                                                ║\n");
    printf("║  1. Display Code Metrics Table                                ║\n");
    printf("║  2. Analyze Different File                                     ║\n");
    printf("║  3. Exit                                                       ║\n");
    printf("║                                                                ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    printf("\nSelect an option (1-3): ");
}

int get_user_choice() {
    int choice;
    if (scanf("%d", &choice) != 1) {
        // Clear input buffer
        while (getchar() != '\n');
        return -1;
    }
    // Clear remaining input
    while (getchar() != '\n');
    return choice;
}

void clear_screen() {
    printf("\033[2J\033[H"); // ANSI escape codes to clear screen
}