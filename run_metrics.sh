#!/bin/bash

# Code Metrics Spreadsheet Launcher
# This script analyzes C files and opens the interactive spreadsheet interface

echo "🔍 Code Metrics Analyzer & Spreadsheet Tool"
echo "============================================="
echo

# Check if Python is available
if ! command -v python3 &> /dev/null; then
    echo "❌ Error: Python3 is required but not installed."
    echo "Please install Python3 and try again."
    exit 1
fi

# Run the Python analyzer
echo "📊 Analyzing C files..."
python3 code_metrics.py

# Check if analysis was successful
if [ ! -f "code_metrics.json" ]; then
    echo "❌ Error: Analysis failed. No metrics file generated."
    exit 1
fi

echo
echo "✅ Analysis complete!"
echo
echo "📋 To view the interactive spreadsheet:"
echo "   1. Open 'metrics_spreadsheet.html' in your web browser"
echo "   2. Or use: python3 -m http.server 8000"
echo "   3. Then navigate to: http://localhost:8000/metrics_spreadsheet.html"
echo

# Try to open the HTML file automatically (works on some systems)
if command -v xdg-open &> /dev/null; then
    echo "🌐 Attempting to open spreadsheet in browser..."
    xdg-open metrics_spreadsheet.html
elif command -v open &> /dev/null; then
    echo "🌐 Attempting to open spreadsheet in browser..."
    open metrics_spreadsheet.html
else
    echo "💡 Tip: Manually open 'metrics_spreadsheet.html' in your web browser"
fi

echo
echo "🔄 To refresh data: run this script again or click 'Refresh Data' in the browser"