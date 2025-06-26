#!/usr/bin/env python3
"""
Code Metrics Analyzer for C files
Analyzes code metrics and displays them in a spreadsheet-like format
"""

import os
import re
import json
from datetime import datetime

def analyze_c_file(filepath):
    """Analyze a C file and extract various code metrics"""
    if not os.path.exists(filepath):
        return None
    
    with open(filepath, 'r', encoding='utf-8') as file:
        content = file.read()
        lines = content.split('\n')
    
    metrics = {
        'filename': os.path.basename(filepath),
        'total_lines': len(lines),
        'file_size_bytes': len(content),
        'timestamp': datetime.now().isoformat()
    }
    
    # Count non-empty lines
    non_empty_lines = [line for line in lines if line.strip()]
    metrics['non_empty_lines'] = len(non_empty_lines)
    
    # Count comment lines
    comment_lines = []
    in_multiline_comment = False
    for line in lines:
        line_stripped = line.strip()
        if '/*' in line_stripped and '*/' in line_stripped:
            comment_lines.append(line)
        elif '/*' in line_stripped:
            in_multiline_comment = True
            comment_lines.append(line)
        elif '*/' in line_stripped and in_multiline_comment:
            in_multiline_comment = False
            comment_lines.append(line)
        elif in_multiline_comment:
            comment_lines.append(line)
        elif line_stripped.startswith('//'):
            comment_lines.append(line)
    
    metrics['comment_lines'] = len(comment_lines)
    metrics['code_lines'] = metrics['non_empty_lines'] - metrics['comment_lines']
    
    # Count includes
    includes = re.findall(r'#\s*include\s*[<"][^>"]*[>"]', content)
    metrics['include_statements'] = len(includes)
    
    # Count functions (simple pattern matching)
    # Look for function definitions (return_type function_name(...) {)
    function_pattern = r'\b\w+\s+\w+\s*\([^)]*\)\s*\{'
    functions = re.findall(function_pattern, content)
    metrics['function_count'] = len(functions)
    
    # Count printf statements
    printf_pattern = r'printf\s*\('
    printf_calls = re.findall(printf_pattern, content)
    metrics['printf_statements'] = len(printf_calls)
    
    # Count variable declarations (simple heuristic)
    # Look for common C types followed by variable names
    var_pattern = r'\b(int|char|float|double|long|short|void\s*\*)\s+\w+'
    variables = re.findall(var_pattern, content)
    metrics['variable_declarations'] = len(variables)
    
    # Simple cyclomatic complexity (count decision points)
    complexity_keywords = ['if', 'else', 'while', 'for', 'switch', 'case', '&&', '||', '?']
    complexity = 1  # Base complexity
    for keyword in complexity_keywords:
        if keyword in ['&&', '||', '?']:
            complexity += content.count(keyword)
        else:
            complexity += len(re.findall(r'\b' + keyword + r'\b', content))
    
    metrics['cyclomatic_complexity'] = complexity
    
    # Count braces (indicators of code blocks)
    metrics['opening_braces'] = content.count('{')
    metrics['closing_braces'] = content.count('}')
    metrics['semicolons'] = content.count(';')
    
    return metrics

def display_metrics_table(metrics_list):
    """Display metrics in a tabular format"""
    if not metrics_list:
        print("No metrics to display")
        return
    
    # Define column headers and their display names
    columns = [
        ('filename', 'File Name'),
        ('total_lines', 'Total Lines'),
        ('code_lines', 'Code Lines'),
        ('comment_lines', 'Comment Lines'),
        ('non_empty_lines', 'Non-Empty Lines'),
        ('file_size_bytes', 'File Size (bytes)'),
        ('include_statements', 'Includes'),
        ('function_count', 'Functions'),
        ('variable_declarations', 'Variables'),
        ('printf_statements', 'Printf Calls'),
        ('cyclomatic_complexity', 'Complexity'),
        ('opening_braces', 'Opening Braces'),
        ('closing_braces', 'Closing Braces'),
        ('semicolons', 'Semicolons')
    ]
    
    # Calculate column widths
    col_widths = {}
    for col_key, col_name in columns:
        max_width = len(col_name)
        for metrics in metrics_list:
            if col_key in metrics:
                max_width = max(max_width, len(str(metrics[col_key])))
        col_widths[col_key] = max_width + 2
    
    # Print header
    header = "|"
    separator = "|"
    for col_key, col_name in columns:
        header += f" {col_name:<{col_widths[col_key]-1}}|"
        separator += "-" * col_widths[col_key] + "|"
    
    print(header)
    print(separator)
    
    # Print data rows
    for metrics in metrics_list:
        row = "|"
        for col_key, _ in columns:
            value = metrics.get(col_key, 'N/A')
            row += f" {str(value):<{col_widths[col_key]-1}}|"
        print(row)
    
    print(separator)

def export_to_json(metrics_list, filename='code_metrics.json'):
    """Export metrics to JSON file for use by HTML interface"""
    with open(filename, 'w') as f:
        json.dump(metrics_list, f, indent=2)
    print(f"Metrics exported to {filename}")

def main():
    """Main function to analyze C files and display metrics"""
    # Find all C files in current directory
    c_files = [f for f in os.listdir('.') if f.endswith('.c')]
    
    if not c_files:
        print("No C files found in current directory")
        return
    
    print("Code Metrics Analyzer")
    print("=" * 50)
    print()
    
    metrics_list = []
    for c_file in c_files:
        print(f"Analyzing {c_file}...")
        metrics = analyze_c_file(c_file)
        if metrics:
            metrics_list.append(metrics)
    
    if metrics_list:
        print(f"\nAnalysis complete. Found {len(metrics_list)} file(s).")
        print("\nCode Metrics Table:")
        print("=" * 120)
        display_metrics_table(metrics_list)
        
        # Export to JSON for HTML interface
        export_to_json(metrics_list)
        
        print(f"\nSummary:")
        print(f"- Total files analyzed: {len(metrics_list)}")
        total_lines = sum(m.get('total_lines', 0) for m in metrics_list)
        total_code_lines = sum(m.get('code_lines', 0) for m in metrics_list)
        print(f"- Total lines of code: {total_code_lines}")
        print(f"- Total lines (including empty/comments): {total_lines}")
    else:
        print("No files could be analyzed")

if __name__ == "__main__":
    main()