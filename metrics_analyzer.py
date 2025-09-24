#!/usr/bin/env python3
"""
Code Metrics Analyzer for Hello World C Program
Analyzes hello.c and generates metrics for display in spreadsheet interface
"""

import os
import re
import json
import sys
from datetime import datetime


def analyze_c_file(filepath):
    """Analyze a C source file and extract various code metrics"""
    if not os.path.exists(filepath):
        return {"error": f"File {filepath} not found"}
    
    with open(filepath, 'r') as f:
        content = f.read()
    
    lines = content.split('\n')
    
    # Basic metrics
    metrics = {
        "file_name": os.path.basename(filepath),
        "file_path": filepath,
        "total_lines": len(lines),
        "total_chars": len(content),
        "total_bytes": len(content.encode('utf-8')),
        "blank_lines": sum(1 for line in lines if line.strip() == ''),
        "comment_lines": 0,
        "code_lines": 0,
        "analysis_time": datetime.now().isoformat()
    }
    
    # Count different types of lines
    in_block_comment = False
    for line in lines:
        stripped = line.strip()
        if not stripped:
            continue
            
        # Check for comments
        if '/*' in line:
            in_block_comment = True
        if '*/' in line:
            in_block_comment = False
            metrics["comment_lines"] += 1
            continue
        if in_block_comment or stripped.startswith('//'):
            metrics["comment_lines"] += 1
            continue
        
        # If not blank or comment, it's code
        metrics["code_lines"] += 1
    
    # Advanced metrics
    metrics.update({
        "include_count": len(re.findall(r'#\s*include', content)),
        "function_count": len(re.findall(r'\w+\s+\w+\s*\([^)]*\)\s*{', content)),
        "printf_count": len(re.findall(r'printf\s*\(', content)),
        "brace_count": content.count('{') + content.count('}'),
        "semicolon_count": content.count(';'),
        "string_literals": len(re.findall(r'"[^"]*"', content))
    })
    
    # Calculate ratios and derived metrics
    if metrics["total_lines"] > 0:
        metrics["code_density"] = round(metrics["code_lines"] / metrics["total_lines"] * 100, 2)
        metrics["comment_density"] = round(metrics["comment_lines"] / metrics["total_lines"] * 100, 2)
    else:
        metrics["code_density"] = 0
        metrics["comment_density"] = 0
        
    if metrics["code_lines"] > 0:
        metrics["avg_chars_per_code_line"] = round(metrics["total_chars"] / metrics["code_lines"], 2)
    else:
        metrics["avg_chars_per_code_line"] = 0
    
    # Complexity estimation (very basic)
    complexity_patterns = [
        r'\bif\b', r'\bfor\b', r'\bwhile\b', r'\bswitch\b', 
        r'\bcase\b', r'\?\s*:', r'\&\&', r'\|\|'
    ]
    complexity = sum(len(re.findall(pattern, content)) for pattern in complexity_patterns)
    metrics["estimated_complexity"] = complexity
    
    return metrics


