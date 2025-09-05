#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char metric_name[30];
    int value;
    char description[60];
} CodeMetric;

void print_header() {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                        CODE METRICS SPREADSHEET                             ║\n");
    printf("║                          Analysis of hello.c                                ║\n");
    printf("╚══════════════════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

void print_table_header() {
    printf("┌──────────────────────────────┬───────────┬────────────────────────────────────────┐\n");
    printf("│ METRIC                       │   VALUE   │ DESCRIPTION                            │\n");
    printf("├──────────────────────────────┼───────────┼────────────────────────────────────────┤\n");
}

void print_metric_row(CodeMetric metric) {
    printf("│ %-28s │ %9d │ %-38s │\n", metric.metric_name, metric.value, metric.description);
}

void print_table_footer() {
    printf("└──────────────────────────────┴───────────┴────────────────────────────────────────┘\n");
}

int analyze_file(const char* filename, CodeMetric metrics[]) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return -1;
    }

    int lines = 0;
    int characters = 0;
    int words = 0;
    int includes = 0;
    int functions = 0;
    int printf_calls = 0;
    int braces = 0;
    int semicolons = 0;
    int blank_lines = 0;
    
    char line[256];
    int in_word = 0;
    
    while (fgets(line, sizeof(line), file)) {
        lines++;
        
        // Check if line is blank
        int is_blank = 1;
        for (int i = 0; line[i]; i++) {
            if (!isspace(line[i])) {
                is_blank = 0;
                break;
            }
        }
        if (is_blank) blank_lines++;
        
        // Count characters
        characters += strlen(line);
        
        // Count words
        for (int i = 0; line[i]; i++) {
            if (isspace(line[i])) {
                in_word = 0;
            } else if (!in_word) {
                in_word = 1;
                words++;
            }
        }
        
        // Count includes
        if (strstr(line, "#include") || strstr(line, "# include")) {
            includes++;
        }
        
        // Count function definitions (simple heuristic)
        if (strstr(line, "int main") || strstr(line, "void ") || 
            strstr(line, "char ") || strstr(line, "float ") || 
            strstr(line, "double ")) {
            if (strchr(line, '(') && strchr(line, ')')) {
                functions++;
            }
        }
        
        // Count printf calls
        if (strstr(line, "printf")) {
            printf_calls++;
        }
        
        // Count braces and semicolons
        for (int i = 0; line[i]; i++) {
            if (line[i] == '{' || line[i] == '}') braces++;
            if (line[i] == ';') semicolons++;
        }
    }
    
    fclose(file);
    
    // Populate metrics array
    strcpy(metrics[0].metric_name, "Total Lines");
    metrics[0].value = lines;
    strcpy(metrics[0].description, "Total number of lines in the file");
    
    strcpy(metrics[1].metric_name, "Code Lines");
    metrics[1].value = lines - blank_lines;
    strcpy(metrics[1].description, "Non-blank lines of code");
    
    strcpy(metrics[2].metric_name, "Blank Lines");
    metrics[2].value = blank_lines;
    strcpy(metrics[2].description, "Empty or whitespace-only lines");
    
    strcpy(metrics[3].metric_name, "Characters");
    metrics[3].value = characters;
    strcpy(metrics[3].description, "Total character count including spaces");
    
    strcpy(metrics[4].metric_name, "Words");
    metrics[4].value = words;
    strcpy(metrics[4].description, "Total word count");
    
    strcpy(metrics[5].metric_name, "Include Statements");
    metrics[5].value = includes;
    strcpy(metrics[5].description, "Number of #include directives");
    
    strcpy(metrics[6].metric_name, "Functions");
    metrics[6].value = functions;
    strcpy(metrics[6].description, "Number of function definitions");
    
    strcpy(metrics[7].metric_name, "Printf Calls");
    metrics[7].value = printf_calls;
    strcpy(metrics[7].description, "Number of printf statements");
    
    strcpy(metrics[8].metric_name, "Braces");
    metrics[8].value = braces;
    strcpy(metrics[8].description, "Total number of { and } characters");
    
    strcpy(metrics[9].metric_name, "Semicolons");
    metrics[9].value = semicolons;
    strcpy(metrics[9].description, "Number of semicolon terminators");
    
    return 10; // Number of metrics
}

void display_metrics_subset(CodeMetric metrics[], int start, int end) {
    print_table_header();
    for (int i = start; i <= end && i < 10; i++) {
        print_metric_row(metrics[i]);
    }
    print_table_footer();
}

void print_usage() {
    printf("Usage: ./code_metrics [option]\n");
    printf("Options:\n");
    printf("  (no option)   Show all metrics\n");
    printf("  --basic       Show basic metrics (lines, characters, words)\n");
    printf("  --structural  Show structural metrics (functions, includes, braces)\n");
    printf("  --help        Show this help message\n");
}

int main(int argc, char* argv[]) {
    CodeMetric metrics[10];
    int metric_count;
    
    // Handle help
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        print_usage();
        return 0;
    }
    
    print_header();
    
    // Analyze hello.c
    metric_count = analyze_file("hello.c", metrics);
    if (metric_count < 0) {
        return 1;
    }
    
    // Default behavior: show all metrics
    if (argc == 1) {
        printf("Complete Code Metrics Report:\n");
        display_metrics_subset(metrics, 0, 9);
    }
    // Basic metrics
    else if (argc > 1 && strcmp(argv[1], "--basic") == 0) {
        printf("Basic Metrics (Lines, Characters, Words):\n");
        display_metrics_subset(metrics, 0, 4);
    }
    // Structural metrics
    else if (argc > 1 && strcmp(argv[1], "--structural") == 0) {
        printf("Structural Metrics (Functions, Includes, Syntax Elements):\n");
        print_table_header();
        print_metric_row(metrics[5]); // Includes
        print_metric_row(metrics[6]); // Functions
        print_metric_row(metrics[7]); // Printf calls
        print_metric_row(metrics[8]); // Braces
        print_metric_row(metrics[9]); // Semicolons
        print_table_footer();
    }
    else {
        printf("Unknown option: %s\n", argv[1]);
        print_usage();
        return 1;
    }
    
    printf("\nAnalysis complete!\n");
    return 0;
}