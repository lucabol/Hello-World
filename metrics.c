#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char filename[256];
    int lines_total;
    int lines_code;
    int lines_comments;
    int lines_blank;
    int functions;
    int includes;
    int characters;
    int semicolons;
    int braces;
} CodeMetrics;

void analyze_file(const char* filename, CodeMetrics* metrics) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return;
    }
    
    strncpy(metrics->filename, filename, sizeof(metrics->filename) - 1);
    metrics->filename[sizeof(metrics->filename) - 1] = '\0';
    
    char line[1024];
    int in_multiline_comment = 0;
    
    while (fgets(line, sizeof(line), file)) {
        metrics->lines_total++;
        
        // Count characters (excluding newline)
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            len--;
        }
        metrics->characters += (int)len;
        
        // Trim whitespace for analysis
        char* trimmed = line;
        while (isspace(*trimmed)) trimmed++;
        char* end = trimmed + strlen(trimmed) - 1;
        while (end > trimmed && isspace(*end)) end--;
        *(end + 1) = '\0';
        
        // Check for blank lines
        if (strlen(trimmed) == 0) {
            metrics->lines_blank++;
            continue;
        }
        
        // Check for comments
        int is_comment = 0;
        if (strstr(trimmed, "//") || strstr(trimmed, "/*") || in_multiline_comment) {
            metrics->lines_comments++;
            is_comment = 1;
            if (strstr(trimmed, "/*") && !strstr(trimmed, "*/")) {
                in_multiline_comment = 1;
            }
            if (strstr(trimmed, "*/")) {
                in_multiline_comment = 0;
            }
        }
        
        if (!is_comment) {
            metrics->lines_code++;
            
            // Count includes (handle both "#include" and "# include")
            if (strstr(trimmed, "#include") || strstr(trimmed, "# include")) {
                metrics->includes++;
            }
            
            // Count functions (simple heuristic: lines with "int main" or "type function_name(")
            if (strstr(trimmed, "int main") || 
                (strchr(trimmed, '(') && strchr(trimmed, ')') && 
                 !strstr(trimmed, "#") && !strstr(trimmed, "printf"))) {
                metrics->functions++;
            }
        }
        
        // Count semicolons and braces
        for (char* p = line; *p; p++) {
            if (*p == ';') metrics->semicolons++;
            if (*p == '{' || *p == '}') metrics->braces++;
        }
    }
    
    fclose(file);
}

void print_spreadsheet_header() {
    printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                                           CODE METRICS SPREADSHEET                                                                                                                  ║\n");
    printf("╠══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║ File          │ Total │ Code  │ Comments │ Blank │ Functions │ Includes │ Characters │ Semicolons │ Braces │ Code Density │ Avg Line Len │ Complexity  ║\n");
    printf("║               │ Lines │ Lines │ Lines    │ Lines │           │          │            │            │        │ (Code/Total) │ (Chars/Lines) │ (Braces/Fn) ║\n");
    printf("╠═══════════════╪═══════╪═══════╪══════════╪═══════╪═══════════╪══════════╪════════════╪════════════╪════════╪══════════════╪═══════════════╪═════════════╣\n");
}

void print_spreadsheet_row(const CodeMetrics* metrics) {
    // Calculate derived metrics
    double code_density = metrics->lines_total > 0 ? (double)metrics->lines_code / metrics->lines_total : 0.0;
    double avg_line_length = metrics->lines_total > 0 ? (double)metrics->characters / metrics->lines_total : 0.0;
    double complexity = metrics->functions > 0 ? (double)metrics->braces / metrics->functions : 0.0;
    
    printf("║ %-13s │ %5d │ %5d │ %8d │ %5d │ %9d │ %8d │ %10d │ %10d │ %6d │ %11.2f%% │ %12.1f │ %10.1f ║\n",
           metrics->filename,
           metrics->lines_total,
           metrics->lines_code,
           metrics->lines_comments,
           metrics->lines_blank,
           metrics->functions,
           metrics->includes,
           metrics->characters,
           metrics->semicolons,
           metrics->braces,
           code_density * 100,
           avg_line_length,
           complexity);
}

void print_spreadsheet_footer() {
    printf("╚═══════════════╧═══════╧═══════╧══════════╧═══════╧═══════════╧══════════╧════════════╧════════════╧════════╧══════════════╧═══════════════╧═════════════╝\n");
}

