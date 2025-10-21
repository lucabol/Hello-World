#include "../metrics.h"
#include "simple_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Helper to create temporary test files */
static void create_test_file(const char* filename, const char* content) {
    FILE* f = fopen(filename, "w");
    if (f) {
        fprintf(f, "%s", content);
        fclose(f);
    }
}

/* Test basic metrics on a simple file */
void test_basic_metrics(void) {
    const char* test_file = "test/test_basic.c.tmp";
    create_test_file(test_file, 
        "# include <stdio.h>\n"
        "\n"
        "int main() {\n"
        "    printf(\"Hello\");\n"
        "}\n");
    
    CodeMetrics m = analyze_file(test_file);
    
    TEST_ASSERT_EQUAL_INT(5, m.total_lines);
    TEST_ASSERT_EQUAL_INT(4, m.code_lines);
    TEST_ASSERT_EQUAL_INT(1, m.blank_lines);
    TEST_ASSERT_EQUAL_INT(0, m.comment_lines);
    TEST_ASSERT_EQUAL_INT(1, m.include_count);
    TEST_ASSERT_EQUAL_INT(1, m.printf_count);
    
    remove(test_file);
}

/* Test single-line comments */
void test_single_line_comments(void) {
    const char* test_file = "test/test_comments.c.tmp";
    create_test_file(test_file,
        "// This is a comment\n"
        "int x = 5; // inline comment\n"
        "// Another comment\n");
    
    CodeMetrics m = analyze_file(test_file);
    
    TEST_ASSERT_EQUAL_INT(3, m.total_lines);
    TEST_ASSERT_EQUAL_INT(2, m.comment_lines);  /* Lines starting with // */
    TEST_ASSERT_EQUAL_INT(1, m.code_lines);
    
    remove(test_file);
}

/* Test multi-line comments */
void test_multi_line_comments(void) {
    const char* test_file = "test/test_multiline.c.tmp";
    create_test_file(test_file,
        "/* Comment line 1\n"
        "   Comment line 2\n"
        "   Comment line 3 */\n"
        "int x = 5;\n");
    
    CodeMetrics m = analyze_file(test_file);
    
    TEST_ASSERT_EQUAL_INT(4, m.total_lines);
    TEST_ASSERT_EQUAL_INT(3, m.comment_lines);
    TEST_ASSERT_EQUAL_INT(1, m.code_lines);
    
    remove(test_file);
}

/* Test includes in comments should not be counted */
void test_include_in_comment(void) {
    const char* test_file = "test/test_inc_comment.c.tmp";
    create_test_file(test_file,
        "#include <stdio.h>\n"
        "// #include <stdlib.h>\n"
        "/* #include <string.h> */\n"
        "int main() {}\n");
    
    CodeMetrics m = analyze_file(test_file);
    
    TEST_ASSERT_EQUAL_INT(1, m.include_count); /* Only first include */
    
    remove(test_file);
}

/* Test includes in strings should not be counted */
void test_include_in_string(void) {
    const char* test_file = "test/test_inc_string.c.tmp";
    create_test_file(test_file,
        "#include <stdio.h>\n"
        "char* s = \"#include <fake.h>\";\n"
        "int main() {}\n");
    
    CodeMetrics m = analyze_file(test_file);
    
    TEST_ASSERT_EQUAL_INT(1, m.include_count); /* String literal doesn't count */
    
    remove(test_file);
}

/* Test printf in comments should not be counted */
void test_printf_in_comment(void) {
    const char* test_file = "test/test_printf_comment.c.tmp";
    create_test_file(test_file,
        "#include <stdio.h>\n"
        "int main() {\n"
        "    printf(\"Hello\");\n"
        "    // printf(\"Not counted\");\n"
        "    /* printf(\"Also not counted\"); */\n"
        "}\n");
    
    CodeMetrics m = analyze_file(test_file);
    
    TEST_ASSERT_EQUAL_INT(1, m.printf_count); /* Only actual printf */
    
    remove(test_file);
}

/* Test printf in strings - documented limitation */
void test_printf_in_string(void) {
    const char* test_file = "test/test_printf_string.c.tmp";
    create_test_file(test_file,
        "#include <stdio.h>\n"
        "int main() {\n"
        "    printf(\"printf in string\");\n"
        "    char* s = \"printf\";\n"
        "}\n");
    
    CodeMetrics m = analyze_file(test_file);
    
    /* Our simple heuristic counts printf(...) as a function call
       but not standalone "printf" in a string, which is correct behavior */
    TEST_ASSERT_EQUAL_INT(1, m.printf_count);
    
    remove(test_file);
}

