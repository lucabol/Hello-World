#include "code_metrics.h"
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* Analyze a C source file and collect metrics */
int analyze_file(const char* filename, CodeMetrics* metrics) {
    FILE* file;
    char line[1024];
    bool in_multiline_comment = false;
    
    /* Initialize metrics */
    strncpy(metrics->filename, filename, sizeof(metrics->filename) - 1);
    metrics->filename[sizeof(metrics->filename) - 1] = '\0';
    metrics->total_lines = 0;
    metrics->code_lines = 0;
    metrics->blank_lines = 0;
    metrics->comment_lines = 0;
    metrics->functions = 0;
    metrics->includes = 0;
    metrics->max_line_length = 0;
    
    file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return -1;
    }
    
    while (fgets(line, sizeof(line), file)) {
        size_t len = strlen(line);
        int is_blank = 1;
        int is_comment = 0;
        size_t i;
        
        metrics->total_lines++;
        
        /* Track max line length (excluding newline) */
        if (len > 0 && line[len - 1] == '\n') {
            len--;
        }
        if ((int)len > metrics->max_line_length) {
            metrics->max_line_length = (int)len;
        }
        
        /* Check for multiline comment end */
        if (in_multiline_comment) {
            metrics->comment_lines++;
            if (strstr(line, "*/")) {
                in_multiline_comment = false;
            }
            continue;
        }
        
        /* Analyze line content */
        for (i = 0; i < len; i++) {
            if (!isspace((unsigned char)line[i])) {
                is_blank = 0;
                break;
            }
        }
        
        if (is_blank) {
            metrics->blank_lines++;
            continue;
        }
        
        /* Check for single-line comment */
        if (strstr(line, "//")) {
            metrics->comment_lines++;
            is_comment = 1;
        }
        
        /* Check for multiline comment start */
        if (strstr(line, "/*")) {
            metrics->comment_lines++;
            is_comment = 1;
            if (!strstr(line, "*/")) {
                in_multiline_comment = true;
            }
        }
        
        /* Count includes */
        if (strstr(line, "#include") || strstr(line, "# include")) {
            metrics->includes++;
        }
        
        /* Count functions (simple heuristic: lines with '(' followed by ')' and '{') */
        if (!is_comment && strstr(line, "(") && strstr(line, ")")) {
            /* Check if it looks like a function definition */
            if (strstr(line, "{") || (fgets(line, sizeof(line), file) && strstr(line, "{"))) {
                /* Found potential function definition */
                char* type_keywords[] = {"int", "void", "char", "float", "double", "long", "short"};
                int j;
                for (j = 0; j < 7; j++) {
                    if (strstr(line, type_keywords[j])) {
                        metrics->functions++;
                        break;
                    }
                }
            }
        }
        
        /* If not a comment or blank line, it's a code line */
        if (!is_comment && !is_blank) {
            metrics->code_lines++;
        }
    }
    
    fclose(file);
    return 0;
}

/* Display metrics in a spreadsheet-like table format */
void display_metrics_table(CodeMetrics* metrics, int count) {
    int i;
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                         CODE METRICS SPREADSHEET                               ║\n");
    printf("╠═══════════════╦═════════╦═════════╦═════════╦═════════╦═════════╦═════════════╣\n");
    printf("║ Filename      ║ Total   ║ Code    ║ Blank   ║ Comment ║ Funcs   ║ Max Line    ║\n");
    printf("║               ║ Lines   ║ Lines   ║ Lines   ║ Lines   ║         ║ Length      ║\n");
    printf("╠═══════════════╬═════════╬═════════╬═════════╬═════════╬═════════╬═════════════╣\n");
    
    for (i = 0; i < count; i++) {
        printf("║ %-13s ║ %7d ║ %7d ║ %7d ║ %7d ║ %7d ║ %11d ║\n",
               metrics[i].filename,
               metrics[i].total_lines,
               metrics[i].code_lines,
               metrics[i].blank_lines,
               metrics[i].comment_lines,
               metrics[i].functions,
               metrics[i].max_line_length);
    }
    
    printf("╚═══════════════╩═════════╩═════════╩═════════╩═════════╩═════════╩═════════════╝\n");
    printf("\n");
    
    /* Display includes information */
    printf("Additional Metrics:\n");
    for (i = 0; i < count; i++) {
        printf("  %s: %d #include directive(s)\n", metrics[i].filename, metrics[i].includes);
    }
    printf("\n");
}

/* Display interactive menu */
void display_menu(void) {
    printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                     INTERACTIVE CODE METRICS TOOL                              ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║ Commands:                                                                      ║\n");
    printf("║   r - Refresh metrics                                                          ║\n");
    printf("║   s - Show detailed statistics                                                 ║\n");
    printf("║   h - Display this help menu                                                   ║\n");
    printf("║   q - Quit                                                                     ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
}
