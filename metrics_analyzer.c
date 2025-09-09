#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int total_lines;
    int code_lines;
    int comment_lines;
    int blank_lines;
    int include_count;
    int function_count;
    int variable_count;
    int statement_count;
    int brace_count;
    int semicolon_count;
} CodeMetrics;

void analyze_file(const char* filename, CodeMetrics* metrics) {
    // Initialize metrics first to ensure they're always in a known state
    memset(metrics, 0, sizeof(CodeMetrics));
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return;
    }

    char line[1024];
    int in_multiline_comment = 0;
    
    while (fgets(line, sizeof(line), file)) {
        metrics->total_lines++;
        
        // Ensure null termination and handle potential overflow
        line[sizeof(line) - 1] = '\0';
        
        // Trim whitespace
        char* trimmed = line;
        while (isspace(*trimmed)) trimmed++;
        
        int line_len = strlen(trimmed);
        if (line_len > 0 && trimmed[line_len-1] == '\n') {
            trimmed[line_len-1] = '\0';
            line_len--;
        }
        
        // Check for blank lines
        if (line_len == 0) {
            metrics->blank_lines++;
            continue;
        }
        
        // Check for comments
        int is_comment_line = 0;
        if (strstr(trimmed, "//") == trimmed) {
            metrics->comment_lines++;
            is_comment_line = 1;
        } else if (strstr(trimmed, "/*")) {
            metrics->comment_lines++;
            is_comment_line = 1;
            in_multiline_comment = 1;
        } else if (in_multiline_comment) {
            metrics->comment_lines++;
            is_comment_line = 1;
            if (strstr(trimmed, "*/")) {
                in_multiline_comment = 0;
            }
        }
        
        if (!is_comment_line) {
            metrics->code_lines++;
            
            // Count includes (handle both "#include" and "# include")
            if (strstr(trimmed, "#include") || strstr(trimmed, "# include")) {
                metrics->include_count++;
            }
            
            // Count functions (simple heuristic: look for function patterns)
            if (strstr(trimmed, "int main") || 
                (strstr(trimmed, "(") && strstr(trimmed, ")") && strstr(trimmed, "{"))) {
                metrics->function_count++;
            }
            
            // Count variables (simple heuristic: lines with int, char, float, etc.)
            // Don't count function declarations
            if ((strstr(trimmed, "int ") || strstr(trimmed, "char ") || 
                strstr(trimmed, "float ") || strstr(trimmed, "double ")) &&
                !strstr(trimmed, "(") && !strstr(trimmed, "main")) {
                char* ptr = trimmed;
                while ((ptr = strchr(ptr, ' ')) != NULL) {
                    ptr++;
                    if (isalpha(*ptr)) {
                        metrics->variable_count++;
                        break;
                    }
                }
            }
            
            // Count statements (semicolons)
            char* ptr = trimmed;
            while ((ptr = strchr(ptr, ';')) != NULL) {
                metrics->semicolon_count++;
                metrics->statement_count++;
                ptr++;
            }
            
            // Count braces
            ptr = trimmed;
            while (*ptr) {
                if (*ptr == '{' || *ptr == '}') {
                    metrics->brace_count++;
                }
                ptr++;
            }
        }
    }
    
    fclose(file);
}

