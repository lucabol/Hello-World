#!/usr/bin/env python3
"""
Code Metrics Analyzer for C files
Analyzes hello.c and extracts various code metrics for display in a spreadsheet interface
"""

import re
import json
import os
from typing import Dict, Any


def analyze_c_file(filepath: str) -> Dict[str, Any]:
    """
    Analyze a C file and extract various code metrics
    
    Args:
        filepath: Path to the C file to analyze
        
    Returns:
        Dictionary containing various code metrics
    """
    if not os.path.exists(filepath):
        return {"error": f"File {filepath} not found"}
    
    with open(filepath, 'r') as f:
        content = f.read()
    
    lines = content.split('\n')
    
    # Basic metrics
    metrics = {
        "file_name": os.path.basename(filepath),
        "total_lines": len(lines),
        "non_empty_lines": len([line for line in lines if line.strip()]),
        "comment_lines": count_comment_lines(lines),
        "code_lines": count_code_lines(lines),
        "blank_lines": len([line for line in lines if not line.strip()]),
        "file_size_bytes": len(content),
        "includes": count_includes(content),
        "functions": count_functions(content),
        "variables": count_variables(content),
        "statements": count_statements(content),
        "cyclomatic_complexity": calculate_cyclomatic_complexity(content),
        "function_details": analyze_functions(content)
    }
    
    # Calculate ratios
    if metrics["total_lines"] > 0:
        metrics["comment_ratio"] = round(metrics["comment_lines"] / metrics["total_lines"], 3)
        metrics["code_ratio"] = round(metrics["code_lines"] / metrics["total_lines"], 3)
    else:
        metrics["comment_ratio"] = 0.0
        metrics["code_ratio"] = 0.0
    
    return metrics


def count_comment_lines(lines):
    """Count lines that contain comments"""
    count = 0
    in_block_comment = False
    
    for line in lines:
        line = line.strip()
        
        # Check for block comments
        if '/*' in line and '*/' in line:
            count += 1
        elif '/*' in line:
            in_block_comment = True
            count += 1
        elif '*/' in line and in_block_comment:
            in_block_comment = False
            count += 1
        elif in_block_comment:
            count += 1
        # Check for line comments
        elif '//' in line:
            count += 1
            
    return count


def count_code_lines(lines):
    """Count lines that contain actual code (excluding comments and blank lines)"""
    count = 0
    in_block_comment = False
    
    for line in lines:
        original_line = line
        line = line.strip()
        
        # Skip blank lines
        if not line:
            continue
            
        # Handle block comments
        if '/*' in line and '*/' in line:
            # Remove the comment part and check if there's code left
            before_comment = line.split('/*')[0]
            after_comment = line.split('*/')[-1] if '*/' in line else ''
            if before_comment.strip() or after_comment.strip():
                count += 1
        elif '/*' in line:
            in_block_comment = True
            # Check if there's code before the comment
            before_comment = line.split('/*')[0]
            if before_comment.strip():
                count += 1
        elif '*/' in line and in_block_comment:
            in_block_comment = False
            # Check if there's code after the comment
            after_comment = line.split('*/')[-1]
            if after_comment.strip():
                count += 1
        elif in_block_comment:
            continue
        elif '//' in line:
            # Check if there's code before the comment
            before_comment = line.split('//')[0]
            if before_comment.strip():
                count += 1
        else:
            count += 1
            
    return count


def count_includes(content):
    """Count #include statements"""
    return len(re.findall(r'#\s*include\s*[<"].*?[>"]', content))


def count_functions(content):
    """Count function definitions"""
    # Look for function patterns: type name(params) { or type name(params)\n{
    pattern = r'\b\w+\s+\w+\s*\([^)]*\)\s*\{'
    return len(re.findall(pattern, content, re.MULTILINE))


def count_variables(content):
    """Count variable declarations (simple heuristic)"""
    # This is a simplified approach - counts basic variable declarations
    lines = content.split('\n')
    var_count = 0
    
    for line in lines:
        line = line.strip()
        # Skip empty lines, comments, and preprocessor directives
        if not line or line.startswith('//') or line.startswith('/*') or line.startswith('#'):
            continue
            
        # Look for basic variable declarations (int, char, float, double, etc.)
        if re.search(r'\b(int|char|float|double|long|short|unsigned)\s+\w+', line):
            var_count += len(re.findall(r'\b(int|char|float|double|long|short|unsigned)\s+(\w+)', line))
    
    return var_count


def count_statements(content):
    """Count statements (lines ending with semicolon)"""
    return len(re.findall(r';', content))


def calculate_cyclomatic_complexity(content):
    """Calculate cyclomatic complexity (simplified)"""
    # Count decision points: if, while, for, switch, case, &&, ||
    decision_points = 0
    decision_points += len(re.findall(r'\bif\b', content))
    decision_points += len(re.findall(r'\bwhile\b', content))
    decision_points += len(re.findall(r'\bfor\b', content))
    decision_points += len(re.findall(r'\bswitch\b', content))
    decision_points += len(re.findall(r'\bcase\b', content))
    decision_points += len(re.findall(r'&&', content))
    decision_points += len(re.findall(r'\|\|', content))
    
    # Cyclomatic complexity = decision points + 1
    return decision_points + 1


def analyze_functions(content):
    """Analyze individual functions"""
    functions = []
    
    # Find function definitions
    pattern = r'(\w+)\s+(\w+)\s*\(([^)]*)\)\s*\{'
    matches = re.finditer(pattern, content, re.MULTILINE)
    
    for match in matches:
        return_type = match.group(1)
        func_name = match.group(2)
        parameters = match.group(3).strip()
        
        # Count parameters
        param_count = 0 if not parameters else len([p for p in parameters.split(',') if p.strip()])
        
        functions.append({
            "name": func_name,
            "return_type": return_type,
            "parameters": param_count,
            "parameter_list": parameters
        })
    
    return functions


def main():
    """Main function to analyze hello.c and output metrics"""
    metrics = analyze_c_file("hello.c")
    
    # Output as JSON for the web interface
    print(json.dumps(metrics, indent=2))


if __name__ == "__main__":
    main()