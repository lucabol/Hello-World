#!/usr/bin/env python3
"""
Code Metrics Spreadsheet Tool

A spreadsheet-like interface for analyzing code metrics from C source files.
Analyzes hello.c and displays various metrics in a tabular, interactive format.
"""

import os
import re
import sys
from typing import Dict, List, Tuple, Any
from datetime import datetime


class CodeMetricsAnalyzer:
    """Analyzes C source code files for various metrics."""
    
    def __init__(self, filename: str):
        self.filename = filename
        self.content = ""
        self.lines = []
        self.metrics = {}
        
    def load_file(self) -> bool:
        """Load the C source file."""
        try:
            with open(self.filename, 'r', encoding='utf-8') as f:
                self.content = f.read()
                self.lines = self.content.split('\n')
            return True
        except FileNotFoundError:
            print(f"Error: File '{self.filename}' not found.")
            return False
        except Exception as e:
            print(f"Error reading file: {e}")
            return False
    
    def analyze(self) -> Dict[str, Any]:
        """Analyze the code and calculate metrics."""
        if not self.content:
            return {}
        
        # Calculate basic metrics first
        non_empty_lines = self._count_non_empty_lines()
        comment_lines = self._count_comment_lines()
        blank_lines = self._count_blank_lines()
        
        self.metrics = {
            'File Name': os.path.basename(self.filename),
            'File Size (bytes)': len(self.content.encode('utf-8')),
            'Total Lines': len(self.lines),
            'Non-empty Lines': non_empty_lines,
            'Comment Lines': comment_lines,
            'Source Lines': non_empty_lines - comment_lines,
            'Blank Lines': blank_lines,
            'Functions': self._count_functions(),
            'Variables': self._count_variables(),
            'Include Statements': self._count_includes(),
            'Keywords': self._count_keywords(),
            'Characters': len(self.content),
            'Cyclomatic Complexity': self._calculate_complexity(),
            'Max Line Length': self._max_line_length(),
            'Avg Line Length': self._avg_line_length(),
        }
        
        return self.metrics
    
    def _count_non_empty_lines(self) -> int:
        """Count non-empty lines."""
        return sum(1 for line in self.lines if line.strip())
    
    def _count_comment_lines(self) -> int:
        """Count comment lines (single and multi-line)."""
        count = 0
        in_multiline_comment = False
        
        for line in self.lines:
            stripped = line.strip()
            
            # Check for multi-line comment start
            if '/*' in stripped:
                in_multiline_comment = True
                count += 1
                continue
            
            # Check for multi-line comment end
            if '*/' in stripped:
                in_multiline_comment = False
                if not stripped.startswith('*/'):
                    count += 1
                continue
            
            # Count lines inside multi-line comments
            if in_multiline_comment:
                count += 1
                continue
            
            # Count single-line comments
            if stripped.startswith('//'):
                count += 1
        
        return count
    
    def _count_blank_lines(self) -> int:
        """Count blank lines."""
        return sum(1 for line in self.lines if not line.strip())
    
    def _count_functions(self) -> int:
        """Count function definitions."""
        # Simple regex to match function definitions
        pattern = r'\b\w+\s+\w+\s*\([^)]*\)\s*\{'
        return len(re.findall(pattern, self.content))
    
    def _count_variables(self) -> int:
        """Count variable declarations."""
        # Simple count of basic C variable patterns
        patterns = [
            r'\bint\s+\w+',
            r'\bfloat\s+\w+',
            r'\bdouble\s+\w+',
            r'\bchar\s+\w+',
            r'\blong\s+\w+',
            r'\bshort\s+\w+',
        ]
        count = 0
        for pattern in patterns:
            count += len(re.findall(pattern, self.content))
        return count
    
    def _count_includes(self) -> int:
        """Count include statements."""
        return len(re.findall(r'#\s*include', self.content))
    
    def _count_keywords(self) -> int:
        """Count C keywords usage."""
        keywords = [
            'if', 'else', 'while', 'for', 'do', 'switch', 'case', 'default',
            'return', 'break', 'continue', 'int', 'float', 'double', 'char',
            'void', 'main', 'printf', 'scanf', 'include'
        ]
        count = 0
        for keyword in keywords:
            count += len(re.findall(r'\b' + keyword + r'\b', self.content))
        return count
    
    def _calculate_complexity(self) -> int:
        """Calculate basic cyclomatic complexity."""
        # Count decision points
        decision_keywords = ['if', 'while', 'for', 'switch', 'case']
        complexity = 1  # Base complexity
        
        for keyword in decision_keywords:
            complexity += len(re.findall(r'\b' + keyword + r'\b', self.content))
        
        return complexity
    
    def _max_line_length(self) -> int:
        """Find maximum line length."""
        return max(len(line) for line in self.lines) if self.lines else 0
    
    def _avg_line_length(self) -> float:
        """Calculate average line length."""
        if not self.lines:
            return 0.0
        total_length = sum(len(line) for line in self.lines)
        return round(total_length / len(self.lines), 2)


