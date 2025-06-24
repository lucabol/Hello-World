#!/usr/bin/env python3
"""
Validation script for the code metrics spreadsheet solution
Tests that all components work correctly
"""

import os
import json
import sys
from pathlib import Path

def test_analyzer():
    """Test that the metrics analyzer works correctly"""
    print("🧪 Testing metrics analyzer...")
    
    # Import and run the analyzer
    try:
        import metrics_analyzer
        result = metrics_analyzer.main()
        
        if result is None:
            print("❌ Analyzer failed to return results")
            return False
            
        print("✅ Analyzer ran successfully")
        return True
        
    except Exception as e:
        print(f"❌ Analyzer failed: {e}")
        return False

def test_json_output():
    """Test that JSON output is valid and contains expected fields"""
    print("🧪 Testing JSON output...")
    
    json_path = "metrics.json"
    if not os.path.exists(json_path):
        print("❌ metrics.json not found")
        return False
    
    try:
        with open(json_path, 'r') as f:
            data = json.load(f)
        
        required_fields = [
            'file_name', 'total_lines', 'code_lines', 'function_count',
            'include_count', 'cyclomatic_complexity', 'functions', 'includes'
        ]
        
        for field in required_fields:
            if field not in data:
                print(f"❌ Missing required field: {field}")
                return False
        
        # Validate specific values for hello.c
        if data['file_name'] != 'hello.c':
            print("❌ Incorrect file name")
            return False
            
        if data['function_count'] != 1:
            print("❌ Incorrect function count")
            return False
            
        if data['include_count'] != 1:
            print("❌ Incorrect include count")
            return False
        
        print("✅ JSON output is valid")
        return True
        
    except Exception as e:
        print(f"❌ JSON validation failed: {e}")
        return False

def test_html_file():
    """Test that HTML file exists and contains expected content"""
    print("🧪 Testing HTML file...")
    
    html_path = "metrics.html"
    if not os.path.exists(html_path):
        print("❌ metrics.html not found")
        return False
    
    try:
        with open(html_path, 'r') as f:
            content = f.read()
        
        required_elements = [
            'Code Metrics Spreadsheet',
            'metricsTable',
            'searchInput',
            'sortTable',
            'filterTable',
            'loadMetrics'
        ]
        
        for element in required_elements:
            if element not in content:
                print(f"❌ Missing HTML element/function: {element}")
                return False
        
        print("✅ HTML file is valid")
        return True
        
    except Exception as e:
        print(f"❌ HTML validation failed: {e}")
        return False

def test_readme():
    """Test that README was updated with usage instructions"""
    print("🧪 Testing README update...")
    
    readme_path = "README.md"
    if not os.path.exists(readme_path):
        print("❌ README.md not found")
        return False
    
    try:
        with open(readme_path, 'r') as f:
            content = f.read()
        
        required_sections = [
            'Code Metrics Spreadsheet',
            'Usage',
            'Features',
            'metrics_analyzer.py',
            'metrics.html'
        ]
        
        for section in required_sections:
            if section not in content:
                print(f"❌ Missing README section: {section}")
                return False
        
        print("✅ README properly updated")
        return True
        
    except Exception as e:
        print(f"❌ README validation failed: {e}")
        return False

def main():
    """Run all validation tests"""
    print("🚀 Running Code Metrics Spreadsheet Validation\n")
    
    tests = [
        test_analyzer,
        test_json_output,
        test_html_file,
        test_readme
    ]
    
    passed = 0
    total = len(tests)
    
    for test in tests:
        if test():
            passed += 1
        print()
    
    print(f"📊 Test Results: {passed}/{total} tests passed")
    
    if passed == total:
        print("🎉 All tests passed! The code metrics spreadsheet is working correctly.")
        print("\n📋 To use the spreadsheet:")
        print("1. Run: python3 metrics_analyzer.py")
        print("2. Open: metrics.html in your web browser")
        return True
    else:
        print("❌ Some tests failed. Please check the implementation.")
        return False

if __name__ == '__main__':
    success = main()
    sys.exit(0 if success else 1)