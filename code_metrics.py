#!/usr/bin/env python3
"""
Code Metrics Analyzer for C files
Analyzes hello.c and generates metrics for spreadsheet display
"""

import re
import json
import os
from datetime import datetime

def analyze_c_file(filepath):
    """
    Analyze a C file and extract various code metrics.
    
    Args:
        filepath (str): Path to the C source file to analyze
        
    Returns:
        dict: Dictionary containing comprehensive code metrics, or None if file doesn't exist
        
    The function analyzes multiple aspects of C code:
    - Size metrics (lines, characters)
    - Code quality metrics (density, comments)
    - Complexity metrics (cyclomatic complexity)
    - Structure metrics (functions, includes)
    - Style metrics (line lengths)
    - Keyword usage statistics
    """
    # Validate file existence before processing
    if not os.path.exists(filepath):
        return None
    
    # Read the entire file content for analysis
    with open(filepath, 'r') as file:
        content = file.read()
        lines = content.splitlines()
    
    # Initialize the metrics dictionary with default values
    # This will store all calculated metrics for the file
    metrics = {
        'file_name': os.path.basename(filepath),
        'analysis_date': datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
        'total_lines': len(lines),
        'blank_lines': 0,
        'comment_lines': 0,
        'code_lines': 0,
        'total_characters': len(content),
        'functions': [],        # List of function definitions found
        'includes': [],         # List of include statements
        'keywords': {},         # Count of C language keywords
        'cyclomatic_complexity': 1,  # Base complexity starts at 1
        'max_line_length': 0,
        'avg_line_length': 0
    }
    
    # Define standard C language keywords for analysis
    # These keywords are counted to understand language feature usage
    c_keywords = [
        'auto', 'break', 'case', 'char', 'const', 'continue', 'default', 'do',
        'double', 'else', 'enum', 'extern', 'float', 'for', 'goto', 'if',
        'int', 'long', 'register', 'return', 'short', 'signed', 'sizeof',
        'static', 'struct', 'switch', 'typedef', 'union', 'unsigned', 'void',
        'volatile', 'while'
    ]
    
    # Initialize keyword counters to zero
    for keyword in c_keywords:
        metrics['keywords'][keyword] = 0
    
    # Track state for multiline comment parsing
    in_multiline_comment = False
    
    # Process each line to extract metrics
    for i, line in enumerate(lines, 1):
        stripped = line.strip()
        
        # Track line length statistics for style analysis
        line_length = len(line)
        if line_length > metrics['max_line_length']:
            metrics['max_line_length'] = line_length
        
        # Count blank lines (lines with no content)
        if not stripped:
            metrics['blank_lines'] += 1
            continue
        
        # Handle multiline comments (/* ... */)
        # This logic tracks when we're inside a multiline comment block
        if '/*' in line:
            in_multiline_comment = True
        if '*/' in line:
            in_multiline_comment = False
            # Handle single-line /* comment */ style
            if stripped.startswith('/*') and stripped.endswith('*/'):
                metrics['comment_lines'] += 1
                continue
        
        # Classify comment lines vs code lines
        # Single-line comments (//) or lines inside multiline comments
        if stripped.startswith('//') or in_multiline_comment:
            metrics['comment_lines'] += 1
            continue
        
        # If we reach here, this is an actual code line
        metrics['code_lines'] += 1
        
        # Parse include statements using regex pattern
        # Matches both #include <stdio.h> and #include "myheader.h" formats
        include_match = re.search(r'#\s*include\s*[<"]([^>"]+)[>"]', line)
        if include_match:
            metrics['includes'].append({
                'header': include_match.group(1),  # Extract header filename
                'line_number': i,
                'type': 'system' if '<' in line else 'local'  # Determine include type
            })
        
        # Parse function definitions using regex pattern
        # Matches patterns like: int main(void) or void function_name(params)
        func_match = re.search(r'^\s*(\w+)\s+(\w+)\s*\([^)]*\)\s*\{?', line)
        if func_match and not line.strip().startswith('//'):
            return_type = func_match.group(1)
            func_name = func_match.group(2)
            # Filter to only include actual function definitions (not variable declarations)
            if return_type in ['int', 'void', 'char', 'float', 'double', 'long', 'short']:
                metrics['functions'].append({
                    'name': func_name,
                    'return_type': return_type,
                    'line_number': i
                })
        
        # Count occurrences of C keywords in the line
        # Extract all word tokens and check against our keyword list
        words = re.findall(r'\b\w+\b', line.lower())
        for word in words:
            if word in metrics['keywords']:
                metrics['keywords'][word] += 1
        
        # Calculate cyclomatic complexity by counting decision points
        # Each branching construct increases the complexity score
        complexity_keywords = ['if', 'else', 'while', 'for', 'case', 'catch', '&&', '||', '?']
        for keyword in complexity_keywords:
            metrics['cyclomatic_complexity'] += line.lower().count(keyword)
    
    # Calculate derived metrics from the raw counts
    # Average line length across all lines (including blank lines)
    if metrics['total_lines'] > 0:
        metrics['avg_line_length'] = round(sum(len(line) for line in lines) / metrics['total_lines'], 2)
    
    # Calculate percentage-based quality metrics
    # Code density: percentage of lines that contain actual code
    metrics['code_density'] = round((metrics['code_lines'] / metrics['total_lines']) * 100, 2) if metrics['total_lines'] > 0 else 0
    # Comment density: percentage of lines that contain comments
    metrics['comment_density'] = round((metrics['comment_lines'] / metrics['total_lines']) * 100, 2) if metrics['total_lines'] > 0 else 0
    
    return metrics

