#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

// Copy the CodeMetrics structure and analyze_file function for testing
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

// Test helper functions
void create_test_file(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w");
    if (file) {
        fprintf(file, "%s", content);
        fclose(file);
    }
}

void cleanup_test_file(const char* filename) {
    remove(filename);
}

// Test cases
void test_empty_file() {
    printf("Running test_empty_file...\n");
    
    create_test_file("test_empty.c", "");
    
    CodeMetrics metrics;
    analyze_file("test_empty.c", &metrics);
    
    assert(metrics.total_lines == 0);
    assert(metrics.code_lines == 0);
    assert(metrics.comment_lines == 0);
    assert(metrics.blank_lines == 0);
    assert(metrics.include_count == 0);
    assert(metrics.function_count == 0);
    
    cleanup_test_file("test_empty.c");
    printf("✓ test_empty_file passed\n");
}

void test_simple_hello_world() {
    printf("Running test_simple_hello_world...\n");
    
    const char* content = "#include <stdio.h>\n\nint main(){\n    printf(\"Hello world!\");\n}\n";
    create_test_file("test_hello.c", content);
    
    CodeMetrics metrics;
    analyze_file("test_hello.c", &metrics);
    
    assert(metrics.total_lines == 5);
    assert(metrics.code_lines == 4);
    assert(metrics.comment_lines == 0);
    assert(metrics.blank_lines == 1);
    assert(metrics.include_count == 1);
    assert(metrics.function_count == 1);
    assert(metrics.statement_count == 1);
    assert(metrics.brace_count == 2);
    
    cleanup_test_file("test_hello.c");
    printf("✓ test_simple_hello_world passed\n");
}

void test_comments() {
    printf("Running test_comments...\n");
    
    const char* content = "// Single line comment\n#include <stdio.h>\n/* Multi-line\n   comment */\nint main() { return 0; }\n";
    create_test_file("test_comments.c", content);
    
    CodeMetrics metrics;
    analyze_file("test_comments.c", &metrics);
    
    assert(metrics.total_lines == 5);
    assert(metrics.code_lines == 2); // #include and main function
    assert(metrics.comment_lines == 3); // Single line + 2 multi-line
    assert(metrics.include_count == 1);
    assert(metrics.function_count == 1);
    
    cleanup_test_file("test_comments.c");
    printf("✓ test_comments passed\n");
}

void test_variables() {
    printf("Running test_variables...\n");
    
    const char* content = "#include <stdio.h>\nint x;\nchar buffer;\nfloat pi;\ndouble value;\nint main() { return 0; }\n";
    create_test_file("test_vars.c", content);
    
    CodeMetrics metrics;
    analyze_file("test_vars.c", &metrics);
    
    assert(metrics.include_count == 1);
    assert(metrics.variable_count == 4); // x, buffer, pi, value
    assert(metrics.function_count == 1);
    
    cleanup_test_file("test_vars.c");
    printf("✓ test_variables passed\n");
}

void test_nonexistent_file() {
    printf("Running test_nonexistent_file...\n");
    
    CodeMetrics metrics;
    // Initialize with garbage values to ensure they get cleared
    memset(&metrics, 0xFF, sizeof(CodeMetrics));
    
    analyze_file("nonexistent_file.c", &metrics);
    
    // Should be initialized to zero even if file doesn't exist
    assert(metrics.total_lines == 0);
    assert(metrics.code_lines == 0);
    assert(metrics.comment_lines == 0);
    assert(metrics.blank_lines == 0);
    assert(metrics.include_count == 0);
    assert(metrics.function_count == 0);
    
    printf("✓ test_nonexistent_file passed\n");
}

int main() {
    printf("Running Code Metrics Analyzer Unit Tests\n");
    printf("========================================\n");
    
    test_empty_file();
    test_simple_hello_world();
    test_comments();
    test_variables();
    test_nonexistent_file();
    
    printf("\n✓ All tests passed successfully!\n");
    return 0;
}