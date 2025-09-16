#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char filename[256];
    int total_lines;
    int code_lines;
    int blank_lines;
    int comment_lines;
    int include_count;
    int function_count;
    int printf_count;
    int brace_count;
} CodeMetrics;

void analyze_file(const char* filename, CodeMetrics* metrics) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }
    
    strncpy(metrics->filename, filename, sizeof(metrics->filename) - 1);
    metrics->filename[sizeof(metrics->filename) - 1] = '\0';
    
    char line[1024];
    int in_multiline_comment = 0;
    
    while (fgets(line, sizeof(line), file)) {
        metrics->total_lines++;
        
        // Trim whitespace
        char* trimmed = line;
        while (isspace(*trimmed)) trimmed++;
        
        // Check for blank line
        if (strlen(trimmed) == 0) {
            metrics->blank_lines++;
            continue;
        }
        
        // Check for single line comments
        if (strstr(trimmed, "//") == trimmed) {
            metrics->comment_lines++;
            continue;
        }
        
        // Check for multiline comments
        if (strstr(trimmed, "/*")) {
            in_multiline_comment = 1;
            metrics->comment_lines++;
            if (strstr(trimmed, "*/")) {
                in_multiline_comment = 0;
            }
            continue;
        }
        
        if (in_multiline_comment) {
            metrics->comment_lines++;
            if (strstr(trimmed, "*/")) {
                in_multiline_comment = 0;
            }
            continue;
        }
        
        // This is a code line
        metrics->code_lines++;
        
        // Count specific patterns
        if (strstr(line, "#include") || strstr(line, "# include")) {
            metrics->include_count++;
        }
        
        if (strstr(line, "int main") || strstr(line, "void ") || strstr(line, "char ") || 
            strstr(line, "float ") || strstr(line, "double ")) {
            if (strstr(line, "(") && strstr(line, ")")) {
                metrics->function_count++;
            }
        }
        
        if (strstr(line, "printf")) {
            metrics->printf_count++;
        }
        
        // Count braces
        for (char* p = line; *p; p++) {
            if (*p == '{' || *p == '}') {
                metrics->brace_count++;
            }
        }
    }
    
    fclose(file);
}

void print_csv_header() {
    printf("Filename,Total Lines,Code Lines,Blank Lines,Comment Lines,Include Count,Function Count,Printf Count,Brace Count\n");
}

void print_csv_metrics(const CodeMetrics* metrics) {
    printf("%s,%d,%d,%d,%d,%d,%d,%d,%d\n",
           metrics->filename,
           metrics->total_lines,
           metrics->code_lines,
           metrics->blank_lines,
           metrics->comment_lines,
           metrics->include_count,
           metrics->function_count,
           metrics->printf_count,
           metrics->brace_count);
}

void print_table_header() {
    printf("%-20s %-12s %-12s %-12s %-14s %-14s %-14s %-13s %-12s\n",
           "Filename", "Total Lines", "Code Lines", "Blank Lines", "Comment Lines", 
           "Include Count", "Function Count", "Printf Count", "Brace Count");
    printf("%-20s %-12s %-12s %-12s %-14s %-14s %-14s %-13s %-12s\n",
           "--------------------", "------------", "------------", "------------", 
           "--------------", "--------------", "--------------", "-------------", "------------");
}

void print_table_metrics(const CodeMetrics* metrics) {
    printf("%-20s %-12d %-12d %-12d %-14d %-14d %-14d %-13d %-12d\n",
           metrics->filename,
           metrics->total_lines,
           metrics->code_lines,
           metrics->blank_lines,
           metrics->comment_lines,
           metrics->include_count,
           metrics->function_count,
           metrics->printf_count,
           metrics->brace_count);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s [--csv] <filename1> [filename2] ...\n", argv[0]);
        printf("       --csv: Output in CSV format for spreadsheet import\n");
        return 1;
    }
    
    int csv_mode = 0;
    int start_index = 1;
    
    if (argc > 1 && strcmp(argv[1], "--csv") == 0) {
        csv_mode = 1;
        start_index = 2;
        if (argc < 3) {
            printf("Error: No files specified after --csv flag\n");
            return 1;
        }
    }
    
    if (csv_mode) {
        print_csv_header();
    } else {
        print_table_header();
    }
    
    for (int i = start_index; i < argc; i++) {
        CodeMetrics metrics = {0};
        analyze_file(argv[i], &metrics);
        
        if (csv_mode) {
            print_csv_metrics(&metrics);
        } else {
            print_table_metrics(&metrics);
        }
    }
    
    return 0;
}