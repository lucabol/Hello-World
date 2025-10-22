#include "metrics.h"

/* Analyze a C source file and extract metrics */
CodeMetrics* analyze_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return NULL;
    }

    CodeMetrics* metrics = (CodeMetrics*)malloc(sizeof(CodeMetrics));
    if (!metrics) {
        fclose(file);
        return NULL;
    }

    /* Initialize metrics */
    metrics->total_lines = 0;
    metrics->code_lines = 0;
    metrics->comment_lines = 0;
    metrics->blank_lines = 0;
    metrics->function_count = 0;
    metrics->include_count = 0;
    metrics->function_names = (char**)malloc(10 * sizeof(char*));
    
    if (!metrics->function_names) {
        free(metrics);
        fclose(file);
        return NULL;
    }

    char line[1024];
    int in_multiline_comment = 0;

    while (fgets(line, sizeof(line), file)) {
        metrics->total_lines++;
        
        /* Check for blank lines */
        int is_blank = 1;
        for (int i = 0; line[i]; i++) {
            if (!isspace((unsigned char)line[i])) {
                is_blank = 0;
                break;
            }
        }
        
        if (is_blank) {
            metrics->blank_lines++;
            continue;
        }

        /* Check for comments */
        char* line_ptr = line;
        int is_comment = 0;
        
        /* Skip leading whitespace */
        while (*line_ptr && isspace((unsigned char)*line_ptr)) {
            line_ptr++;
        }

        /* Check for multiline comment start */
        if (!in_multiline_comment && strstr(line_ptr, "/*")) {
            in_multiline_comment = 1;
            is_comment = 1;
        }
        
        /* Check for multiline comment end */
        if (in_multiline_comment && strstr(line_ptr, "*/")) {
            in_multiline_comment = 0;
            is_comment = 1;
        } else if (in_multiline_comment) {
            is_comment = 1;
        }

        /* Check for single-line comment */
        if (!in_multiline_comment && strstr(line_ptr, "//")) {
            is_comment = 1;
        }

        if (is_comment) {
            metrics->comment_lines++;
        } else {
            metrics->code_lines++;
            
            /* Check for include statements */
            if (strstr(line_ptr, "#include") || strstr(line_ptr, "# include")) {
                metrics->include_count++;
            }
            
            /* Simple function detection: look for "int ", "void ", "char " etc followed by identifier and ( */
            if ((strstr(line_ptr, "int ") || strstr(line_ptr, "void ") || 
                 strstr(line_ptr, "char ") || strstr(line_ptr, "float ") ||
                 strstr(line_ptr, "double ")) && strstr(line_ptr, "(")) {
                
                /* Extract function name */
                char* func_start = line_ptr;
                while (*func_start && !isalpha((unsigned char)*func_start)) {
                    func_start++;
                }
                while (*func_start && !isspace((unsigned char)*func_start)) {
                    func_start++;
                }
                while (*func_start && isspace((unsigned char)*func_start)) {
                    func_start++;
                }
                
                char* func_end = func_start;
                while (*func_end && *func_end != '(' && !isspace((unsigned char)*func_end)) {
                    func_end++;
                }
                
                int len = (int)(func_end - func_start);
                if (len > 0 && len < 100 && metrics->function_count < 10) {
                    metrics->function_names[metrics->function_count] = (char*)malloc((size_t)(len + 1));
                    if (metrics->function_names[metrics->function_count]) {
                        strncpy(metrics->function_names[metrics->function_count], func_start, (size_t)len);
                        metrics->function_names[metrics->function_count][len] = '\0';
                        metrics->function_count++;
                    }
                }
            }
        }
    }

    fclose(file);
    return metrics;
}

/* Display metrics in a spreadsheet-like table */
void display_metrics_spreadsheet(CodeMetrics* metrics, const char* filename) {
    printf("\n");
    printf("================================================================================\n");
    printf("                     CODE METRICS SPREADSHEET                                  \n");
    printf("================================================================================\n");
    printf("File: %s\n", filename);
    printf("================================================================================\n");
    printf("\n");
    
    /* Table header */
    printf("+----------------------------------+------------------+---------------------+\n");
    printf("| %-32s | %-16s | %-19s |\n", "METRIC", "VALUE", "PERCENTAGE");
    printf("+----------------------------------+------------------+---------------------+\n");
    
    /* Display metrics rows */
    printf("| %-32s | %-16d | %-19s |\n", "Total Lines", metrics->total_lines, "100%");
    
    if (metrics->total_lines > 0) {
        double code_pct = ((double)metrics->code_lines / metrics->total_lines) * 100.0;
        double comment_pct = ((double)metrics->comment_lines / metrics->total_lines) * 100.0;
        double blank_pct = ((double)metrics->blank_lines / metrics->total_lines) * 100.0;
        
        char code_pct_str[20], comment_pct_str[20], blank_pct_str[20];
        snprintf(code_pct_str, sizeof(code_pct_str), "%.1f%%", code_pct);
        snprintf(comment_pct_str, sizeof(comment_pct_str), "%.1f%%", comment_pct);
        snprintf(blank_pct_str, sizeof(blank_pct_str), "%.1f%%", blank_pct);
        
        printf("| %-32s | %-16d | %-19s |\n", "Code Lines", metrics->code_lines, code_pct_str);
        printf("| %-32s | %-16d | %-19s |\n", "Comment Lines", metrics->comment_lines, comment_pct_str);
        printf("| %-32s | %-16d | %-19s |\n", "Blank Lines", metrics->blank_lines, blank_pct_str);
    } else {
        printf("| %-32s | %-16d | %-19s |\n", "Code Lines", metrics->code_lines, "N/A");
        printf("| %-32s | %-16d | %-19s |\n", "Comment Lines", metrics->comment_lines, "N/A");
        printf("| %-32s | %-16d | %-19s |\n", "Blank Lines", metrics->blank_lines, "N/A");
    }
    
    printf("+----------------------------------+------------------+---------------------+\n");
    printf("| %-32s | %-16d | %-19s |\n", "Function Count", metrics->function_count, "N/A");
    printf("| %-32s | %-16d | %-19s |\n", "Include Statements", metrics->include_count, "N/A");
    printf("+----------------------------------+------------------+---------------------+\n");
    
    /* Display function names if any */
    if (metrics->function_count > 0) {
        printf("\n");
        printf("Functions Detected:\n");
        printf("+----------------------------------+\n");
        for (int i = 0; i < metrics->function_count; i++) {
            printf("| %-32s |\n", metrics->function_names[i]);
        }
        printf("+----------------------------------+\n");
    }
    
    printf("\n");
}

