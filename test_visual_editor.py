#!/usr/bin/env python3
"""
Comprehensive test suite for the Visual C Block Editor
Tests server functionality, code generation, and compilation
"""

import unittest
import requests
import subprocess
import time
import os
import threading
import json
import tempfile
from pathlib import Path
import sys

# Add server module to path
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from server import CodeValidator, BlockEditorHandler

class TestCodeValidator(unittest.TestCase):
    """Test the input validation functions"""
    
    def test_validate_header_name(self):
        """Test header name validation"""
        # Valid headers
        self.assertTrue(CodeValidator.validate_header_name("stdio.h"))
        self.assertTrue(CodeValidator.validate_header_name("stdlib.h"))
        self.assertTrue(CodeValidator.validate_header_name("string.h"))
        self.assertTrue(CodeValidator.validate_header_name("sys/types.h"))
        self.assertTrue(CodeValidator.validate_header_name("custom_header.h"))
        
        # Invalid headers
        self.assertFalse(CodeValidator.validate_header_name(""))
        self.assertFalse(CodeValidator.validate_header_name("../../../etc/passwd"))
        self.assertFalse(CodeValidator.validate_header_name("header with spaces.h"))
        self.assertFalse(CodeValidator.validate_header_name("header;rm -rf /"))
        self.assertFalse(CodeValidator.validate_header_name("a" * 101))  # Too long
    
    def test_validate_identifier(self):
        """Test C identifier validation"""
        # Valid identifiers
        self.assertTrue(CodeValidator.validate_identifier("variable"))
        self.assertTrue(CodeValidator.validate_identifier("_private"))
        self.assertTrue(CodeValidator.validate_identifier("var123"))
        self.assertTrue(CodeValidator.validate_identifier("MyVariable"))
        
        # Invalid identifiers
        self.assertFalse(CodeValidator.validate_identifier(""))
        self.assertFalse(CodeValidator.validate_identifier("123var"))  # Starts with number
        self.assertFalse(CodeValidator.validate_identifier("var-name"))  # Contains hyphen
        self.assertFalse(CodeValidator.validate_identifier("var name"))  # Contains space
        self.assertFalse(CodeValidator.validate_identifier("a" * 51))  # Too long
    
    def test_escape_string_literal(self):
        """Test string literal escaping"""
        # Basic strings
        self.assertEqual(CodeValidator.escape_string_literal("Hello"), "Hello")
        self.assertEqual(CodeValidator.escape_string_literal("Hello World"), "Hello World")
        
        # Strings with quotes
        self.assertEqual(CodeValidator.escape_string_literal('Say "Hello"'), 'Say \\"Hello\\"')
        
        # Strings with backslashes
        self.assertEqual(CodeValidator.escape_string_literal("Path\\to\\file"), "Path\\\\to\\\\file")
        
        # Special characters
        self.assertEqual(CodeValidator.escape_string_literal("Line1\nLine2"), "Line1\\nLine2")
        self.assertEqual(CodeValidator.escape_string_literal("Tab\there"), "Tab\\there")
        
        # Long strings
        long_string = "a" * 1001
        escaped = CodeValidator.escape_string_literal(long_string)
        self.assertTrue(len(escaped) <= 1003)  # Should be truncated with "..."
        self.assertTrue(escaped.endswith("..."))
    
    def test_validate_c_code(self):
        """Test C code validation"""
        # Valid C code
        valid_code = """#include <stdio.h>

int main(){
    printf("Hello world!");
}"""
        self.assertTrue(CodeValidator.validate_c_code(valid_code))
        
        # Code without main function
        invalid_code1 = """#include <stdio.h>

void test(){
    printf("Hello");
}"""
        self.assertFalse(CodeValidator.validate_c_code(invalid_code1))
        
        # Code with unbalanced braces
        invalid_code2 = """#include <stdio.h>

int main(){
    printf("Hello");
"""
        self.assertFalse(CodeValidator.validate_c_code(invalid_code2))
        
        # Empty or too long code
        self.assertFalse(CodeValidator.validate_c_code(""))
        self.assertFalse(CodeValidator.validate_c_code("a" * 10001))


