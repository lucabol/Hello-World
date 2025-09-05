#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1000
#define MAX_METRICS 20

typedef struct {
    char name[50];
    int value;
    char description[100];
} Metric;

typedef struct {
    Metric metrics[MAX_METRICS];
    int count;
    char filename[100];
} CodeAnalysis;

// Function prototypes
void analyze_file(const char* filename, CodeAnalysis* analysis);
void display_spreadsheet(CodeAnalysis* analysis);
void display_interactive_menu(CodeAnalysis* analysis);
int count_lines(FILE* file);
int count_functions(FILE* file);
int count_includes(FILE* file);
int count_comments(FILE* file);
int count_blank_lines(FILE* file);
long get_file_size(const char* filename);

int main(int argc, char* argv[]) {
    CodeAnalysis analysis = {0};
    const char* filename = "hello.c";
    
    if (argc > 1) {
        filename = argv[1];
    }
    
    printf("=== Code Metrics Spreadsheet Interface ===\n");
    printf("Analyzing file: %s\n\n", filename);
    
    analyze_file(filename, &analysis);
    
    if (analysis.count == 0) {
        printf("Error: Could not analyze file %s\n", filename);
        return 1;
    }
    
    display_interactive_menu(&analysis);
    
    return 0;
}

void analyze_file(const char* filename, CodeAnalysis* analysis) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return;
    }
    
    strcpy(analysis->filename, filename);
    
    // Analyze various metrics
    rewind(file);
    int total_lines = count_lines(file);
    
    rewind(file);
    int functions = count_functions(file);
    
    rewind(file);
    int includes = count_includes(file);
    
    rewind(file);
    int comments = count_comments(file);
    
    rewind(file);
    int blank_lines = count_blank_lines(file);
    
    long file_size = get_file_size(filename);
    
    fclose(file);
    
    // Store metrics
    analysis->count = 0;
    
    strcpy(analysis->metrics[analysis->count].name, "Total Lines");
    analysis->metrics[analysis->count].value = total_lines;
    strcpy(analysis->metrics[analysis->count].description, "Total number of lines in file");
    analysis->count++;
    
    strcpy(analysis->metrics[analysis->count].name, "Code Lines");
    analysis->metrics[analysis->count].value = total_lines - blank_lines - comments;
    strcpy(analysis->metrics[analysis->count].description, "Lines containing actual code");
    analysis->count++;
    
    strcpy(analysis->metrics[analysis->count].name, "Blank Lines");
    analysis->metrics[analysis->count].value = blank_lines;
    strcpy(analysis->metrics[analysis->count].description, "Empty or whitespace-only lines");
    analysis->count++;
    
    strcpy(analysis->metrics[analysis->count].name, "Comments");
    analysis->metrics[analysis->count].value = comments;
    strcpy(analysis->metrics[analysis->count].description, "Lines containing comments");
    analysis->count++;
    
    strcpy(analysis->metrics[analysis->count].name, "Functions");
    analysis->metrics[analysis->count].value = functions;
    strcpy(analysis->metrics[analysis->count].description, "Number of function definitions");
    analysis->count++;
    
    strcpy(analysis->metrics[analysis->count].name, "Includes");
    analysis->metrics[analysis->count].value = includes;
    strcpy(analysis->metrics[analysis->count].description, "Number of #include directives");
    analysis->count++;
    
    strcpy(analysis->metrics[analysis->count].name, "File Size");
    analysis->metrics[analysis->count].value = (int)file_size;
    strcpy(analysis->metrics[analysis->count].description, "File size in bytes");
    analysis->count++;
}

void display_spreadsheet(CodeAnalysis* analysis) {
    printf("\n╔══════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                            CODE METRICS SPREADSHEET                         ║\n");
    printf("║                                File: %-35s ║\n", analysis->filename);
    printf("╠══════════════════╤═══════════╤═══════════════════════════════════════════════╣\n");
    printf("║ Metric           │   Value   │ Description                               ║\n");
    printf("╠══════════════════╪═══════════╪═══════════════════════════════════════════════╣\n");
    
    for (int i = 0; i < analysis->count; i++) {
        printf("║ %-16s │ %9d │ %-41s ║\n", 
               analysis->metrics[i].name,
               analysis->metrics[i].value,
               analysis->metrics[i].description);
    }
    
    printf("╚══════════════════╧═══════════╧═══════════════════════════════════════════════╝\n");
}

