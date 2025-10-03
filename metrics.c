/* Code Metrics Analyzer - Spreadsheet-like Interface
 * Analyzes hello.c and displays various code metrics in tabular format
 * Features:
 * - Tabular display of metrics
 * - Line-by-line analysis
 * - Interactive filtering options
 * - Export capabilities
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LINES 1000

typedef struct {
    char *name;
    int value;
} Metric;

typedef struct {
    int total_lines;
    int code_lines;
    int blank_lines;
    int comment_lines;
    int includes;
    int functions;
    int statements;
    int chars;
} CodeMetrics;

typedef struct {
    int line_number;
    char content[MAX_LINE_LENGTH];
    char type[20];  /* "code", "blank", "comment" */
    int char_count;
    int has_semicolon;
    int has_include;
} LineInfo;

typedef struct {
    LineInfo lines[MAX_LINES];
    int line_count;
} LineAnalysis;

/* Check if a line is blank */
int is_blank_line(const char *line) {
    while (*line) {
        if (!isspace((unsigned char)*line)) {
            return 0;
        }
        line++;
    }
    return 1;
}

/* Check if a line contains a comment */
int is_comment_line(const char *line) {
    const char *p = line;
    while (*p && isspace((unsigned char)*p)) {
        p++;
    }
    return (p[0] == '/' && p[1] == '/') || (p[0] == '/' && p[1] == '*');
}

/* Analyze the code file */
CodeMetrics analyze_file(const char *filename) {
    CodeMetrics metrics = {0, 0, 0, 0, 0, 0, 0, 0};
    FILE *file = fopen(filename, "r");
    char line[MAX_LINE_LENGTH];
    
    if (!file) {
        fprintf(stderr, "Error: Could not open file '%s'\n", filename);
        return metrics;
    }
    
    while (fgets(line, sizeof(line), file)) {
        metrics.total_lines++;
        metrics.chars += (int)strlen(line);
        
        if (is_blank_line(line)) {
            metrics.blank_lines++;
        } else if (is_comment_line(line)) {
            metrics.comment_lines++;
        } else {
            metrics.code_lines++;
            
            /* Count includes */
            if (strstr(line, "#include") || strstr(line, "# include")) {
                metrics.includes++;
            }
            
            /* Count function definitions (simple heuristic: lines with '{' at end or standalone) */
            if (strstr(line, "int main") || strstr(line, "void ") || strstr(line, "char *")) {
                const char *p = line;
                while (*p && isspace((unsigned char)*p)) p++;
                if (strstr(p, "int main") || strstr(p, "void ") || strstr(p, "char *")) {
                    metrics.functions++;
                }
            }
            
            /* Count statements (lines with semicolon) */
            if (strchr(line, ';')) {
                metrics.statements++;
            }
        }
    }
    
    fclose(file);
    return metrics;
}

/* Perform line-by-line analysis */
LineAnalysis analyze_lines(const char *filename) {
    LineAnalysis analysis = {{{0}}, 0};
    FILE *file = fopen(filename, "r");
    char line[MAX_LINE_LENGTH];
    
    if (!file) {
        return analysis;
    }
    
    while (fgets(line, sizeof(line), file) && analysis.line_count < MAX_LINES) {
        LineInfo *info = &analysis.lines[analysis.line_count];
        info->line_number = analysis.line_count + 1;
        
        /* Copy content (remove newline for display) */
        strncpy(info->content, line, MAX_LINE_LENGTH - 1);
        info->content[MAX_LINE_LENGTH - 1] = '\0';
        size_t len = strlen(info->content);
        if (len > 0 && info->content[len - 1] == '\n') {
            info->content[len - 1] = '\0';
        }
        
        info->char_count = (int)strlen(info->content);
        info->has_semicolon = (strchr(line, ';') != NULL);
        info->has_include = (strstr(line, "#include") != NULL || strstr(line, "# include") != NULL);
        
        /* Determine line type */
        if (is_blank_line(line)) {
            strcpy(info->type, "blank");
        } else if (is_comment_line(line)) {
            strcpy(info->type, "comment");
        } else {
            strcpy(info->type, "code");
        }
        
        analysis.line_count++;
    }
    
    fclose(file);
    return analysis;
}

