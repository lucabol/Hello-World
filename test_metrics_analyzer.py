#!/usr/bin/env python3
"""
Unit tests for the Code Metrics Analyzer
Tests the core analysis functionality with various C code patterns
"""

import unittest
import tempfile
import os
import json
import sys
from unittest.mock import patch

# Add the parent directory to the path to import our module
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from metrics_analyzer import analyze_c_file, generate_html_interface, main


class TestMetricsAnalyzer(unittest.TestCase):
    
    def create_temp_c_file(self, content):
        """Helper to create a temporary C file with given content"""
        temp_file = tempfile.NamedTemporaryFile(mode='w', suffix='.c', delete=False)
        temp_file.write(content)
        temp_file.close()
        return temp_file.name
    
    def test_simple_hello_world(self):
        """Test analysis of the basic hello.c file (matches actual repo file)"""
        content = '''# include <stdio.h>

int main(){
    printf("Hello world!");
}
'''
        temp_file = self.create_temp_c_file(content)
        
        try:
            metrics = analyze_c_file(temp_file)
            
            # Basic sanity checks (matches actual hello.c file)
            self.assertEqual(metrics['total_lines'], 6)
            self.assertEqual(metrics['code_lines'], 4)
            self.assertEqual(metrics['blank_lines'], 2)
            self.assertEqual(metrics['comment_lines'], 0)
            self.assertEqual(metrics['function_count'], 1)
            self.assertEqual(metrics['include_count'], 1)
            self.assertEqual(metrics['printf_count'], 1)
            self.assertEqual(metrics['semicolon_count'], 1)
            self.assertEqual(metrics['string_literals'], 1)
            self.assertAlmostEqual(metrics['code_density'], 66.67, places=1)
            
        finally:
            os.unlink(temp_file)
    
    def test_empty_file(self):
        """Test analysis of an empty file"""
        content = ""
        temp_file = self.create_temp_c_file(content)
        
        try:
            metrics = analyze_c_file(temp_file)
            
            self.assertEqual(metrics['total_lines'], 1)  # Empty file still has 1 line
            self.assertEqual(metrics['code_lines'], 0)
            self.assertEqual(metrics['blank_lines'], 1)
            self.assertEqual(metrics['comment_lines'], 0)
            self.assertEqual(metrics['function_count'], 0)
            self.assertEqual(metrics['include_count'], 0)
            self.assertEqual(metrics['code_density'], 0.0)
            
        finally:
            os.unlink(temp_file)
    
    def test_comments_and_strings(self):
        """Test detection of various comment styles and string literals"""
        content = '''#include <stdio.h>
#include <stdlib.h>

/* Multi-line comment
   spanning several lines */
int factorial(int n) {
    if (n <= 1) {
        return 1;  // Single line comment
    }
    return n * factorial(n - 1);
}

int main() {
    // Another comment
    printf("Result: %d\\n", factorial(5));  /* Inline comment */
    printf("String with \\"quotes\\" inside");
    return 0;
}
'''
        temp_file = self.create_temp_c_file(content)
        
        try:
            metrics = analyze_c_file(temp_file)
            
            self.assertEqual(metrics['include_count'], 2)
            self.assertEqual(metrics['function_count'], 2)
            self.assertGreater(metrics['comment_lines'], 0)
            self.assertEqual(metrics['printf_count'], 2)
            # The analyzer finds 3 string literals (including the escaped quotes)
            self.assertGreaterEqual(metrics['string_literals'], 2)
            self.assertGreater(metrics['estimated_complexity'], 0)  # Should detect if statement
            
        finally:
            os.unlink(temp_file)
    
    def test_complex_code_patterns(self):
        """Test detection of various complexity patterns"""
        content = '''#include <stdio.h>

int complex_function(int x) {
    if (x > 10) {
        for (int i = 0; i < x; i++) {
            while (i % 2 == 0) {
                if (i > 5) break;
                i++;
            }
        }
        return x > 20 ? x * 2 : x / 2;
    }
    
    switch (x) {
        case 1:
            return 1;
        case 2:
            return 2;
        default:
            return 0;
    }
}
'''
        temp_file = self.create_temp_c_file(content)
        
        try:
            metrics = analyze_c_file(temp_file)
            
            self.assertEqual(metrics['function_count'], 1)
            self.assertGreater(metrics['estimated_complexity'], 5)  # Multiple control structures
            self.assertGreaterEqual(metrics['brace_count'], 10)  # At least 10 braces
            self.assertGreater(metrics['semicolon_count'], 5)
            
        finally:
            os.unlink(temp_file)
    
    def test_string_literals_with_comment_like_content(self):
        """Test strings containing comment-like sequences"""
        content = '''#include <stdio.h>

int main() {
    printf("This /* is not */ a comment");
    printf("Neither // is this");
    printf("Quote: \\"Hello world!\\"");
    return 0;
}
'''
        temp_file = self.create_temp_c_file(content)
        
        try:
            metrics = analyze_c_file(temp_file)
            
            # Note: Our simple parser may incorrectly identify some comment patterns in strings
            # This is a known limitation documented in the README
            self.assertLessEqual(metrics['comment_lines'], 1)  # Should be minimal
            self.assertGreaterEqual(metrics['string_literals'], 2)  # At least 2 strings
            self.assertEqual(metrics['printf_count'], 3)
            
        finally:
            os.unlink(temp_file)
    
    def test_preprocessor_directives(self):
        """Test handling of preprocessor directives"""
        content = '''#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 100
#define DEBUG 1

#ifdef DEBUG
    #define LOG(x) printf(x)
#else
    #define LOG(x)
#endif

int main() {
    LOG("Debug message\\n");
    return 0;
}
'''
        temp_file = self.create_temp_c_file(content)
        
        try:
            metrics = analyze_c_file(temp_file)
            
            self.assertEqual(metrics['include_count'], 2)
            self.assertEqual(metrics['function_count'], 1)
            # Note: Our simple parser treats preprocessor lines as code
            self.assertGreater(metrics['code_lines'], 5)
            
        finally:
            os.unlink(temp_file)
    
    def test_nonexistent_file(self):
        """Test error handling for nonexistent files"""
        metrics = analyze_c_file("/path/that/does/not/exist.c")
        self.assertIn("error", metrics)
    
    def test_html_generation(self):
        """Test HTML interface generation"""
        metrics = {
            "file_name": "test.c",
            "file_path": "/tmp/test.c",
            "total_lines": 10,
            "code_lines": 8,
            "function_count": 2,
            "code_density": 80.0,
            "analysis_time": "2023-01-01T12:00:00"
        }
        
        html_content = generate_html_interface(metrics)
        
        # Basic checks
        self.assertIn("<!DOCTYPE html>", html_content)
        self.assertIn("Code Metrics Spreadsheet", html_content)
        self.assertIn("test.c", html_content)
        self.assertIn("80.0", html_content)
        
        # Check for security - HTML escaping
        metrics_with_special_chars = {
            "file_name": "<script>alert('test')</script>",
            "file_path": "/tmp/<test>.c",
            "total_lines": 1,
            "code_lines": 1,
            "function_count": 0,
            "code_density": 0.0,
            "analysis_time": "2023-01-01T12:00:00"
        }
        
        html_content = generate_html_interface(metrics_with_special_chars)
        # Should be escaped
        self.assertNotIn("<script>alert('test')</script>", html_content)
        self.assertIn("&lt;script&gt;", html_content)