void generate_html_report(const char* source_file, CodeMetrics* metrics) {
    FILE* html_file = fopen("code_metrics.html", "w");
    if (!html_file) {
        printf("Error: Cannot create HTML report file\n");
        return;
    }
    
    fprintf(html_file, "<!DOCTYPE html>\n");
    fprintf(html_file, "<html>\n<head>\n");
    fprintf(html_file, "<title>Code Metrics - %s</title>\n", source_file);
    fprintf(html_file, "<style>\n");
    fprintf(html_file, "body { font-family: Arial, sans-serif; margin: 20px; }\n");
    fprintf(html_file, ".container { max-width: 800px; margin: 0 auto; }\n");
    fprintf(html_file, "h1 { color: #333; text-align: center; }\n");
    fprintf(html_file, "table { width: 100%%; border-collapse: collapse; margin: 20px 0; }\n");
    fprintf(html_file, "th, td { border: 1px solid #ddd; padding: 12px; text-align: left; }\n");
    fprintf(html_file, "th { background-color: #f2f2f2; font-weight: bold; cursor: pointer; }\n");
    fprintf(html_file, "th:hover { background-color: #e0e0e0; }\n");
    fprintf(html_file, "tr:nth-child(even) { background-color: #f9f9f9; }\n");
    fprintf(html_file, "tr:hover { background-color: #f5f5f5; }\n");
    fprintf(html_file, ".metric-value { font-weight: bold; color: #2c5aa0; }\n");
    fprintf(html_file, ".summary { background-color: #e8f4fd; padding: 15px; border-radius: 5px; margin: 20px 0; }\n");
    fprintf(html_file, "</style>\n");
    fprintf(html_file, "<script>\n");
    fprintf(html_file, "function sortTable(columnIndex) {\n");
    fprintf(html_file, "  var table = document.getElementById('metricsTable');\n");
    fprintf(html_file, "  var rows = Array.from(table.rows).slice(1);\n");
    fprintf(html_file, "  var sortedRows = rows.sort(function(a, b) {\n");
    fprintf(html_file, "    var aVal = a.cells[columnIndex].textContent;\n");
    fprintf(html_file, "    var bVal = b.cells[columnIndex].textContent;\n");
    fprintf(html_file, "    if (!isNaN(aVal) && !isNaN(bVal)) {\n");
    fprintf(html_file, "      return parseFloat(bVal) - parseFloat(aVal);\n");
    fprintf(html_file, "    }\n");
    fprintf(html_file, "    return aVal.localeCompare(bVal);\n");
    fprintf(html_file, "  });\n");
    fprintf(html_file, "  sortedRows.forEach(function(row) { table.appendChild(row); });\n");
    fprintf(html_file, "}\n");
    fprintf(html_file, "</script>\n");
    fprintf(html_file, "</head>\n<body>\n");
    
    fprintf(html_file, "<div class='container'>\n");
    fprintf(html_file, "<h1>Code Metrics Analysis</h1>\n");
    fprintf(html_file, "<div class='summary'>\n");
    fprintf(html_file, "<h2>File: %s</h2>\n", source_file);
    fprintf(html_file, "<p>Analysis completed successfully. Click on column headers to sort.</p>\n");
    fprintf(html_file, "</div>\n");
    
    fprintf(html_file, "<table id='metricsTable'>\n");
    fprintf(html_file, "<thead>\n");
    fprintf(html_file, "<tr>\n");
    fprintf(html_file, "<th onclick='sortTable(0)'>Metric</th>\n");
    fprintf(html_file, "<th onclick='sortTable(1)'>Value</th>\n");
    fprintf(html_file, "<th onclick='sortTable(2)'>Description</th>\n");
    fprintf(html_file, "</tr>\n");
    fprintf(html_file, "</thead>\n");
    fprintf(html_file, "<tbody>\n");
    
    // Add metrics rows
    fprintf(html_file, "<tr><td>Total Lines</td><td class='metric-value'>%d</td><td>Total number of lines in the file</td></tr>\n", metrics->total_lines);
    fprintf(html_file, "<tr><td>Code Lines</td><td class='metric-value'>%d</td><td>Lines containing actual code</td></tr>\n", metrics->code_lines);
    fprintf(html_file, "<tr><td>Comment Lines</td><td class='metric-value'>%d</td><td>Lines containing comments</td></tr>\n", metrics->comment_lines);
    fprintf(html_file, "<tr><td>Blank Lines</td><td class='metric-value'>%d</td><td>Empty or whitespace-only lines</td></tr>\n", metrics->blank_lines);
    fprintf(html_file, "<tr><td>Include Statements</td><td class='metric-value'>%d</td><td>Number of #include directives</td></tr>\n", metrics->include_count);
    fprintf(html_file, "<tr><td>Functions</td><td class='metric-value'>%d</td><td>Number of function definitions</td></tr>\n", metrics->function_count);
    fprintf(html_file, "<tr><td>Variables</td><td class='metric-value'>%d</td><td>Number of variable declarations</td></tr>\n", metrics->variable_count);
    fprintf(html_file, "<tr><td>Statements</td><td class='metric-value'>%d</td><td>Number of statements (semicolons)</td></tr>\n", metrics->statement_count);
    fprintf(html_file, "<tr><td>Braces</td><td class='metric-value'>%d</td><td>Number of opening/closing braces</td></tr>\n", metrics->brace_count);
    
    // Calculate ratios
    double code_ratio = metrics->total_lines > 0 ? (double)metrics->code_lines / metrics->total_lines * 100 : 0;
    double comment_ratio = metrics->total_lines > 0 ? (double)metrics->comment_lines / metrics->total_lines * 100 : 0;
    
    fprintf(html_file, "<tr><td>Code Ratio</td><td class='metric-value'>%.1f%%</td><td>Percentage of lines that are code</td></tr>\n", code_ratio);
    fprintf(html_file, "<tr><td>Comment Ratio</td><td class='metric-value'>%.1f%%</td><td>Percentage of lines that are comments</td></tr>\n", comment_ratio);
    
    fprintf(html_file, "</tbody>\n");
    fprintf(html_file, "</table>\n");
    fprintf(html_file, "</div>\n");
    fprintf(html_file, "</body>\n</html>\n");
    
    fclose(html_file);
    printf("HTML report generated: code_metrics.html\n");
}

int main(int argc, char* argv[]) {
    const char* filename = "hello.c";
    
    if (argc > 1) {
        filename = argv[1];
    }
    
    printf("Analyzing file: %s\n", filename);
    
    CodeMetrics metrics;
    analyze_file(filename, &metrics);
    
    // Print summary to console
    printf("\n=== Code Metrics Summary ===\n");
    printf("Total Lines: %d\n", metrics.total_lines);
    printf("Code Lines: %d\n", metrics.code_lines);
    printf("Comment Lines: %d\n", metrics.comment_lines);
    printf("Blank Lines: %d\n", metrics.blank_lines);
    printf("Include Statements: %d\n", metrics.include_count);
    printf("Functions: %d\n", metrics.function_count);
    printf("Variables: %d\n", metrics.variable_count);
    printf("Statements: %d\n", metrics.statement_count);
    printf("Braces: %d\n", metrics.brace_count);
    
    // Generate HTML report
    generate_html_report(filename, &metrics);
    
    return 0;
}