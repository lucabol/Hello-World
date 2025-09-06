#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_FUNCTIONS 100
#define MAX_INCLUDES 50

typedef struct {
    char name[256];
    int line_number;
} function_info;

typedef struct {
    char name[256];
    int line_number;
} include_info;

typedef struct {
    int total_lines;
    int blank_lines;
    int comment_lines;
    int code_lines;
    int function_count;
    int include_count;
    int character_count;
    int file_size;
    function_info functions[MAX_FUNCTIONS];
    include_info includes[MAX_INCLUDES];
} code_metrics;

// Function prototypes
code_metrics analyze_file(const char* filename);
void display_metrics(code_metrics* metrics, const char* filename);
void draw_table(int start_row, int current_selection);
int count_tokens(const char* line);

int main(int argc, char* argv[]) {
    const char* filename = "hello.c";
    
    if (argc > 1) {
        filename = argv[1];
    }
    
    // Analyze the file
    code_metrics metrics = analyze_file(filename);
    
    if (metrics.total_lines == -1) {
        printf("Error: Could not open file '%s'\n", filename);
        return 1;
    }
    
    // Display interactive interface
    display_metrics(&metrics, filename);
    
    return 0;
}

code_metrics analyze_file(const char* filename) {
    code_metrics metrics = {0};
    FILE* file = fopen(filename, "r");
    char line[MAX_LINE_LEN];
    int line_num = 0;
    
    if (!file) {
        metrics.total_lines = -1;
        return metrics;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    metrics.file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    while (fgets(line, sizeof(line), file)) {
        line_num++;
        metrics.total_lines++;
        metrics.character_count += strlen(line);
        
        // Remove trailing newline for analysis
        line[strcspn(line, "\n")] = 0;
        
        // Check for blank lines
        int is_blank = 1;
        for (int i = 0; line[i]; i++) {
            if (!isspace(line[i])) {
                is_blank = 0;
                break;
            }
        }
        
        if (is_blank) {
            metrics.blank_lines++;
            continue;
        }
        
        // Check for comment lines (simple check for lines starting with //)
        char* trimmed = line;
        while (*trimmed && isspace(*trimmed)) trimmed++;
        
        if (strncmp(trimmed, "//", 2) == 0 || strncmp(trimmed, "/*", 2) == 0) {
            metrics.comment_lines++;
            continue;
        }
        
        // It's a code line
        metrics.code_lines++;
        
        // Check for includes (handle both "#include" and "# include")
        if ((strncmp(trimmed, "#include", 8) == 0 || strncmp(trimmed, "# include", 9) == 0) && 
            metrics.include_count < MAX_INCLUDES) {
            strncpy(metrics.includes[metrics.include_count].name, line, 255);
            metrics.includes[metrics.include_count].line_number = line_num;
            metrics.include_count++;
        }
        
        // Check for function definitions (simple heuristic)
        if (strstr(line, "(") && strstr(line, ")") && strstr(line, "{") && 
            !strstr(line, "printf") && !strstr(line, "if") && !strstr(line, "while") &&
            !strstr(line, "for") && metrics.function_count < MAX_FUNCTIONS) {
            
            // Extract function name (simplified)
            char* func_start = strstr(line, " ");
            if (func_start) {
                func_start++;
                char* paren = strstr(func_start, "(");
                if (paren) {
                    int len = paren - func_start;
                    if (len > 0 && len < 255) {
                        strncpy(metrics.functions[metrics.function_count].name, func_start, len);
                        metrics.functions[metrics.function_count].name[len] = '\0';
                        metrics.functions[metrics.function_count].line_number = line_num;
                        metrics.function_count++;
                    }
                }
            }
        }
    }
    
    fclose(file);
    return metrics;
}

void display_metrics(code_metrics* metrics, const char* filename) {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    
    int ch;
    int current_view = 0; // 0: summary, 1: functions, 2: includes
    int current_selection = 0;
    
    while ((ch = getch()) != 'q') {
        clear();
        
        // Header
        attron(A_BOLD);
        mvprintw(0, 0, "Code Metrics Analyzer - %s", filename);
        mvprintw(1, 0, "Use arrow keys to navigate, TAB to switch views, 'q' to quit");
        attroff(A_BOLD);
        
        // View selector
        mvprintw(3, 0, "Views: [%s] Summary  [%s] Functions  [%s] Includes", 
                 current_view == 0 ? "*" : " ",
                 current_view == 1 ? "*" : " ",
                 current_view == 2 ? "*" : " ");
        
        // Display content based on current view
        switch (current_view) {
            case 0: // Summary view
                mvprintw(5, 0, "┌─────────────────────────┬──────────┐");
                mvprintw(6, 0, "│ Metric                  │ Value    │");
                mvprintw(7, 0, "├─────────────────────────┼──────────┤");
                mvprintw(8, 0, "│ Total Lines             │ %8d │", metrics->total_lines);
                mvprintw(9, 0, "│ Code Lines              │ %8d │", metrics->code_lines);
                mvprintw(10, 0, "│ Blank Lines             │ %8d │", metrics->blank_lines);
                mvprintw(11, 0, "│ Comment Lines           │ %8d │", metrics->comment_lines);
                mvprintw(12, 0, "│ Functions               │ %8d │", metrics->function_count);
                mvprintw(13, 0, "│ Include Statements      │ %8d │", metrics->include_count);
                mvprintw(14, 0, "│ File Size (bytes)       │ %8d │", metrics->file_size);
                mvprintw(15, 0, "│ Character Count         │ %8d │", metrics->character_count);
                mvprintw(16, 0, "└─────────────────────────┴──────────┘");
                break;
                
            case 1: // Functions view
                mvprintw(5, 0, "┌─────────────────────────┬──────────┐");
                mvprintw(6, 0, "│ Function Name           │ Line #   │");
                mvprintw(7, 0, "├─────────────────────────┼──────────┤");
                if (metrics->function_count == 0) {
                    mvprintw(8, 0, "│ No functions found      │          │");
                } else {
                    for (int i = 0; i < metrics->function_count && i < 10; i++) {
                        if (i == current_selection) attron(A_REVERSE);
                        mvprintw(8 + i, 0, "│ %-23s │ %8d │", 
                                metrics->functions[i].name, 
                                metrics->functions[i].line_number);
                        if (i == current_selection) attroff(A_REVERSE);
                    }
                }
                mvprintw(8 + (metrics->function_count > 0 ? metrics->function_count : 1), 0, 
                         "└─────────────────────────┴──────────┘");
                break;
                
            case 2: // Includes view
                mvprintw(5, 0, "┌─────────────────────────────────────────┬──────────┐");
                mvprintw(6, 0, "│ Include Statement                       │ Line #   │");
                mvprintw(7, 0, "├─────────────────────────────────────────┼──────────┤");
                if (metrics->include_count == 0) {
                    mvprintw(8, 0, "│ No includes found                       │          │");
                } else {
                    for (int i = 0; i < metrics->include_count && i < 10; i++) {
                        if (i == current_selection) attron(A_REVERSE);
                        mvprintw(8 + i, 0, "│ %-39s │ %8d │", 
                                metrics->includes[i].name, 
                                metrics->includes[i].line_number);
                        if (i == current_selection) attroff(A_REVERSE);
                    }
                }
                mvprintw(8 + (metrics->include_count > 0 ? metrics->include_count : 1), 0, 
                         "└─────────────────────────────────────────┴──────────┘");
                break;
        }
        
        refresh();
        
        // Handle navigation
        switch (ch) {
            case '\t': // Tab - switch views
                current_view = (current_view + 1) % 3;
                current_selection = 0;
                break;
            case KEY_UP:
                if (current_selection > 0) current_selection--;
                break;
            case KEY_DOWN:
                if (current_view == 1 && current_selection < metrics->function_count - 1) {
                    current_selection++;
                } else if (current_view == 2 && current_selection < metrics->include_count - 1) {
                    current_selection++;
                }
                break;
        }
    }
    
    endwin();
}