/* Print a horizontal line for table borders */
void print_border(int width) {
    printf("+");
    for (int i = 0; i < width; i++) {
        printf("-");
    }
    printf("+\n");
}

/* Print a table row with metric name and value */
void print_row(const char *name, int value, int name_width, int value_width) {
    printf("| %-*s | %*d |\n", name_width, name, value_width, value);
}

/* Print the spreadsheet-like table */
void print_spreadsheet(const char *filename, CodeMetrics metrics) {
    const int name_width = 30;
    const int value_width = 10;
    const int total_width = name_width + value_width + 5;
    
    printf("\n");
    printf("================================================================================\n");
    printf("                     CODE METRICS ANALYSIS SPREADSHEET\n");
    printf("================================================================================\n");
    printf("File: %s\n", filename);
    printf("\n");
    
    print_border(total_width);
    printf("| %-*s | %*s |\n", name_width, "Metric", value_width, "Value");
    print_border(total_width);
    
    /* Lines section */
    print_row("Total Lines", metrics.total_lines, name_width, value_width);
    print_row("Code Lines", metrics.code_lines, name_width, value_width);
    print_row("Blank Lines", metrics.blank_lines, name_width, value_width);
    print_row("Comment Lines", metrics.comment_lines, name_width, value_width);
    print_border(total_width);
    
    /* Code structure section */
    print_row("Include Directives", metrics.includes, name_width, value_width);
    print_row("Functions", metrics.functions, name_width, value_width);
    print_row("Statements", metrics.statements, name_width, value_width);
    print_border(total_width);
    
    /* Size metrics */
    print_row("Total Characters", metrics.chars, name_width, value_width);
    print_row("Avg Chars per Line", 
              metrics.total_lines > 0 ? metrics.chars / metrics.total_lines : 0,
              name_width, value_width);
    print_border(total_width);
    
    printf("\n");
    printf("Summary Statistics:\n");
    printf("  Code Density: %.1f%% (code lines / total lines)\n", 
           metrics.total_lines > 0 ? (100.0 * metrics.code_lines / metrics.total_lines) : 0);
    printf("  Comment Ratio: %.1f%% (comment lines / total lines)\n",
           metrics.total_lines > 0 ? (100.0 * metrics.comment_lines / metrics.total_lines) : 0);
    printf("  Blank Ratio: %.1f%% (blank lines / total lines)\n",
           metrics.total_lines > 0 ? (100.0 * metrics.blank_lines / metrics.total_lines) : 0);
    printf("\n");
    printf("================================================================================\n");
}

/* Print line-by-line analysis in spreadsheet format */
void print_line_analysis(LineAnalysis analysis) {
    printf("\n");
    printf("================================================================================\n");
    printf("                     LINE-BY-LINE ANALYSIS (Spreadsheet View)\n");
    printf("================================================================================\n");
    printf("\n");
    
    /* Header */
    printf("+------+----------+-------+-----------+---------+---------------------------+\n");
    printf("| Line | Type     | Chars | Semicolon | Include | Content                   |\n");
    printf("+------+----------+-------+-----------+---------+---------------------------+\n");
    
    /* Data rows */
    for (int i = 0; i < analysis.line_count; i++) {
        LineInfo *line = &analysis.lines[i];
        
        /* Truncate content if too long */
        char display_content[28];
        strncpy(display_content, line->content, 24);
        display_content[24] = '\0';
        if ((int)strlen(line->content) > 24) {
            strcat(display_content, "...");
        }
        
        printf("| %4d | %-8s | %5d | %-9s | %-7s | %-25s |\n",
               line->line_number,
               line->type,
               line->char_count,
               line->has_semicolon ? "Yes" : "No",
               line->has_include ? "Yes" : "No",
               display_content);
    }
    
    printf("+------+----------+-------+-----------+---------+---------------------------+\n");
    printf("\n");
}