void print_summary(const CodeMetrics* metrics) {
    printf("\n");
    printf("╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                                 DETAILED ANALYSIS SUMMARY                                                              ║\n");
    printf("╠═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║ File: %-30s                                                                                                 ║\n", metrics->filename);
    printf("║                                                                                                                                         ║\n");
    printf("║ Line Distribution:                                                                                                                      ║\n");
    printf("║   • Total Lines:     %5d (100.0%%)                                                                                                      ║\n", metrics->lines_total);
    printf("║   • Code Lines:      %5d (%5.1f%%)                                                                                                      ║\n", metrics->lines_code, metrics->lines_total > 0 ? (double)metrics->lines_code / metrics->lines_total * 100 : 0);
    printf("║   • Comment Lines:   %5d (%5.1f%%)                                                                                                      ║\n", metrics->lines_comments, metrics->lines_total > 0 ? (double)metrics->lines_comments / metrics->lines_total * 100 : 0);
    printf("║   • Blank Lines:     %5d (%5.1f%%)                                                                                                      ║\n", metrics->lines_blank, metrics->lines_total > 0 ? (double)metrics->lines_blank / metrics->lines_total * 100 : 0);
    printf("║                                                                                                                                         ║\n");
    printf("║ Code Structure:                                                                                                                         ║\n");
    printf("║   • Functions:       %5d                                                                                                               ║\n", metrics->functions);
    printf("║   • Include Statements: %2d                                                                                                             ║\n", metrics->includes);
    printf("║   • Total Characters:%5d                                                                                                               ║\n", metrics->characters);
    printf("║   • Semicolons:      %5d                                                                                                               ║\n", metrics->semicolons);
    printf("║   • Braces:          %5d                                                                                                               ║\n", metrics->braces);
    printf("║                                                                                                                                         ║\n");
    printf("║ Quality Metrics:                                                                                                                        ║\n");
    printf("║   • Code Density:    %6.2f%% (code lines / total lines)                                                                                ║\n", metrics->lines_total > 0 ? (double)metrics->lines_code / metrics->lines_total * 100 : 0);
    printf("║   • Avg Line Length: %6.1f characters                                                                                                  ║\n", metrics->lines_total > 0 ? (double)metrics->characters / metrics->lines_total : 0);
    printf("║   • Complexity Est.: %6.1f (braces per function)                                                                                       ║\n", metrics->functions > 0 ? (double)metrics->braces / metrics->functions : 0);
    printf("╚═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
}

void print_interactive_help() {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                              INTERACTIVE FEATURES HELP                                                                      ║\n");
    printf("╠═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║                                                                                                                                               ║\n");
    printf("║ Available Commands:                                                                                                                           ║\n");
    printf("║   metrics [filename]     - Analyze specified file (default: hello.c)                                                                        ║\n");
    printf("║   metrics --csv          - Output in CSV format for spreadsheet import                                                                       ║\n");
    printf("║   metrics --summary      - Show detailed analysis summary                                                                                    ║\n");
    printf("║   metrics --help         - Display this help information                                                                                     ║\n");
    printf("║                                                                                                                                               ║\n");
    printf("║ Examples:                                                                                                                                     ║\n");
    printf("║   ./metrics              - Analyze hello.c with spreadsheet view                                                                             ║\n");
    printf("║   ./metrics hello.c      - Analyze hello.c explicitly                                                                                        ║\n");
    printf("║   ./metrics --csv        - Generate CSV output for Excel/Google Sheets                                                                       ║\n");
    printf("║   ./metrics --summary    - Show detailed breakdown of hello.c metrics                                                                        ║\n");
    printf("║                                                                                                                                               ║\n");
    printf("║ Spreadsheet Features:                                                                                                                         ║\n");
    printf("║   • Tabular display with aligned columns                                                                                                     ║\n");
    printf("║   • Calculated metrics (density, complexity, averages)                                                                                       ║\n");
    printf("║   • CSV export for further analysis in Excel/Google Sheets                                                                                   ║\n");
    printf("║   • Summary view with detailed breakdowns and percentages                                                                                    ║\n");
    printf("║                                                                                                                                               ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
}

void print_csv_format(const CodeMetrics* metrics) {
    printf("Filename,Total Lines,Code Lines,Comment Lines,Blank Lines,Functions,Includes,Characters,Semicolons,Braces,Code Density %%,Avg Line Length,Complexity\n");
    
    double code_density = metrics->lines_total > 0 ? (double)metrics->lines_code / metrics->lines_total * 100 : 0.0;
    double avg_line_length = metrics->lines_total > 0 ? (double)metrics->characters / metrics->lines_total : 0.0;
    double complexity = metrics->functions > 0 ? (double)metrics->braces / metrics->functions : 0.0;
    
    printf("%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%.2f,%.1f,%.1f\n",
           metrics->filename,
           metrics->lines_total,
           metrics->lines_code,
           metrics->lines_comments,
           metrics->lines_blank,
           metrics->functions,
           metrics->includes,
           metrics->characters,
           metrics->semicolons,
           metrics->braces,
           code_density,
           avg_line_length,
           complexity);
}

int main(int argc, char* argv[]) {
    const char* filename = "hello.c";
    int csv_mode = 0;
    int summary_mode = 0;
    int help_mode = 0;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--csv") == 0) {
            csv_mode = 1;
        } else if (strcmp(argv[i], "--summary") == 0) {
            summary_mode = 1;
        } else if (strcmp(argv[i], "--help") == 0) {
            help_mode = 1;
        } else {
            filename = argv[i];
        }
    }
    
    if (help_mode) {
        print_interactive_help();
        return 0;
    }
    
    CodeMetrics metrics = {0};
    analyze_file(filename, &metrics);
    
    if (metrics.lines_total == 0) {
        printf("Error: Could not analyze file %s\n", filename);
        return 1;
    }
    
    if (csv_mode) {
        print_csv_format(&metrics);
    } else if (summary_mode) {
        print_summary(&metrics);
    } else {
        print_spreadsheet_header();
        print_spreadsheet_row(&metrics);
        print_spreadsheet_footer();
        
        printf("\nTip: Use './metrics --csv' for Excel/Google Sheets import, or './metrics --summary' for detailed analysis.\n");
    }
    
    return 0;
}