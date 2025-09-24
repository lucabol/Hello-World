#!/bin/bash
# Code Metrics Analyzer - Spreadsheet Interface
# Analyzes C source files and generates an interactive spreadsheet interface for code metrics

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Default file to analyze
FILE_TO_ANALYZE="hello.c"

# Function to show usage
show_usage() {
    cat << EOF
Usage: $0 [file] [options]

Analyze C source files and generate interactive metrics spreadsheet.

Arguments:
    file                C source file to analyze (default: hello.c)

Examples:
    $0                  # Analyze hello.c (default)
    $0 main.c           # Analyze main.c
    $0 --help           # Show this help

Generated files:
    code_metrics_spreadsheet.html   Interactive web interface
    metrics.json                    Raw JSON data

Requirements:
    - Python 3.6+ 
    - Web browser (to view results)
    - No external dependencies
EOF
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_usage
            exit 0
            ;;
        -*)
            echo "Error: Unknown option '$1'"
            echo "Use --help for usage information"
            exit 1
            ;;
        *)
            FILE_TO_ANALYZE="$1"
            ;;
    esac
    shift
done

echo "🔍 Analyzing code metrics for: $FILE_TO_ANALYZE"

# Validate input file
if [[ ! -f "$FILE_TO_ANALYZE" ]]; then
    echo "❌ Error: File '$FILE_TO_ANALYZE' not found"
    echo "   Please provide a valid C source file path"
    exit 1
fi

# Check if file is readable
if [[ ! -r "$FILE_TO_ANALYZE" ]]; then
    echo "❌ Error: File '$FILE_TO_ANALYZE' is not readable"
    echo "   Please check file permissions"
    exit 1
fi

# Validate Python is available
if ! command -v python3 &> /dev/null; then
    echo "❌ Error: python3 is required but not found in PATH"
    echo "   Please install Python 3.6 or newer"
    exit 1
fi

# Run the metrics analyzer with error handling
if python3 metrics_analyzer.py "$FILE_TO_ANALYZE"; then
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
    EXIT_CODE=$?
    echo ""
    echo "❌ Analysis failed with exit code $EXIT_CODE"
    echo "   Please check the error messages above for details"
    exit $EXIT_CODE
fi