/* Export metrics to CSV format */
void export_csv(const char *filename, CodeMetrics metrics, LineAnalysis analysis) {
    char csv_filename[256];
    snprintf(csv_filename, sizeof(csv_filename), "%s_metrics.csv", filename);
    
    FILE *csv = fopen(csv_filename, "w");
    if (!csv) {
        fprintf(stderr, "Error: Could not create CSV file '%s'\n", csv_filename);
        return;
    }
    
    /* Write summary metrics */
    fprintf(csv, "Metric,Value\n");
    fprintf(csv, "Total Lines,%d\n", metrics.total_lines);
    fprintf(csv, "Code Lines,%d\n", metrics.code_lines);
    fprintf(csv, "Blank Lines,%d\n", metrics.blank_lines);
    fprintf(csv, "Comment Lines,%d\n", metrics.comment_lines);
    fprintf(csv, "Include Directives,%d\n", metrics.includes);
    fprintf(csv, "Functions,%d\n", metrics.functions);
    fprintf(csv, "Statements,%d\n", metrics.statements);
    fprintf(csv, "Total Characters,%d\n", metrics.chars);
    fprintf(csv, "\n");
    
    /* Write line-by-line data */
    fprintf(csv, "Line,Type,Chars,Semicolon,Include,Content\n");
    for (int i = 0; i < analysis.line_count; i++) {
        LineInfo *line = &analysis.lines[i];
        /* Escape quotes in content */
        char escaped_content[MAX_LINE_LENGTH * 2];
        int j = 0;
        for (int k = 0; line->content[k] && j < (int)sizeof(escaped_content) - 2; k++) {
            if (line->content[k] == '"') {
                escaped_content[j++] = '"';
            }
            escaped_content[j++] = line->content[k];
        }
        escaped_content[j] = '\0';
        
        fprintf(csv, "%d,%s,%d,%s,%s,\"%s\"\n",
                line->line_number,
                line->type,
                line->char_count,
                line->has_semicolon ? "Yes" : "No",
                line->has_include ? "Yes" : "No",
                escaped_content);
    }
    
    fclose(csv);
    printf("Metrics exported to: %s\n", csv_filename);
}

int main(int argc, char *argv[]) {
    const char *filename = "hello.c";
    int show_lines = 0;
    int export_csv_flag = 0;
    
    /* Parse command line arguments */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--lines") == 0 || strcmp(argv[i], "-l") == 0) {
            show_lines = 1;
        } else if (strcmp(argv[i], "--csv") == 0 || strcmp(argv[i], "-c") == 0) {
            export_csv_flag = 1;
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("Code Metrics Analyzer - Spreadsheet Interface\n");
            printf("Usage: %s [options] [filename]\n", argv[0]);
            printf("Options:\n");
            printf("  -l, --lines    Show line-by-line analysis\n");
            printf("  -c, --csv      Export metrics to CSV file\n");
            printf("  -h, --help     Show this help message\n");
            printf("\nDefault file: hello.c\n");
            return 0;
        } else {
            filename = argv[i];
        }
    }
    
    printf("Code Metrics Analyzer - Spreadsheet Interface\n");
    printf("Analyzing: %s\n", filename);
    
    CodeMetrics metrics = analyze_file(filename);
    
    if (metrics.total_lines == 0) {
        fprintf(stderr, "Error: Failed to analyze file or file is empty\n");
        return 1;
    }
    
    print_spreadsheet(filename, metrics);
    
    LineAnalysis analysis = {{{0}}, 0};
    if (show_lines || export_csv_flag) {
        analysis = analyze_lines(filename);
    }
    
    if (show_lines) {
        print_line_analysis(analysis);
    }
    
    if (export_csv_flag) {
        export_csv(filename, metrics, analysis);
    }
    
    return 0;
}
