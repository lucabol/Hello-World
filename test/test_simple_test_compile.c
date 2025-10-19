/* Compile-only test for simple_test.h
 * Verifies that the test framework header compiles cleanly under strict flags
 * and can be included in a test program without issues.
 * 
 * This test doesn't run any actual tests - it just ensures the header
 * compiles and links correctly under the strict flags used by CI.
 */
#include "simple_test.h"

/* Minimal test to ensure framework compiles and links */
void test_framework_compiles(void) {
    /* This test just verifies the framework is usable */
    TEST_ASSERT_EQUAL_INT(1, 1);
}

int main(void) {
    RUN_TEST(test_framework_compiles);
    TEST_SUMMARY();
}
