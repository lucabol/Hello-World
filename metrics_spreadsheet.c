#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_METRICS 20

typedef struct {
    char name[50];
    int value;
} Metric;

typedef struct {
    Metric metrics[MAX_METRICS];
    int count;
} MetricsData;

void print_separator(int width) {
    for (int i = 0; i < width; i++) {
        printf("-");
    }
    printf("\n");
}

void print_header() {
    printf("\n");
    print_separator(60);
    printf("| %-40s | %-12s |\n", "Metric", "Value");
    print_separator(60);
}

void print_metric(const char* name, int value) {
    printf("| %-40s | %12d |\n", name, value);
}

void print_footer() {
    print_separator(60);
    printf("\n");
}

int analyze_file(const char* filename, MetricsData* data) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    int total_lines = 0;
    int code_lines = 0;
    int blank_lines = 0;
    int comment_lines = 0;
    int include_count = 0;
    int function_count = 0;
    int printf_count = 0;
    int total_chars = 0;
    int brace_open = 0;
    int brace_close = 0;
    int semicolons = 0;

    while (fgets(line, sizeof(line), file)) {
        total_lines++;
        total_chars += strlen(line);
        
        int is_blank = 1;
        
        for (int i = 0; line[i]; i++) {
            if (!isspace(line[i])) {
                is_blank = 0;
            }
            if (line[i] == '{') brace_open++;
            if (line[i] == '}') brace_close++;
            if (line[i] == ';') semicolons++;
        }
        
        char trimmed[MAX_LINE_LENGTH];
        int j = 0;
        for (int i = 0; line[i] && j < MAX_LINE_LENGTH - 1; i++) {
            if (!isspace(line[i])) {
                trimmed[j++] = line[i];
            }
        }
        trimmed[j] = '\0';
        
        if (is_blank) {
            blank_lines++;
        } else if (strstr(line, "//") == line || strstr(trimmed, "//") == trimmed) {
            comment_lines++;
        } else {
            code_lines++;
            
            if (strstr(line, "#include")) {
                include_count++;
            }
            if (strstr(line, "int main") || strstr(line, "void ") || 
                strstr(line, "char ") || strstr(line, "float ")) {
                if (strchr(line, '(')) {
                    function_count++;
                }
            }
            if (strstr(line, "printf")) {
                printf_count++;
            }
        }
    }

    fclose(file);

    data->count = 0;
    
    strcpy(data->metrics[data->count].name, "Total Lines");
    data->metrics[data->count++].value = total_lines;
    
    strcpy(data->metrics[data->count].name, "Code Lines");
    data->metrics[data->count++].value = code_lines;
    
    strcpy(data->metrics[data->count].name, "Blank Lines");
    data->metrics[data->count++].value = blank_lines;
    
    strcpy(data->metrics[data->count].name, "Comment Lines");
    data->metrics[data->count++].value = comment_lines;
    
    strcpy(data->metrics[data->count].name, "Include Statements");
    data->metrics[data->count++].value = include_count;
    
    strcpy(data->metrics[data->count].name, "Functions");
    data->metrics[data->count++].value = function_count;
    
    strcpy(data->metrics[data->count].name, "Printf Calls");
    data->metrics[data->count++].value = printf_count;
    
    strcpy(data->metrics[data->count].name, "Total Characters");
    data->metrics[data->count++].value = total_chars;
    
    strcpy(data->metrics[data->count].name, "Opening Braces");
    data->metrics[data->count++].value = brace_open;
    
    strcpy(data->metrics[data->count].name, "Closing Braces");
    data->metrics[data->count++].value = brace_close;
    
    strcpy(data->metrics[data->count].name, "Semicolons");
    data->metrics[data->count++].value = semicolons;

    return 0;
}

void display_spreadsheet(MetricsData* data, const char* filename) {
    printf("\n=== Code Metrics Spreadsheet ===\n");
    printf("File: %s\n", filename);
    
    print_header();
    
    for (int i = 0; i < data->count; i++) {
        print_metric(data->metrics[i].name, data->metrics[i].value);
    }
    
    print_footer();
    
    printf("Interactive Commands:\n");
    printf("  - Press 's' to sort by value\n");
    printf("  - Press 'n' to sort by name\n");
    printf("  - Press 'q' to quit\n");
    printf("\nCommand: ");
}

int compare_by_value(const void* a, const void* b) {
    return ((Metric*)b)->value - ((Metric*)a)->value;
}

int compare_by_name(const void* a, const void* b) {
    return strcmp(((Metric*)a)->name, ((Metric*)b)->name);
}

void interactive_mode(MetricsData* data, const char* filename) {
    char command;
    
    display_spreadsheet(data, filename);
    
    while (scanf(" %c", &command) == 1) {
        if (command == 'q' || command == 'Q') {
            printf("\nExiting spreadsheet.\n");
            break;
        } else if (command == 's' || command == 'S') {
            qsort(data->metrics, data->count, sizeof(Metric), compare_by_value);
            display_spreadsheet(data, filename);
        } else if (command == 'n' || command == 'N') {
            qsort(data->metrics, data->count, sizeof(Metric), compare_by_name);
            display_spreadsheet(data, filename);
        } else {
            printf("Unknown command. Try 's', 'n', or 'q': ");
        }
    }
}

int main(int argc, char* argv[]) {
    const char* filename = "hello.c";
    
    if (argc > 1) {
        filename = argv[1];
    }
    
    MetricsData data;
    
    if (analyze_file(filename, &data) != 0) {
        return 1;
    }
    
    interactive_mode(&data, filename);
    
    return 0;
}