void display_interactive_menu(CodeAnalysis* analysis) {
    int choice;
    
    while (1) {
        printf("\n=== Interactive Code Metrics Menu ===\n");
        printf("1. Display Full Spreadsheet\n");
        printf("2. Display Summary Statistics\n");
        printf("3. Display Individual Metrics\n");
        printf("4. Export to CSV Format\n");
        printf("5. Exit\n");
        printf("Choose an option (1-5): ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        
        switch (choice) {
            case 1:
                display_spreadsheet(analysis);
                break;
                
            case 2:
                printf("\n=== Summary Statistics ===\n");
                printf("File: %s\n", analysis->filename);
                printf("Total Metrics Collected: %d\n", analysis->count);
                for (int i = 0; i < analysis->count; i++) {
                    printf("  %s: %d\n", analysis->metrics[i].name, analysis->metrics[i].value);
                }
                break;
                
            case 3:
                printf("\n=== Individual Metrics ===\n");
                for (int i = 0; i < analysis->count; i++) {
                    printf("%d. %s\n", i + 1, analysis->metrics[i].name);
                }
                printf("Select metric (1-%d): ", analysis->count);
                int metric_choice;
                if (scanf("%d", &metric_choice) == 1 && metric_choice >= 1 && metric_choice <= analysis->count) {
                    int idx = metric_choice - 1;
                    printf("\nMetric: %s\n", analysis->metrics[idx].name);
                    printf("Value: %d\n", analysis->metrics[idx].value);
                    printf("Description: %s\n", analysis->metrics[idx].description);
                } else {
                    printf("Invalid selection.\n");
                }
                break;
                
            case 4:
                printf("\n=== CSV Export ===\n");
                printf("Metric,Value,Description\n");
                for (int i = 0; i < analysis->count; i++) {
                    printf("%s,%d,\"%s\"\n", 
                           analysis->metrics[i].name,
                           analysis->metrics[i].value,
                           analysis->metrics[i].description);
                }
                break;
                
            case 5:
                printf("Thank you for using Code Metrics Spreadsheet!\n");
                return;
                
            default:
                printf("Invalid choice. Please select 1-5.\n");
                break;
        }
    }
}

int count_lines(FILE* file) {
    int lines = 0;
    int ch;
    
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            lines++;
        }
    }
    
    // Count last line if file doesn't end with newline
    fseek(file, -1, SEEK_END);
    if (ftell(file) > 0 && fgetc(file) != '\n') {
        lines++;
    }
    
    return lines;
}

int count_functions(FILE* file) {
    char line[MAX_LINE_LENGTH];
    int functions = 0;
    
    while (fgets(line, sizeof(line), file)) {
        // Simple function detection: look for pattern "type name("
        char* paren = strchr(line, '(');
        if (paren) {
            // Check if it's likely a function definition (not a call)
            char* ptr = paren - 1;
            while (ptr >= line && isspace(*ptr)) ptr--;
            
            if (ptr >= line) {
                // Look backwards for identifier
                char* end = ptr;
                while (ptr >= line && (isalnum(*ptr) || *ptr == '_')) ptr--;
                
                if (end > ptr) {
                    // Check if there's a type before the identifier
                    while (ptr >= line && isspace(*ptr)) ptr--;
                    
                    if (ptr >= line && (strstr(line, "int ") || strstr(line, "void ") || 
                                      strstr(line, "char ") || strstr(line, "float ") ||
                                      strstr(line, "double "))) {
                        functions++;
                    }
                }
            }
        }
    }
    
    return functions;
}

int count_includes(FILE* file) {
    char line[MAX_LINE_LENGTH];
    int includes = 0;
    
    while (fgets(line, sizeof(line), file)) {
        char* trimmed = line;
        while (isspace(*trimmed)) trimmed++;
        
        if (*trimmed == '#') {
            trimmed++;
            while (isspace(*trimmed)) trimmed++;
            if (strncmp(trimmed, "include", 7) == 0) {
                includes++;
            }
        }
    }
    
    return includes;
}

int count_comments(FILE* file) {
    char line[MAX_LINE_LENGTH];
    int comments = 0;
    
    while (fgets(line, sizeof(line), file)) {
        char* trimmed = line;
        while (isspace(*trimmed)) trimmed++;
        
        if (strncmp(trimmed, "//", 2) == 0 || strncmp(trimmed, "/*", 2) == 0) {
            comments++;
        }
    }
    
    return comments;
}

int count_blank_lines(FILE* file) {
    char line[MAX_LINE_LENGTH];
    int blank_lines = 0;
    
    while (fgets(line, sizeof(line), file)) {
        int is_blank = 1;
        for (int i = 0; line[i] != '\0'; i++) {
            if (!isspace(line[i])) {
                is_blank = 0;
                break;
            }
        }
        if (is_blank) {
            blank_lines++;
        }
    }
    
    return blank_lines;
}

long get_file_size(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    
    return size;
}