def generate_metrics_json(c_file='hello.c', output_file='metrics.json'):
    """
    Generate metrics JSON file for the spreadsheet interface.
    
    Args:
        c_file (str): Path to the C source file to analyze (default: 'hello.c')
        output_file (str): Path where the JSON output will be saved (default: 'metrics.json')
        
    Returns:
        bool: True if successful, False if analysis failed
        
    This function:
    1. Analyzes the C file using analyze_c_file()
    2. Transforms the raw metrics into a spreadsheet-friendly format
    3. Creates categorized metric entries for the web interface
    4. Saves the structured data as JSON for the HTML interface to consume
    """
    # Analyze the C file to get raw metrics
    metrics = analyze_c_file(c_file)
    
    # Handle file analysis errors
    if metrics is None:
        print(f"Error: Could not analyze {c_file}")
        return False
    
    # Create a flattened structure optimized for spreadsheet display
    # Each metric becomes a row with: name, value, category, and unit
    spreadsheet_data = []
    
    # Define basic file and code metrics organized by logical categories
    # These metrics provide fundamental information about the code structure and quality
    basic_metrics = [
        # File identification and metadata
        {'metric': 'File Name', 'value': metrics['file_name'], 'category': 'File Info', 'unit': ''},
        {'metric': 'Analysis Date', 'value': metrics['analysis_date'], 'category': 'File Info', 'unit': ''},
        
        # Size-related metrics (volume of code)
        {'metric': 'Total Lines', 'value': metrics['total_lines'], 'category': 'Size', 'unit': 'lines'},
        {'metric': 'Code Lines', 'value': metrics['code_lines'], 'category': 'Size', 'unit': 'lines'},
        {'metric': 'Comment Lines', 'value': metrics['comment_lines'], 'category': 'Size', 'unit': 'lines'},
        {'metric': 'Blank Lines', 'value': metrics['blank_lines'], 'category': 'Size', 'unit': 'lines'},
        {'metric': 'Total Characters', 'value': metrics['total_characters'], 'category': 'Size', 'unit': 'chars'},
        
        # Quality metrics (code maintainability indicators)
        {'metric': 'Code Density', 'value': metrics['code_density'], 'category': 'Quality', 'unit': '%'},
        {'metric': 'Comment Density', 'value': metrics['comment_density'], 'category': 'Quality', 'unit': '%'},
        
        # Complexity metrics (code complexity indicators)
        {'metric': 'Cyclomatic Complexity', 'value': metrics['cyclomatic_complexity'], 'category': 'Complexity', 'unit': ''},
        
        # Style metrics (code formatting and readability)
        {'metric': 'Max Line Length', 'value': metrics['max_line_length'], 'category': 'Style', 'unit': 'chars'},
        {'metric': 'Average Line Length', 'value': metrics['avg_line_length'], 'category': 'Style', 'unit': 'chars'},
        
        # Structure metrics (code organization)
        {'metric': 'Number of Functions', 'value': len(metrics['functions']), 'category': 'Structure', 'unit': ''},
        {'metric': 'Number of Includes', 'value': len(metrics['includes']), 'category': 'Structure', 'unit': ''},
    ]
    
    # Add the basic metrics to our spreadsheet data
    spreadsheet_data.extend(basic_metrics)
    
    # Add detailed function information as separate metrics
    # Each function found in the code becomes its own metric entry
    for func in metrics['functions']:
        spreadsheet_data.append({
            'metric': f"Function: {func['name']}",
            'value': f"{func['return_type']} (line {func['line_number']})",
            'category': 'Functions',
            'unit': ''
        })
    
    # Add detailed include information as separate metrics
    # Each include statement becomes its own metric entry
    for inc in metrics['includes']:
        spreadsheet_data.append({
            'metric': f"Include: {inc['header']}",
            'value': f"{inc['type']} (line {inc['line_number']})",
            'category': 'Includes',
            'unit': ''
        })
    
    # Add keyword usage statistics (only for keywords that appear in the code)
    # This helps understand which C language features are being used
    for keyword, count in metrics['keywords'].items():
        if count > 0:  # Only include keywords that actually appear
            spreadsheet_data.append({
                'metric': f"Keyword: {keyword}",
                'value': count,
                'category': 'Keywords',
                'unit': 'occurrences'
            })
    
    # Create the final output structure with multiple data views
    # This provides both raw metrics and formatted data for the web interface
    output_data = {
        'raw_metrics': metrics,           # Original metrics object for programmatic use
        'spreadsheet_data': spreadsheet_data,  # Formatted data for the table display
        'summary': {                      # Summary statistics for dashboard
            'total_metrics': len(spreadsheet_data),
            'categories': list(set(item['category'] for item in spreadsheet_data))
        }
    }
    
    # Save the structured data as JSON for the web interface to consume
    with open(output_file, 'w') as f:
        json.dump(output_data, f, indent=2)
    
    print(f"Metrics generated and saved to {output_file}")
    return True

# Main execution block - handles command line arguments and runs the analysis
if __name__ == '__main__':
    import sys
    
    # Parse command line arguments with defaults
    # Usage: python3 code_metrics.py [input_file] [output_file]
    c_file = sys.argv[1] if len(sys.argv) > 1 else 'hello.c'        # Source file to analyze
    output_file = sys.argv[2] if len(sys.argv) > 2 else 'metrics.json'  # Output JSON file
    
    # Run the metrics generation process
    generate_metrics_json(c_file, output_file)