#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int total_lines;
    int code_lines;
    int blank_lines;
    int comment_lines;
    int include_count;
    int function_count;
    long file_size;
    int char_count;
    int keyword_count;
    int brace_count;
} CodeMetrics;

// Common C keywords to count
const char* c_keywords[] = {
    "int", "char", "float", "double", "void", "if", "else", "while", 
    "for", "do", "switch", "case", "break", "continue", "return",
    "struct", "union", "enum", "typedef", "static", "extern", "const",
    "volatile", "sizeof", "auto", "register", "signed", "unsigned",
    "long", "short", "include", "define", "ifdef", "ifndef", "endif",
    "printf", "scanf", "main"
};
const int keyword_count = sizeof(c_keywords) / sizeof(c_keywords[0]);

int is_keyword(const char* word) {
    for (int i = 0; i < keyword_count; i++) {
        if (strcmp(word, c_keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void analyze_file(const char* filename, CodeMetrics* metrics) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return;
    }
    
    // Initialize metrics
    memset(metrics, 0, sizeof(CodeMetrics));
    
    // Get file size
    fseek(file, 0, SEEK_END);
    metrics->file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char line[1024];
    int in_multiline_comment = 0;
    
    while (fgets(line, sizeof(line), file)) {
        metrics->total_lines++;
        metrics->char_count += strlen(line);
        
        // Count braces
        for (char* p = line; *p; p++) {
            if (*p == '{' || *p == '}') {
                metrics->brace_count++;
            }
        }
        
        // Trim whitespace for analysis
        char* trimmed = line;
        while (isspace(*trimmed)) trimmed++;
        
        // Check for blank lines
        if (strlen(trimmed) == 0 || trimmed[0] == '\n') {
            metrics->blank_lines++;
            continue;
        }
        
        // Check for comments
        if (strstr(trimmed, "/*") || in_multiline_comment) {
            metrics->comment_lines++;
            if (strstr(trimmed, "*/")) {
                in_multiline_comment = 0;
            } else if (strstr(trimmed, "/*")) {
                in_multiline_comment = 1;
            }
            continue;
        }
        
        if (trimmed[0] == '/' && trimmed[1] == '/') {
            metrics->comment_lines++;
            continue;
        }
        
        // Count includes
        if (strstr(trimmed, "#include") || strstr(trimmed, "include")) {
            metrics->include_count++;
        }
        
        // Count functions (simple heuristic - lines with function signatures)
        if (strstr(trimmed, "()") || (strstr(trimmed, "(") && strstr(trimmed, ")") && strstr(trimmed, "{"))) {
            metrics->function_count++;
        }
        
        // Count keywords
        char line_copy[1024];
        strcpy(line_copy, line);
        char* token = strtok(line_copy, " \t\n\r(){}[];,");
        while (token) {
            if (is_keyword(token)) {
                metrics->keyword_count++;
            }
            token = strtok(NULL, " \t\n\r(){}[];,");
        }
        
        metrics->code_lines++;
    }
    
    fclose(file);
}

void print_metrics_table(const char* filename, CodeMetrics* metrics) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                     CODE METRICS ANALYZER                   ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║ File: %-51s ║\n", filename);
    printf("╠═══════════════════════════════════╤══════════════════════════╣\n");
    printf("║ Metric                            │ Value                    ║\n");
    printf("╠═══════════════════════════════════╪══════════════════════════╣\n");
    printf("║ Total Lines                       │ %-24d ║\n", metrics->total_lines);
    printf("║ Code Lines                        │ %-24d ║\n", metrics->code_lines);
    printf("║ Blank Lines                       │ %-24d ║\n", metrics->blank_lines);
    printf("║ Comment Lines                     │ %-24d ║\n", metrics->comment_lines);
    printf("║ Include Statements               │ %-24d ║\n", metrics->include_count);
    printf("║ Functions                         │ %-24d ║\n", metrics->function_count);
    printf("║ File Size (bytes)                 │ %-24ld ║\n", metrics->file_size);
    printf("║ Character Count                   │ %-24d ║\n", metrics->char_count);
    printf("║ Keyword Occurrences              │ %-24d ║\n", metrics->keyword_count);
    printf("║ Brace Count                       │ %-24d ║\n", metrics->brace_count);
    printf("╠═══════════════════════════════════╪══════════════════════════╣\n");
    
    // Calculate percentages
    double code_percentage = metrics->total_lines > 0 ? 
        (double)metrics->code_lines / metrics->total_lines * 100.0 : 0.0;
    double comment_percentage = metrics->total_lines > 0 ? 
        (double)metrics->comment_lines / metrics->total_lines * 100.0 : 0.0;
    
    printf("║ Code Coverage                     │ %-22.1f%% ║\n", code_percentage);
    printf("║ Comment Coverage                  │ %-22.1f%% ║\n", comment_percentage);
    printf("╚═══════════════════════════════════╧══════════════════════════╝\n");
    printf("\n");
}

int main(int argc, char* argv[]) {
    const char* filename = "hello.c"; // Default to hello.c
    
    if (argc > 1) {
        filename = argv[1];
    }
    
    CodeMetrics metrics;
    analyze_file(filename, &metrics);
    print_metrics_table(filename, &metrics);
    
    return 0;
}