class TestServerIntegration(unittest.TestCase):
    """Test the HTTP server functionality"""
    
    @classmethod
    def setUpClass(cls):
        """Start the server in a separate thread"""
        cls.server_process = None
        cls.server_port = 8082  # Use different port for testing
        cls.base_url = f"http://localhost:{cls.server_port}"
        
        # Start server in background
        def run_server():
            import socketserver
            from server import BlockEditorHandler
            with socketserver.TCPServer(("localhost", cls.server_port), BlockEditorHandler) as httpd:
                cls.server_instance = httpd
                httpd.serve_forever()
        
        cls.server_thread = threading.Thread(target=run_server, daemon=True)
        cls.server_thread.start()
        
        # Wait for server to start
        time.sleep(1)
    
    def test_export_valid_code(self):
        """Test exporting valid C code"""
        valid_code = """#include <stdio.h>

int main(){
    printf("Hello world!");
}"""
        
        response = requests.post(f"{self.base_url}/export", 
                               json={"code": valid_code},
                               timeout=5)
        
        self.assertEqual(response.status_code, 201)
        data = response.json()
        self.assertTrue(data["success"])
        self.assertIn("hello.c", data["file_path"])
        
        # Verify file was written
        with open("hello.c", "r") as f:
            written_code = f.read()
        self.assertEqual(written_code, valid_code)
    
    def test_export_invalid_code(self):
        """Test exporting invalid C code"""
        invalid_code = "This is not C code"
        
        response = requests.post(f"{self.base_url}/export", 
                               json={"code": invalid_code},
                               timeout=5)
        
        self.assertEqual(response.status_code, 400)
        data = response.json()
        self.assertFalse(data["success"])
    
    def test_export_no_code(self):
        """Test exporting with no code"""
        response = requests.post(f"{self.base_url}/export", 
                               json={},
                               timeout=5)
        
        self.assertEqual(response.status_code, 400)
        data = response.json()
        self.assertFalse(data["success"])
    
    def test_export_invalid_json(self):
        """Test exporting with invalid JSON"""
        response = requests.post(f"{self.base_url}/export", 
                               data="invalid json",
                               headers={"Content-Type": "application/json"},
                               timeout=5)
        
        self.assertEqual(response.status_code, 400)
    
    def test_invalid_endpoint(self):
        """Test accessing invalid endpoint"""
        response = requests.post(f"{self.base_url}/invalid", 
                               json={"test": "data"},
                               timeout=5)
        
        self.assertEqual(response.status_code, 404)


class TestCodeGeneration(unittest.TestCase):
    """Test code generation and compilation"""
    
    def test_generated_code_compiles(self):
        """Test that generated code compiles successfully"""
        test_cases = [
            {
                "name": "Basic hello world",
                "code": """#include <stdio.h>

int main(){
    printf("Hello world!");
}""",
                "expected_output": "Hello world!"
            },
            {
                "name": "With escaped quotes",
                "code": """#include <stdio.h>

int main(){
    printf("Say \\"Hello\\"");
}""",
                "expected_output": 'Say "Hello"'
            },
            {
                "name": "With variables",
                "code": """#include <stdio.h>

int main(){
    int count = 42;
    char letter = 'A';
    printf("Count: %d, Letter: %c", count, letter);
}""",
                "expected_output": "Count: 42, Letter: A"
            }
        ]
        
        for test_case in test_cases:
            with self.subTest(test_case["name"]):
                # Write code to temporary file
                with tempfile.NamedTemporaryFile(mode='w', suffix='.c', delete=False) as f:
                    f.write(test_case["code"])
                    temp_c_file = f.name
                
                temp_exe_file = temp_c_file.replace('.c', '')
                
                try:
                    # Compile the code
                    compile_result = subprocess.run([
                        'gcc', '-Wall', '-Wextra', '-o', temp_exe_file, temp_c_file
                    ], capture_output=True, text=True)
                    
                    self.assertEqual(compile_result.returncode, 0, 
                                   f"Compilation failed for {test_case['name']}: {compile_result.stderr}")
                    
                    # Run the compiled program
                    run_result = subprocess.run([temp_exe_file], 
                                              capture_output=True, text=True)
                    
                    self.assertEqual(run_result.returncode, 0,
                                   f"Execution failed for {test_case['name']}: {run_result.stderr}")
                    
                    # Check output if specified
                    if "expected_output" in test_case:
                        self.assertEqual(run_result.stdout, test_case["expected_output"])
                
                finally:
                    # Clean up temporary files
                    for temp_file in [temp_c_file, temp_exe_file]:
                        if os.path.exists(temp_file):
                            os.unlink(temp_file)


def run_validation_tests():
    """Run the validation test suite"""
    # Save current directory
    original_dir = os.getcwd()
    
    try:
        # Change to script directory
        script_dir = os.path.dirname(os.path.abspath(__file__))
        os.chdir(script_dir)
        
        # Create test suite
        loader = unittest.TestLoader()
        suite = unittest.TestSuite()
        
        # Add test classes
        suite.addTests(loader.loadTestsFromTestCase(TestCodeValidator))
        suite.addTests(loader.loadTestsFromTestCase(TestCodeGeneration))
        
        # Only add server tests if requests is available
        try:
            import requests
            suite.addTests(loader.loadTestsFromTestCase(TestServerIntegration))
        except ImportError:
            print("Warning: requests module not available, skipping server integration tests")
        
        # Run tests
        runner = unittest.TextTestRunner(verbosity=2)
        result = runner.run(suite)
        
        return result.wasSuccessful()
    
    finally:
        os.chdir(original_dir)


if __name__ == "__main__":
    success = run_validation_tests()
    exit(0 if success else 1)