def generate_html_interface(metrics_data):
    """Generate HTML spreadsheet interface for displaying metrics"""
    
    # Create table rows first
    metric_descriptions = {
        "file_name": "Name of the analyzed source file",
        "file_path": "Full path to the source file",  
        "total_lines": "Total number of lines in file",
        "total_chars": "Total character count",
        "total_bytes": "File size in bytes",
        "blank_lines": "Number of empty lines",
        "comment_lines": "Lines containing comments",
        "code_lines": "Lines containing executable code",
        "include_count": "Number of #include directives",
        "function_count": "Number of function definitions",
        "printf_count": "Number of printf() calls",
        "brace_count": "Total braces ({ and })",
        "semicolon_count": "Number of semicolons",
        "string_literals": "Number of string literals",
        "code_density": "Percentage of lines that are code",
        "comment_density": "Percentage of lines that are comments", 
        "avg_chars_per_code_line": "Average characters per code line",
        "estimated_complexity": "Basic complexity estimate",
        "analysis_time": "When analysis was performed"
    }
    
    table_rows = ""
    for key, value in metrics_data.items():
        if key == "error":
            continue
            
        # Format value
        if isinstance(value, float):
            formatted_value = f"{value:.2f}"
            value_type = "Decimal"
            if key.endswith("_density"):
                formatted_value += "%"
                value_type = "Percentage"
        elif isinstance(value, int):
            formatted_value = str(value)
            value_type = "Integer"
        else:
            formatted_value = str(value)
            value_type = "String"
        
        description = metric_descriptions.get(key, "Code metric")
        
        # Apply CSS classes based on content
        css_class = "metric-name"
        if value_type == "Percentage":
            css_class = "metric-percentage"
        elif value_type in ["Integer", "Decimal"]:
            css_class = "metric-value"
        
        table_rows += f'''
                    <tr>
                        <td class="metric-name">{key.replace('_', ' ').title()}</td>
                        <td class="{css_class}">{formatted_value}</td>
                        <td>{value_type}</td>
                        <td class="metric-description">{description}</td>
                    </tr>'''
    
    # Now build the complete HTML with all the data
    html_content = f'''<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Code Metrics Spreadsheet - Hello World</title>
    <style>
        body {{
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            margin: 0;
            padding: 20px;
            background-color: #f5f5f5;
        }}
        
        .header {{
            background-color: #2c3e50;
            color: white;
            padding: 20px;
            border-radius: 8px;
            margin-bottom: 20px;
        }}
        
        .header h1 {{
            margin: 0;
            font-size: 24px;
        }}
        
        .header .subtitle {{
            margin-top: 5px;
            opacity: 0.8;
            font-size: 14px;
        }}
        
        .spreadsheet-container {{
            background: white;
            border-radius: 8px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
            overflow: hidden;
        }}
        
        .toolbar {{
            background-color: #34495e;
            color: white;
            padding: 15px 20px;
            display: flex;
            gap: 15px;
            align-items: center;
            flex-wrap: wrap;
        }}
        
        .toolbar button {{
            background-color: #3498db;
            color: white;
            border: none;
            padding: 8px 16px;
            border-radius: 4px;
            cursor: pointer;
            font-size: 12px;
            transition: background-color 0.3s;
        }}
        
        .toolbar button:hover {{
            background-color: #2980b9;
        }}
        
        .toolbar button.active {{
            background-color: #e74c3c;
        }}
        
        .toolbar input[type="text"] {{
            padding: 8px;
            border: 1px solid #ddd;
            border-radius: 4px;
            font-size: 12px;
            width: 200px;
        }}
        
        .table-container {{
            max-height: 600px;
            overflow: auto;
        }}
        
        table {{
            width: 100%;
            border-collapse: collapse;
            font-size: 13px;
        }}
        
        th {{
            background-color: #ecf0f1;
            color: #2c3e50;
            font-weight: 600;
            padding: 12px 8px;
            text-align: left;
            border-bottom: 2px solid #bdc3c7;
            position: sticky;
            top: 0;
            cursor: pointer;
            user-select: none;
        }}
        
        th:hover {{
            background-color: #d5dbdb;
        }}
        
        th.sortable::after {{
            content: " ⇅";
            opacity: 0.5;
        }}
        
        th.sort-asc::after {{
            content: " ↑";
            opacity: 1;
            color: #3498db;
        }}
        
        th.sort-desc::after {{
            content: " ↓";
            opacity: 1;
            color: #3498db;
        }}
        
        td {{
            padding: 10px 8px;
            border-bottom: 1px solid #ecf0f1;
            background-color: white;
        }}
        
        tr:nth-child(even) td {{
            background-color: #f8f9fa;
        }}
        
        tr:hover td {{
            background-color: #e8f4fd;
        }}
        
        .metric-name {{
            font-weight: 500;
            color: #2c3e50;
        }}
        
        .metric-value {{
            font-family: 'Courier New', monospace;
            color: #27ae60;
            font-weight: 500;
        }}
        
        .metric-percentage {{
            color: #e67e22;
            font-weight: 500;
        }}
        
        .metric-description {{
            color: #7f8c8d;
            font-style: italic;
        }}
        
        .summary-stats {{
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
            gap: 15px;
            margin-bottom: 20px;
        }}
        
        .stat-card {{
            background: white;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 2px 5px rgba(0,0,0,0.1);
            text-align: center;
        }}
        
        .stat-value {{
            font-size: 24px;
            font-weight: bold;
            color: #2c3e50;
        }}
        
        .stat-label {{
            color: #7f8c8d;
            font-size: 12px;
            text-transform: uppercase;
            letter-spacing: 0.5px;
        }}
        
        .filter-active {{
            background-color: #e74c3c !important;
        }}
        
        .hidden-row {{
            display: none;
        }}
    </style>
</head>
<body>
    <div class="header">
        <h1>📊 Code Metrics Spreadsheet</h1>
        <div class="subtitle">Interactive analysis of hello.c - Generated: {metrics_data.get("analysis_time", "Unknown")}</div>
    </div>
    
    <div class="summary-stats">
        <div class="stat-card">
            <div class="stat-value">{metrics_data.get("total_lines", 0)}</div>
            <div class="stat-label">Total Lines</div>
        </div>
        <div class="stat-card">
            <div class="stat-value">{metrics_data.get("code_lines", 0)}</div>
            <div class="stat-label">Code Lines</div>
        </div>
        <div class="stat-card">
            <div class="stat-value">{metrics_data.get("function_count", 0)}</div>
            <div class="stat-label">Functions</div>
        </div>
        <div class="stat-card">
            <div class="stat-value">{metrics_data.get("code_density", 0)}%</div>
            <div class="stat-label">Code Density</div>
        </div>
    </div>
    
    <div class="spreadsheet-container">
        <div class="toolbar">
            <button onclick="sortTable(1, 'asc')">Sort Value ↑</button>
            <button onclick="sortTable(1, 'desc')">Sort Value ↓</button>
            <button onclick="clearFilter()">Clear Filter</button>
            <input type="text" id="filterInput" placeholder="Filter metrics..." onkeyup="filterTable()">
            <button onclick="exportData()">Export CSV</button>
            <button onclick="refreshData()">Refresh</button>
        </div>
        
        <div class="table-container">
            <table id="metricsTable">
                <thead>
                    <tr>
                        <th class="sortable" onclick="sortTable(0)">Metric Name</th>
                        <th class="sortable" onclick="sortTable(1)">Value</th>
                        <th class="sortable" onclick="sortTable(2)">Type</th>
                        <th class="sortable" onclick="sortTable(3)">Description</th>
                    </tr>
                </thead>
                <tbody>
                    {table_rows}
                </tbody>
            </table>
        </div>
    </div>
    
    <script>
        let currentSort = {{column: -1, direction: 'asc'}};
        
        function sortTable(columnIndex, direction = null) {{
            const table = document.getElementById('metricsTable');
            const tbody = table.getElementsByTagName('tbody')[0];
            const rows = Array.from(tbody.rows);
            
            // Toggle direction if same column
            if (currentSort.column === columnIndex && direction === null) {{
                direction = currentSort.direction === 'asc' ? 'desc' : 'asc';
            }} else if (direction === null) {{
                direction = 'asc';
            }}
            
            rows.sort((a, b) => {{
                let aVal = a.cells[columnIndex].textContent.trim();
                let bVal = b.cells[columnIndex].textContent.trim();
                
                // Try to parse as numbers
                const aNum = parseFloat(aVal);
                const bNum = parseFloat(bVal);
                
                if (!isNaN(aNum) && !isNaN(bNum)) {{
                    return direction === 'asc' ? aNum - bNum : bNum - aNum;
                }} else {{
                    return direction === 'asc' 
                        ? aVal.localeCompare(bVal)
                        : bVal.localeCompare(aVal);
                }}
            }});
            
            // Remove existing rows and add sorted ones
            tbody.innerHTML = '';
            rows.forEach(row => tbody.appendChild(row));
            
            // Update header styling
            const headers = table.getElementsByTagName('th');
            for (let i = 0; i < headers.length; i++) {{
                headers[i].classList.remove('sort-asc', 'sort-desc');
            }}
            headers[columnIndex].classList.add(direction === 'asc' ? 'sort-asc' : 'sort-desc');
            
            currentSort = {{column: columnIndex, direction: direction}};
        }}
        
        function filterTable() {{
            const filter = document.getElementById('filterInput').value.toLowerCase();
            const table = document.getElementById('metricsTable');
            const rows = table.getElementsByTagName('tbody')[0].rows;
            
            for (let i = 0; i < rows.length; i++) {{
                const row = rows[i];
                let showRow = false;
                
                for (let j = 0; j < row.cells.length; j++) {{
                    const cellText = row.cells[j].textContent.toLowerCase();
                    if (cellText.includes(filter)) {{
                        showRow = true;
                        break;
                    }}
                }}
                
                row.style.display = showRow ? '' : 'none';
            }}
        }}
        
        function clearFilter() {{
            document.getElementById('filterInput').value = '';
            filterTable();
        }}
        
        function exportData() {{
            const table = document.getElementById('metricsTable');
            let csv = '';
            
            // Headers
            const headers = Array.from(table.rows[0].cells).map(cell => cell.textContent);
            csv += headers.join(',') + '\\n';
            
            // Data rows
            const tbody = table.getElementsByTagName('tbody')[0];
            for (let row of tbody.rows) {{
                if (row.style.display !== 'none') {{
                    const rowData = Array.from(row.cells).map(cell => 
                        '"' + cell.textContent.replace(/"/g, '""') + '"'
                    );
                    csv += rowData.join(',') + '\\n';
                }}
            }}
            
            // Download
            const blob = new Blob([csv], {{type: 'text/csv'}});
            const url = window.URL.createObjectURL(blob);
            const a = document.createElement('a');
            a.href = url;
            a.download = 'hello_metrics.csv';
            a.click();
            window.URL.revokeObjectURL(url);
        }}
        
        function refreshData() {{
            location.reload();
        }}
        
        // Initialize with default sort
        document.addEventListener('DOMContentLoaded', function() {{
            // Sort by metric name initially
            sortTable(0, 'asc');
        }});
    </script>
</body>
</html>'''
    
    return html_content


def main():
    """Main function to analyze hello.c and generate spreadsheet interface"""
    hello_c_path = "hello.c"
    
    if len(sys.argv) > 1:
        hello_c_path = sys.argv[1]
    
    print(f"Analyzing {hello_c_path}...")
    
    # Analyze the C file
    metrics = analyze_c_file(hello_c_path)
    
    if "error" in metrics:
        print(f"Error: {metrics['error']}")
        return 1
    
    # Generate HTML interface
    html_content = generate_html_interface(metrics)
    
    # Write HTML file
    output_file = "code_metrics_spreadsheet.html"
    with open(output_file, 'w') as f:
        f.write(html_content)
    
    # Also output JSON for other tools
    json_output = "metrics.json"
    with open(json_output, 'w') as f:
        json.dump(metrics, f, indent=2)
    
    print(f"Analysis complete!")
    print(f"- Metrics spreadsheet: {output_file}")
    print(f"- JSON data: {json_output}")
    print(f"- Analyzed {metrics['total_lines']} lines with {metrics['code_lines']} lines of code")
    
    return 0


if __name__ == "__main__":
    sys.exit(main())