class SpreadsheetDisplay:
    """Display metrics in a spreadsheet-like tabular format."""
    
    def __init__(self):
        self.data = []
        self.headers = ['Metric', 'Value', 'Description']
    
    def add_metrics(self, metrics: Dict[str, Any]):
        """Add metrics data to the spreadsheet."""
        descriptions = {
            'File Name': 'Name of the analyzed file',
            'File Size (bytes)': 'Size of the file in bytes',
            'Total Lines': 'Total number of lines in the file',
            'Non-empty Lines': 'Lines that contain content',
            'Comment Lines': 'Lines containing comments',
            'Source Lines': 'Actual source code lines',
            'Blank Lines': 'Empty lines',
            'Functions': 'Number of function definitions',
            'Variables': 'Number of variable declarations',
            'Include Statements': 'Number of #include directives',
            'Keywords': 'Total C keywords usage count',
            'Characters': 'Total character count',
            'Cyclomatic Complexity': 'Measure of code complexity',
            'Max Line Length': 'Length of the longest line',
            'Avg Line Length': 'Average length of all lines',
        }
        
        for metric, value in metrics.items():
            description = descriptions.get(metric, 'Code metric')
            self.data.append([metric, value, description])
    
    def display_table(self):
        """Display the metrics in a formatted table."""
        if not self.data:
            print("No data to display.")
            return
        
        # Calculate column widths
        col_widths = [
            max(len(str(row[i])) for row in [self.headers] + self.data)
            for i in range(len(self.headers))
        ]
        
        # Ensure minimum width
        col_widths = [max(w, 15) for w in col_widths]
        
        # Print header
        self._print_separator(col_widths)
        self._print_row(self.headers, col_widths, bold=True)
        self._print_separator(col_widths)
        
        # Print data rows
        for row in self.data:
            self._print_row(row, col_widths)
        
        self._print_separator(col_widths)
    
    def _print_separator(self, col_widths: List[int]):
        """Print table separator line."""
        print("+" + "+".join("-" * (w + 2) for w in col_widths) + "+")
    
    def _print_row(self, row: List[Any], col_widths: List[int], bold: bool = False):
        """Print a table row."""
        formatted_row = []
        for i, cell in enumerate(row):
            cell_str = str(cell)
            if bold and sys.stdout.isatty():
                cell_str = f"\033[1m{cell_str}\033[0m"
            formatted_row.append(f" {cell_str:<{col_widths[i]}} ")
        
        print("|" + "|".join(formatted_row) + "|")
    
    def display_summary(self, metrics: Dict[str, Any]):
        """Display a summary of key metrics."""
        print("\n" + "="*60)
        print("CODE METRICS SUMMARY")
        print("="*60)
        print(f"File: {metrics.get('File Name', 'N/A')}")
        print(f"Analysis Date: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        print("-"*60)
        
        key_metrics = [
            ('Total Lines', metrics.get('Total Lines', 0)),
            ('Source Lines', metrics.get('Source Lines', 0)),
            ('Functions', metrics.get('Functions', 0)),
            ('Complexity', metrics.get('Cyclomatic Complexity', 0)),
        ]
        
        for metric, value in key_metrics:
            print(f"{metric:<20}: {value}")
        print("-"*60)


class InteractiveMenu:
    """Interactive menu for the spreadsheet tool."""
    
    def __init__(self, analyzer: CodeMetricsAnalyzer, display: SpreadsheetDisplay):
        self.analyzer = analyzer
        self.display = display
        self.metrics = {}
    
    def run(self):
        """Run the interactive menu."""
        print("Welcome to Code Metrics Spreadsheet Tool")
        print("="*50)
        
        # Load and analyze the file
        if not self.analyzer.load_file():
            return
        
        self.metrics = self.analyzer.analyze()
        self.display.add_metrics(self.metrics)
        
        while True:
            self._show_menu()
            choice = input("\nEnter your choice (1-4): ").strip()
            
            if choice == '1':
                self._show_full_table()
            elif choice == '2':
                self._show_summary()
            elif choice == '3':
                self._show_specific_metrics()
            elif choice == '4':
                print("Thank you for using Code Metrics Spreadsheet Tool!")
                break
            else:
                print("Invalid choice. Please try again.")
    
    def _show_menu(self):
        """Display the main menu."""
        print("\n" + "-"*50)
        print("MENU OPTIONS:")
        print("1. Show Full Metrics Table")
        print("2. Show Summary")
        print("3. Show Specific Metrics")
        print("4. Exit")
        print("-"*50)
    
    def _show_full_table(self):
        """Display the full metrics table."""
        print("\nFULL METRICS TABLE:")
        self.display.display_table()
    
    def _show_summary(self):
        """Display the metrics summary."""
        self.display.display_summary(self.metrics)
    
    def _show_specific_metrics(self):
        """Show specific metrics based on user selection."""
        print("\nAVAILABLE METRICS:")
        metric_names = list(self.metrics.keys())
        
        for i, metric in enumerate(metric_names, 1):
            print(f"{i}. {metric}")
        
        try:
            choice = input("\nEnter metric number (or 'all' for all metrics): ").strip()
            
            if choice.lower() == 'all':
                self._show_full_table()
                return
            
            idx = int(choice) - 1
            if 0 <= idx < len(metric_names):
                metric_name = metric_names[idx]
                value = self.metrics[metric_name]
                print(f"\n{metric_name}: {value}")
            else:
                print("Invalid metric number.")
        
        except ValueError:
            print("Invalid input. Please enter a number or 'all'.")


def main():
    """Main function to run the code metrics spreadsheet tool."""
    # Default file to analyze
    filename = "hello.c"
    
    # Check if file exists
    if not os.path.exists(filename):
        print(f"Error: {filename} not found in current directory.")
        return
    
    # Create components
    analyzer = CodeMetricsAnalyzer(filename)
    display = SpreadsheetDisplay()
    menu = InteractiveMenu(analyzer, display)
    
    # Run the interactive tool
    menu.run()


if __name__ == "__main__":
    main()