/* Free allocated memory for metrics */
void free_metrics(CodeMetrics* metrics) {
    if (metrics) {
        if (metrics->function_names) {
            for (int i = 0; i < metrics->function_count; i++) {
                free(metrics->function_names[i]);
            }
            free(metrics->function_names);
        }
        free(metrics);
    }
}

/* Interactive menu for viewing different metrics */
void interactive_menu(CodeMetrics* metrics, const char* filename) {
    int choice = 0;
    
    while (choice != 5) {
        printf("\n");
        printf("================================================================================\n");
        printf("                     INTERACTIVE METRICS VIEWER                                \n");
        printf("================================================================================\n");
        printf("File: %s\n", filename);
        printf("================================================================================\n");
        printf("\n");
        printf("1. View Full Spreadsheet\n");
        printf("2. View Line Metrics\n");
        printf("3. View Function Metrics\n");
        printf("4. View Summary Statistics\n");
        printf("5. Exit\n");
        printf("\n");
        printf("Enter your choice (1-5): ");
        
        if (scanf("%d", &choice) != 1) {
            /* Clear input buffer */
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        switch (choice) {
            case 1:
                display_metrics_spreadsheet(metrics, filename);
                printf("\nPress Enter to continue...");
                getchar(); /* Consume newline */
                getchar(); /* Wait for Enter */
                break;
                
            case 2:
                printf("\n");
                printf("+----------------------------------+------------------+\n");
                printf("| %-32s | %-16s |\n", "LINE METRIC", "VALUE");
                printf("+----------------------------------+------------------+\n");
                printf("| %-32s | %-16d |\n", "Total Lines", metrics->total_lines);
                printf("| %-32s | %-16d |\n", "Code Lines", metrics->code_lines);
                printf("| %-32s | %-16d |\n", "Comment Lines", metrics->comment_lines);
                printf("| %-32s | %-16d |\n", "Blank Lines", metrics->blank_lines);
                printf("+----------------------------------+------------------+\n");
                printf("\nPress Enter to continue...");
                getchar();
                getchar();
                break;
                
            case 3:
                printf("\n");
                printf("+----------------------------------+------------------+\n");
                printf("| %-32s | %-16s |\n", "FUNCTION METRIC", "VALUE");
                printf("+----------------------------------+------------------+\n");
                printf("| %-32s | %-16d |\n", "Function Count", metrics->function_count);
                printf("+----------------------------------+------------------+\n");
                
                if (metrics->function_count > 0) {
                    printf("\nFunction Names:\n");
                    for (int i = 0; i < metrics->function_count; i++) {
                        printf("  %d. %s\n", i + 1, metrics->function_names[i]);
                    }
                }
                
                printf("\nPress Enter to continue...");
                getchar();
                getchar();
                break;
                
            case 4:
                printf("\n");
                printf("SUMMARY STATISTICS:\n");
                printf("==================\n");
                if (metrics->total_lines > 0) {
                    double code_ratio = ((double)metrics->code_lines / metrics->total_lines) * 100.0;
                    double comment_ratio = ((double)metrics->comment_lines / metrics->total_lines) * 100.0;
                    
                    printf("Code Density:    %.1f%% (code lines / total lines)\n", code_ratio);
                    printf("Comment Density: %.1f%% (comment lines / total lines)\n", comment_ratio);
                }
                
                if (metrics->function_count > 0) {
                    double avg_lines = (double)metrics->code_lines / metrics->function_count;
                    printf("Average Lines per Function: %.1f\n", avg_lines);
                }
                
                printf("\nPress Enter to continue...");
                getchar();
                getchar();
                break;
                
            case 5:
                printf("\nExiting...\n");
                break;
                
            default:
                printf("\nInvalid choice. Please enter a number between 1 and 5.\n");
                printf("Press Enter to continue...");
                getchar();
                getchar();
                break;
        }
    }
}