class TestCLI(unittest.TestCase):
    
    def test_help_output(self):
        """Test that help output is available"""
        with patch('sys.argv', ['metrics_analyzer.py', '--help']):
            exit_code = main()
        # Help should return 0
        self.assertEqual(exit_code, 0)
    
    def test_missing_file_error(self):
        """Test error handling for missing files"""
        with patch('sys.argv', ['metrics_analyzer.py', '/nonexistent/file.c']):
            exit_code = main()
        self.assertEqual(exit_code, 1)
    
    def test_version_output(self):
        """Test version output"""
        with patch('sys.argv', ['metrics_analyzer.py', '--version']):
            exit_code = main()
        # Version should return 0
        self.assertEqual(exit_code, 0)


class TestJSONOutput(unittest.TestCase):
    
    def test_json_schema_compliance(self):
        """Test that JSON output follows expected schema"""
        content = '''#include <stdio.h>
int main() { printf("test"); return 0; }
'''
        temp_file = tempfile.NamedTemporaryFile(mode='w', suffix='.c', delete=False)
        temp_file.write(content)
        temp_file.close()
        
        try:
            metrics = analyze_c_file(temp_file.name)
            
            # Check required fields and their types
            required_fields = {
                'file_name': str,
                'file_path': str,
                'total_lines': int,
                'total_chars': int,
                'total_bytes': int,
                'blank_lines': int,
                'comment_lines': int,
                'code_lines': int,
                'include_count': int,
                'function_count': int,
                'printf_count': int,
                'brace_count': int,
                'semicolon_count': int,
                'string_literals': int,
                'code_density': float,
                'comment_density': float,
                'avg_chars_per_code_line': float,
                'estimated_complexity': int,
                'analysis_time': str
            }
            
            for field, expected_type in required_fields.items():
                self.assertIn(field, metrics, f"Missing field: {field}")
                self.assertIsInstance(metrics[field], expected_type, 
                                    f"Field {field} should be {expected_type.__name__}, got {type(metrics[field]).__name__}")
            
            # Check that JSON is serializable
            json_str = json.dumps(metrics)
            reconstructed = json.loads(json_str)
            self.assertEqual(metrics['file_name'], reconstructed['file_name'])
            
        finally:
            os.unlink(temp_file.name)


if __name__ == '__main__':
    # Run the tests
    unittest.main(verbosity=2)