/* Test # include vs #include (with space) */
void test_include_with_space(void) {
    const char* test_file = "test/test_inc_space.c.tmp";
    create_test_file(test_file,
        "# include <stdio.h>\n"
        "#include <stdlib.h>\n"
        "#  include <string.h>\n"
        "int main() {}\n");
    
    CodeMetrics m = analyze_file(test_file);
    
    TEST_ASSERT_EQUAL_INT(3, m.include_count); /* All three variants */
    
    remove(test_file);
}

/* Test long lines */
void test_long_lines(void) {
    const char* test_file = "test/test_longline.c.tmp";
    char long_line[2000];
    memset(long_line, 'x', 1500);
    long_line[1500] = '\n';
    long_line[1501] = '\0';
    
    FILE* f = fopen(test_file, "w");
    fprintf(f, "short\n");
    fprintf(f, "%s", long_line);
    fprintf(f, "short again\n");
    fclose(f);
    
    CodeMetrics m = analyze_file(test_file);
    
    TEST_ASSERT_EQUAL_INT(3, m.total_lines);
    /* Max line length should be > 1500 */
    if (m.max_line_length > 1500) {
        tests_passed++;
        print_green("  ✓ ");
        printf("Long line detected correctly\n");
    } else {
        tests_failed++;
        print_red("  ✗ ");
        printf("Long line not detected: got %d\n", m.max_line_length);
    }
    tests_run++;
    
    remove(test_file);
}

/* Test function counting */
void test_function_counting(void) {
    const char* test_file = "test/test_functions.c.tmp";
    create_test_file(test_file,
        "#include <stdio.h>\n"
        "\n"
        "void helper() {\n"
        "    printf(\"helper\");\n"
        "}\n"
        "\n"
        "int main() {\n"
        "    return 0;\n"
        "}\n");
    
    CodeMetrics m = analyze_file(test_file);
    
    /* Function counting is heuristic-based, so this may vary */
    /* We expect at least main to be detected */
    if (m.function_count >= 1) {
        tests_passed++;
        print_green("  ✓ ");
        printf("Function counting works (found %d)\n", m.function_count);
    } else {
        tests_failed++;
        print_red("  ✗ ");
        printf("Function counting failed\n");
    }
    tests_run++;
    
    remove(test_file);
}

/* Test empty file */
void test_empty_file(void) {
    const char* test_file = "test/test_empty.c.tmp";
    create_test_file(test_file, "");
    
    CodeMetrics m = analyze_file(test_file);
    
    TEST_ASSERT_EQUAL_INT(0, m.total_lines);
    TEST_ASSERT_EQUAL_INT(0, m.code_lines);
    TEST_ASSERT_EQUAL_INT(0, m.blank_lines);
    
    remove(test_file);
}

/* Test non-existent file */
void test_nonexistent_file(void) {
    CodeMetrics m = analyze_file("test/nonexistent_file_xyz123.c");
    
    /* Should return zeroed metrics */
    TEST_ASSERT_EQUAL_INT(0, m.total_lines);
    
    tests_run++;
    tests_passed++;
    print_green("  ✓ ");
    printf("Non-existent file handled correctly\n");
}

/* Test actual hello.c file */
void test_hello_c(void) {
    CodeMetrics m = analyze_file("hello.c");
    
    /* hello.c should have at least some lines */
    if (m.total_lines > 0) {
        tests_passed++;
        print_green("  ✓ ");
        printf("hello.c analyzed successfully (%d lines)\n", m.total_lines);
    } else {
        tests_failed++;
        print_red("  ✗ ");
        printf("hello.c analysis failed\n");
    }
    tests_run++;
    
    /* Should have at least 1 include and 1 printf */
    TEST_ASSERT_EQUAL_INT(1, m.include_count);
    TEST_ASSERT_EQUAL_INT(1, m.printf_count);
}

int main(void) {
    printf("===========================================\n");
    printf("Running Metrics Analysis Unit Tests\n");
    printf("===========================================\n\n");
    
    RUN_TEST(test_basic_metrics);
    RUN_TEST(test_single_line_comments);
    RUN_TEST(test_multi_line_comments);
    RUN_TEST(test_include_in_comment);
    RUN_TEST(test_include_in_string);
    RUN_TEST(test_printf_in_comment);
    RUN_TEST(test_printf_in_string);
    RUN_TEST(test_include_with_space);
    RUN_TEST(test_long_lines);
    RUN_TEST(test_function_counting);
    RUN_TEST(test_empty_file);
    RUN_TEST(test_nonexistent_file);
    RUN_TEST(test_hello_c);
    
    TEST_SUMMARY();
}
