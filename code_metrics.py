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
    """Analyze a C file and extract various code metrics"""
    if not os.path.exists(filepath):
        return None
    
    with open(filepath, 'r') as file:
        content = file.read()
        lines = content.splitlines()
    
    metrics = {
        'file_name': os.path.basename(filepath),
        'analysis_date': datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
        'total_lines': len(lines),
        'blank_lines': 0,
        'comment_lines': 0,
        'code_lines': 0,
        'total_characters': len(content),
        'functions': [],
        'includes': [],
        'keywords': {},
        'cyclomatic_complexity': 1,  # Base complexity
        'max_line_length': 0,
        'avg_line_length': 0
    }
    
    # C keywords to count
    c_keywords = [
        'auto', 'break', 'case', 'char', 'const', 'continue', 'default', 'do',
        'double', 'else', 'enum', 'extern', 'float', 'for', 'goto', 'if',
        'int', 'long', 'register', 'return', 'short', 'signed', 'sizeof',
        'static', 'struct', 'switch', 'typedef', 'union', 'unsigned', 'void',
        'volatile', 'while'
    ]
    
    for keyword in c_keywords:
        metrics['keywords'][keyword] = 0
    
    in_multiline_comment = False
    
    for i, line in enumerate(lines, 1):
        stripped = line.strip()
        
        # Calculate line length metrics
        line_length = len(line)
        if line_length > metrics['max_line_length']:
            metrics['max_line_length'] = line_length
        
        # Check for blank lines
        if not stripped:
            metrics['blank_lines'] += 1
            continue
        
        # Handle multiline comments
        if '/*' in line:
            in_multiline_comment = True
        if '*/' in line:
            in_multiline_comment = False
            if stripped.startswith('/*') and stripped.endswith('*/'):
                metrics['comment_lines'] += 1
                continue
        
        # Check for comment lines
        if stripped.startswith('//') or in_multiline_comment:
            metrics['comment_lines'] += 1
            continue
        
        # This is a code line
        metrics['code_lines'] += 1
        
        # Find includes
        include_match = re.search(r'#\s*include\s*[<"]([^>"]+)[>"]', line)
        if include_match:
            metrics['includes'].append({
                'header': include_match.group(1),
                'line_number': i,
                'type': 'system' if '<' in line else 'local'
            })
        
        # Find function definitions
        func_match = re.search(r'^\s*(\w+)\s+(\w+)\s*\([^)]*\)\s*\{?', line)
        if func_match and not line.strip().startswith('//'):
            return_type = func_match.group(1)
            func_name = func_match.group(2)
            # Filter out obvious non-functions
            if return_type in ['int', 'void', 'char', 'float', 'double', 'long', 'short']:
                metrics['functions'].append({
                    'name': func_name,
                    'return_type': return_type,
                    'line_number': i
                })
        
        # Count keywords
        words = re.findall(r'\b\w+\b', line.lower())
        for word in words:
            if word in metrics['keywords']:
                metrics['keywords'][word] += 1
        
        # Calculate cyclomatic complexity (count decision points)
        complexity_keywords = ['if', 'else', 'while', 'for', 'case', 'catch', '&&', '||', '?']
        for keyword in complexity_keywords:
            metrics['cyclomatic_complexity'] += line.lower().count(keyword)
    
    # Calculate average line length
    if metrics['total_lines'] > 0:
        metrics['avg_line_length'] = round(sum(len(line) for line in lines) / metrics['total_lines'], 2)
    
    # Calculate additional derived metrics
    metrics['code_density'] = round((metrics['code_lines'] / metrics['total_lines']) * 100, 2) if metrics['total_lines'] > 0 else 0
    metrics['comment_density'] = round((metrics['comment_lines'] / metrics['total_lines']) * 100, 2) if metrics['total_lines'] > 0 else 0
    
    return metrics

def generate_metrics_json(c_file='hello.c', output_file='metrics.json'):
    """Generate metrics JSON file for the spreadsheet interface"""
    metrics = analyze_c_file(c_file)
    
    if metrics is None:
        print(f"Error: Could not analyze {c_file}")
        return False
    
    # Create a flattened structure for spreadsheet display
    spreadsheet_data = []
    
    # Basic file metrics
    basic_metrics = [
        {'metric': 'File Name', 'value': metrics['file_name'], 'category': 'File Info', 'unit': ''},
        {'metric': 'Analysis Date', 'value': metrics['analysis_date'], 'category': 'File Info', 'unit': ''},
        {'metric': 'Total Lines', 'value': metrics['total_lines'], 'category': 'Size', 'unit': 'lines'},
        {'metric': 'Code Lines', 'value': metrics['code_lines'], 'category': 'Size', 'unit': 'lines'},
        {'metric': 'Comment Lines', 'value': metrics['comment_lines'], 'category': 'Size', 'unit': 'lines'},
        {'metric': 'Blank Lines', 'value': metrics['blank_lines'], 'category': 'Size', 'unit': 'lines'},
        {'metric': 'Total Characters', 'value': metrics['total_characters'], 'category': 'Size', 'unit': 'chars'},
        {'metric': 'Code Density', 'value': metrics['code_density'], 'category': 'Quality', 'unit': '%'},
        {'metric': 'Comment Density', 'value': metrics['comment_density'], 'category': 'Quality', 'unit': '%'},
        {'metric': 'Cyclomatic Complexity', 'value': metrics['cyclomatic_complexity'], 'category': 'Complexity', 'unit': ''},
        {'metric': 'Max Line Length', 'value': metrics['max_line_length'], 'category': 'Style', 'unit': 'chars'},
        {'metric': 'Average Line Length', 'value': metrics['avg_line_length'], 'category': 'Style', 'unit': 'chars'},
        {'metric': 'Number of Functions', 'value': len(metrics['functions']), 'category': 'Structure', 'unit': ''},
        {'metric': 'Number of Includes', 'value': len(metrics['includes']), 'category': 'Structure', 'unit': ''},
    ]
    
    spreadsheet_data.extend(basic_metrics)
    
    # Function details
    for func in metrics['functions']:
        spreadsheet_data.append({
            'metric': f"Function: {func['name']}",
            'value': f"{func['return_type']} (line {func['line_number']})",
            'category': 'Functions',
            'unit': ''
        })
    
    # Include details
    for inc in metrics['includes']:
        spreadsheet_data.append({
            'metric': f"Include: {inc['header']}",
            'value': f"{inc['type']} (line {inc['line_number']})",
            'category': 'Includes',
            'unit': ''
        })
    
    # Keyword usage (only non-zero counts)
    for keyword, count in metrics['keywords'].items():
        if count > 0:
            spreadsheet_data.append({
                'metric': f"Keyword: {keyword}",
                'value': count,
                'category': 'Keywords',
                'unit': 'occurrences'
            })
    
    output_data = {
        'raw_metrics': metrics,
        'spreadsheet_data': spreadsheet_data,
        'summary': {
            'total_metrics': len(spreadsheet_data),
            'categories': list(set(item['category'] for item in spreadsheet_data))
        }
    }
    
    with open(output_file, 'w') as f:
        json.dump(output_data, f, indent=2)
    
    print(f"Metrics generated and saved to {output_file}")
    return True

if __name__ == '__main__':
    import sys
    
    c_file = sys.argv[1] if len(sys.argv) > 1 else 'hello.c'
    output_file = sys.argv[2] if len(sys.argv) > 2 else 'metrics.json'
    
    generate_metrics_json(c_file, output_file)