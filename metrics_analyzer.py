#!/usr/bin/env python3
"""
Code Metrics Analyzer for C files
Analyzes hello.c and generates metrics for the spreadsheet interface
"""

import os
import re
import json
from pathlib import Path

def analyze_c_file(file_path):
    """Analyze a C file and extract various code metrics"""
    
    if not os.path.exists(file_path):
        raise FileNotFoundError(f"File {file_path} not found")
    
    with open(file_path, 'r') as f:
        content = f.read()
        lines = content.split('\n')
    
    metrics = {}
    
    # Basic line counts
    total_lines = len(lines)
    blank_lines = len([line for line in lines if line.strip() == ''])
    comment_lines = count_comment_lines(lines)
    code_lines = total_lines - blank_lines - comment_lines
    
    # File info
    file_size = os.path.getsize(file_path)
    
    # Function analysis
    functions = extract_functions(content)
    function_count = len(functions)
    
    # Include analysis
    includes = extract_includes(lines)
    include_count = len(includes)
    
    # Variable analysis
    variables = extract_variables(content)
    variable_count = len(variables)
    
    # Complexity analysis
    cyclomatic_complexity = calculate_cyclomatic_complexity(content)
    
    # Character and word counts
    char_count = len(content)
    word_count = len(content.split())
    
    # Compile the metrics
    metrics = {
        'file_name': os.path.basename(file_path),
        'file_path': file_path,
        'file_size_bytes': file_size,
        'total_lines': total_lines,
        'code_lines': code_lines,
        'blank_lines': blank_lines,
        'comment_lines': comment_lines,
        'character_count': char_count,
        'word_count': word_count,
        'function_count': function_count,
        'functions': functions,
        'include_count': include_count,
        'includes': includes,
        'variable_count': variable_count,
        'variables': variables,
        'cyclomatic_complexity': cyclomatic_complexity
    }
    
    return metrics

def count_comment_lines(lines):
    """Count lines that contain comments"""
    comment_count = 0
    in_multiline_comment = False
    
    for line in lines:
        line = line.strip()
        if '/*' in line:
            in_multiline_comment = True
        if in_multiline_comment:
            comment_count += 1
        elif line.startswith('//'):
            comment_count += 1
        if '*/' in line:
            in_multiline_comment = False
    
    return comment_count

def extract_functions(content):
    """Extract function names and their line counts"""
    # Simple regex to find function definitions
    function_pattern = r'(\w+)\s+(\w+)\s*\([^)]*\)\s*\{'
    functions = []
    
    matches = re.finditer(function_pattern, content)
    for match in matches:
        return_type = match.group(1)
        function_name = match.group(2)
        
        # Count lines in function (simplified - until next function or end)
        start_pos = match.start()
        lines_before = content[:start_pos].count('\n')
        
        # Find matching closing brace (simplified)
        brace_count = 0
        pos = match.end() - 1
        function_end = len(content)
        
        for i, char in enumerate(content[pos:], pos):
            if char == '{':
                brace_count += 1
            elif char == '}':
                brace_count -= 1
                if brace_count == 0:
                    function_end = i
                    break
        
        lines_in_function = content[match.start():function_end+1].count('\n') + 1
        
        functions.append({
            'name': function_name,
            'return_type': return_type,
            'line_start': lines_before + 1,
            'line_count': lines_in_function
        })
    
    return functions

def extract_includes(lines):
    """Extract include statements"""
    includes = []
    include_pattern = r'#\s*include\s*[<"]([^>"]+)[>"]'
    
    for i, line in enumerate(lines):
        match = re.search(include_pattern, line.strip())
        if match:
            includes.append({
                'header': match.group(1),
                'line_number': i + 1,
                'full_statement': line.strip()
            })
    
    return includes

def extract_variables(content):
    """Extract variable declarations (simplified)"""
    variables = []
    # Simple pattern for variable declarations
    var_patterns = [
        r'\b(int|char|float|double|void)\s+(\w+)',
        r'\b(const\s+\w+)\s+(\w+)',
    ]
    
    for pattern in var_patterns:
        matches = re.finditer(pattern, content)
        for match in matches:
            # Skip function definitions
            if '(' not in content[match.end():match.end()+10]:
                var_type = match.group(1)
                var_name = match.group(2)
                variables.append({
                    'name': var_name,
                    'type': var_type
                })
    
    return variables

def calculate_cyclomatic_complexity(content):
    """Calculate cyclomatic complexity (simplified)"""
    # Count decision points: if, while, for, switch, case, &&, ||
    decision_points = 0
    
    patterns = [
        r'\bif\s*\(',
        r'\bwhile\s*\(',
        r'\bfor\s*\(',
        r'\bswitch\s*\(',
        r'\bcase\s+',
        r'\&\&',
        r'\|\|'
    ]
    
    for pattern in patterns:
        decision_points += len(re.findall(pattern, content))
    
    # Cyclomatic complexity = decision points + 1
    return decision_points + 1

def main():
    """Main function to analyze hello.c and output metrics"""
    script_dir = Path(__file__).parent
    hello_c_path = script_dir / 'hello.c'
    
    try:
        metrics = analyze_c_file(str(hello_c_path))
        
        # Output as JSON for the web interface
        output_path = script_dir / 'metrics.json'
        with open(output_path, 'w') as f:
            json.dump(metrics, f, indent=2)
        
        print(f"Metrics analysis complete. Results saved to {output_path}")
        print(f"Analyzed file: {metrics['file_name']}")
        print(f"Lines of code: {metrics['code_lines']}")
        print(f"Functions: {metrics['function_count']}")
        print(f"Cyclomatic complexity: {metrics['cyclomatic_complexity']}")
        
        return metrics
        
    except Exception as e:
        print(f"Error analyzing file: {e}")
        return None

if __name__ == '__main__':
    main()