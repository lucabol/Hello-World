#!/bin/bash
# Code Metrics Analyzer - Spreadsheet Interface
# Analyzes hello.c and generates an interactive spreadsheet interface for code metrics

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Default file to analyze
FILE_TO_ANALYZE="hello.c"

# Parse command line arguments
if [[ $# -gt 0 ]]; then
    FILE_TO_ANALYZE="$1"
fi

echo "🔍 Analyzing code metrics for: $FILE_TO_ANALYZE"

# Check if the file exists
if [[ ! -f "$FILE_TO_ANALYZE" ]]; then
    echo "❌ Error: File '$FILE_TO_ANALYZE' not found"
    exit 1
fi

# Run the metrics analyzer
python3 metrics_analyzer.py "$FILE_TO_ANALYZE"

if [[ $? -eq 0 ]]; then
    echo ""
    echo "✅ Analysis complete! Files generated:"
    echo "   📊 Interactive Spreadsheet: code_metrics_spreadsheet.html"
    echo "   📄 JSON Data: metrics.json"
    echo ""
    echo "🌐 To view the spreadsheet interface:"
    echo "   1. Open code_metrics_spreadsheet.html in your web browser"
    echo "   2. Or run: python3 -m http.server 8080 && open http://localhost:8080/code_metrics_spreadsheet.html"
    echo ""
    echo "🔧 Features available:"
    echo "   • Interactive sorting (click column headers)"
    echo "   • Real-time filtering (use search box)"
    echo "   • CSV export functionality" 
    echo "   • Responsive design for mobile/desktop"
else
    echo "❌ Analysis failed. Please check the error messages above."
    exit 1
fi