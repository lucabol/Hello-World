#!/usr/bin/env node
/**
 * Code Metrics Analyzer
 * Analyzes C source files and generates detailed metrics in a spreadsheet-like format
 */

const fs = require('fs');
const path = require('path');

/**
 * Analyzes a C source file and extracts various code metrics
 * @param {string} filePath - Path to the C source file
 * @returns {Object} Object containing various code metrics
 */
function analyzeFile(filePath) {
    const content = fs.readFileSync(filePath, 'utf-8');
    const lines = content.split('\n');
    
    const metrics = {
        fileName: path.basename(filePath),
        filePath: filePath,
        totalLines: lines.length,
        nonEmptyLines: 0,
        codeLines: 0,
        commentLines: 0,
        blankLines: 0,
        functions: [],
        includes: [],
        mainFunction: false,
        printfCalls: 0,
        curlyBraces: { open: 0, close: 0 },
        semicolons: 0,
        characters: content.length,
        avgLineLength: 0
    };
    
    let inBlockComment = false;
    let inFunction = false;
    let currentFunction = null;
    
    lines.forEach((line, index) => {
        const trimmedLine = line.trim();
        
        // Track blank lines
        if (trimmedLine.length === 0) {
            metrics.blankLines++;
            return;
        }
        
        metrics.nonEmptyLines++;
        
        // Track block comments
        if (trimmedLine.includes('/*')) {
            inBlockComment = true;
        }
        if (trimmedLine.includes('*/')) {
            inBlockComment = false;
            metrics.commentLines++;
            return;
        }
        if (inBlockComment) {
            metrics.commentLines++;
            return;
        }
        
        // Track single-line comments
        if (trimmedLine.startsWith('//')) {
            metrics.commentLines++;
            return;
        }
        
        // It's a code line
        metrics.codeLines++;
        
        // Track includes (handle both "#include" and "# include")
        if (trimmedLine.startsWith('#include') || trimmedLine.startsWith('# include')) {
            const match = trimmedLine.match(/#\s*include\s*[<"](.+)[>"]/);
            if (match) {
                metrics.includes.push(match[1]);
            }
        }
        
        // Track functions
        if (trimmedLine.match(/^\w+\s+\w+\s*\([^)]*\)\s*\{?/)) {
            const funcMatch = trimmedLine.match(/^(\w+)\s+(\w+)\s*\(([^)]*)\)/);
            if (funcMatch) {
                currentFunction = {
                    returnType: funcMatch[1],
                    name: funcMatch[2],
                    parameters: funcMatch[3].trim() || 'void',
                    lineNumber: index + 1,
                    lines: 1
                };
                
                if (funcMatch[2] === 'main') {
                    metrics.mainFunction = true;
                }
                
                inFunction = true;
                metrics.functions.push(currentFunction);
            }
        } else if (inFunction && currentFunction) {
            currentFunction.lines++;
        }
        
        // Track printf calls
        if (trimmedLine.includes('printf')) {
            metrics.printfCalls++;
        }
        
        // Track syntax elements
        metrics.curlyBraces.open += (line.match(/\{/g) || []).length;
        metrics.curlyBraces.close += (line.match(/\}/g) || []).length;
        metrics.semicolons += (line.match(/;/g) || []).length;
    });
    
    // Calculate average line length
    const totalChars = lines.reduce((sum, line) => sum + line.length, 0);
    metrics.avgLineLength = metrics.totalLines > 0 ? 
        (totalChars / metrics.totalLines).toFixed(2) : 0;
    
    // Calculate code density
    metrics.codeDensity = metrics.totalLines > 0 ? 
        ((metrics.codeLines / metrics.totalLines) * 100).toFixed(2) + '%' : '0%';
    
    // Calculate comment density
    metrics.commentDensity = metrics.totalLines > 0 ? 
        ((metrics.commentLines / metrics.totalLines) * 100).toFixed(2) + '%' : '0%';
    
    return metrics;
}

/**
 * Generates an HTML spreadsheet-like interface for the metrics
 * @param {Object} metrics - The metrics object
 * @returns {string} HTML content
 */
function generateHTML(metrics) {
    const functionRows = metrics.functions.map(func => `
        <tr>
            <td>${func.name}</td>
            <td>${func.returnType}</td>
            <td>${func.parameters}</td>
            <td>${func.lineNumber}</td>
            <td>${func.lines}</td>
        </tr>
    `).join('');
    
    const includeList = metrics.includes.map(inc => `<li>${inc}</li>`).join('');
    
    return `<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Code Metrics - ${metrics.fileName}</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
        }
        
        .container {
            max-width: 1400px;
            margin: 0 auto;
            background: white;
            border-radius: 10px;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
            overflow: hidden;
        }
        
        .header {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            padding: 30px;
            text-align: center;
        }
        
        .header h1 {
            font-size: 2.5em;
            margin-bottom: 10px;
        }
        
        .header p {
            font-size: 1.2em;
            opacity: 0.9;
        }
        
        .content {
            padding: 30px;
        }
        
        .metrics-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
            gap: 20px;
            margin-bottom: 30px;
        }
        
        .metric-card {
            background: linear-gradient(135deg, #f5f7fa 0%, #c3cfe2 100%);
            border-radius: 8px;
            padding: 20px;
            box-shadow: 0 4px 6px rgba(0,0,0,0.1);
            transition: transform 0.2s, box-shadow 0.2s;
        }
        
        .metric-card:hover {
            transform: translateY(-5px);
            box-shadow: 0 8px 12px rgba(0,0,0,0.15);
        }
        
        .metric-label {
            font-size: 0.9em;
            color: #666;
            text-transform: uppercase;
            letter-spacing: 1px;
            margin-bottom: 8px;
        }
        
        .metric-value {
            font-size: 2em;
            font-weight: bold;
            color: #667eea;
        }
        
        .section {
            margin-bottom: 30px;
        }
        
        .section-title {
            font-size: 1.8em;
            color: #333;
            margin-bottom: 15px;
            padding-bottom: 10px;
            border-bottom: 3px solid #667eea;
        }
        
        table {
            width: 100%;
            border-collapse: collapse;
            background: white;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
            border-radius: 8px;
            overflow: hidden;
        }
        
        thead {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
        }
        
        th {
            padding: 15px;
            text-align: left;
            font-weight: 600;
            cursor: pointer;
            user-select: none;
            position: relative;
        }
        
        th:hover {
            background: rgba(255,255,255,0.1);
        }
        
        th::after {
            content: '⇅';
            position: absolute;
            right: 10px;
            opacity: 0.5;
        }
        
        td {
            padding: 12px 15px;
            border-bottom: 1px solid #eee;
        }
        
        tbody tr:hover {
            background: #f5f7fa;
        }
        
        tbody tr:nth-child(even) {
            background: #fafbfc;
        }
        
        tbody tr:nth-child(even):hover {
            background: #f5f7fa;
        }
        
        .includes-list {
            list-style: none;
            padding: 0;
        }
        
        .includes-list li {
            background: #f5f7fa;
            padding: 10px 15px;
            margin-bottom: 8px;
            border-radius: 5px;
            border-left: 4px solid #667eea;
        }
        
        .filter-container {
            margin-bottom: 20px;
            padding: 15px;
            background: #f5f7fa;
            border-radius: 8px;
        }
        
        .filter-input {
            width: 100%;
            padding: 10px 15px;
            border: 2px solid #ddd;
            border-radius: 5px;
            font-size: 1em;
            transition: border-color 0.2s;
        }
        
        .filter-input:focus {
            outline: none;
            border-color: #667eea;
        }
        
        .highlight {
            background-color: #ffeb3b;
            padding: 2px 4px;
            border-radius: 2px;
        }
        
        .export-buttons {
            margin-top: 20px;
            text-align: center;
        }
        
        .btn {
            display: inline-block;
            padding: 12px 30px;
            margin: 5px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            font-size: 1em;
            transition: transform 0.2s;
        }
        
        .btn:hover {
            transform: translateY(-2px);
            box-shadow: 0 4px 8px rgba(0,0,0,0.2);
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>📊 Code Metrics Dashboard</h1>
            <p>Analysis of ${metrics.fileName}</p>
        </div>
        
        <div class="content">
            <!-- Summary Metrics -->
            <div class="section">
                <h2 class="section-title">Summary Metrics</h2>
                <div class="metrics-grid">
                    <div class="metric-card">
                        <div class="metric-label">Total Lines</div>
                        <div class="metric-value">${metrics.totalLines}</div>
                    </div>
                    <div class="metric-card">
                        <div class="metric-label">Code Lines</div>
                        <div class="metric-value">${metrics.codeLines}</div>
                    </div>
                    <div class="metric-card">
                        <div class="metric-label">Comment Lines</div>
                        <div class="metric-value">${metrics.commentLines}</div>
                    </div>
                    <div class="metric-card">
                        <div class="metric-label">Blank Lines</div>
                        <div class="metric-value">${metrics.blankLines}</div>
                    </div>
                    <div class="metric-card">
                        <div class="metric-label">Functions</div>
                        <div class="metric-value">${metrics.functions.length}</div>
                    </div>
                    <div class="metric-card">
                        <div class="metric-label">Characters</div>
                        <div class="metric-value">${metrics.characters}</div>
                    </div>
                    <div class="metric-card">
                        <div class="metric-label">Code Density</div>
                        <div class="metric-value">${metrics.codeDensity}</div>
                    </div>
                    <div class="metric-card">
                        <div class="metric-label">Avg Line Length</div>
                        <div class="metric-value">${metrics.avgLineLength}</div>
                    </div>
                </div>
            </div>
            
            <!-- Functions Table -->
            ${metrics.functions.length > 0 ? `
            <div class="section">
                <h2 class="section-title">Functions</h2>
                <div class="filter-container">
                    <input type="text" id="functionFilter" class="filter-input" placeholder="Filter functions by name, return type, or parameters...">
                </div>
                <table id="functionsTable">
                    <thead>
                        <tr>
                            <th onclick="sortTable(0)">Function Name</th>
                            <th onclick="sortTable(1)">Return Type</th>
                            <th onclick="sortTable(2)">Parameters</th>
                            <th onclick="sortTable(3)">Line Number</th>
                            <th onclick="sortTable(4)">Lines of Code</th>
                        </tr>
                    </thead>
                    <tbody>
                        ${functionRows}
                    </tbody>
                </table>
            </div>
            ` : ''}
            
            <!-- Includes -->
            ${metrics.includes.length > 0 ? `
            <div class="section">
                <h2 class="section-title">Includes</h2>
                <ul class="includes-list">
                    ${includeList}
                </ul>
            </div>
            ` : ''}
            
            <!-- Additional Metrics -->
            <div class="section">
                <h2 class="section-title">Additional Metrics</h2>
                <div class="metrics-grid">
                    <div class="metric-card">
                        <div class="metric-label">Printf Calls</div>
                        <div class="metric-value">${metrics.printfCalls}</div>
                    </div>
                    <div class="metric-card">
                        <div class="metric-label">Semicolons</div>
                        <div class="metric-value">${metrics.semicolons}</div>
                    </div>
                    <div class="metric-card">
                        <div class="metric-label">Opening Braces</div>
                        <div class="metric-value">${metrics.curlyBraces.open}</div>
                    </div>
                    <div class="metric-card">
                        <div class="metric-label">Closing Braces</div>
                        <div class="metric-value">${metrics.curlyBraces.close}</div>
                    </div>
                    <div class="metric-card">
                        <div class="metric-label">Has Main Function</div>
                        <div class="metric-value">${metrics.mainFunction ? '✓' : '✗'}</div>
                    </div>
                    <div class="metric-card">
                        <div class="metric-label">Comment Density</div>
                        <div class="metric-value">${metrics.commentDensity}</div>
                    </div>
                </div>
            </div>
            
            <!-- Export Options -->
            <div class="export-buttons">
                <button class="btn" onclick="exportToCSV()">Export to CSV</button>
                <button class="btn" onclick="exportToJSON()">Export to JSON</button>
                <button class="btn" onclick="window.print()">Print Report</button>
            </div>
        </div>
    </div>
    
    <script>
        // Store original metrics data
        const metricsData = ${JSON.stringify(metrics, null, 2)};
        
        // Table sorting functionality
        function sortTable(columnIndex) {
            const table = document.getElementById('functionsTable');
            const tbody = table.querySelector('tbody');
            const rows = Array.from(tbody.querySelectorAll('tr'));
            
            let ascending = table.dataset.sortAscending !== 'true';
            table.dataset.sortAscending = ascending;
            
            rows.sort((a, b) => {
                const aValue = a.cells[columnIndex].textContent.trim();
                const bValue = b.cells[columnIndex].textContent.trim();
                
                // Try numeric comparison first
                const aNum = parseFloat(aValue);
                const bNum = parseFloat(bValue);
                
                if (!isNaN(aNum) && !isNaN(bNum)) {
                    return ascending ? aNum - bNum : bNum - aNum;
                }
                
                // Fall back to string comparison
                return ascending ? 
                    aValue.localeCompare(bValue) : 
                    bValue.localeCompare(aValue);
            });
            
            rows.forEach(row => tbody.appendChild(row));
        }
        
        // Filter functionality
        const functionFilter = document.getElementById('functionFilter');
        if (functionFilter) {
            functionFilter.addEventListener('input', function() {
                const filterValue = this.value.toLowerCase();
                const table = document.getElementById('functionsTable');
                const rows = table.querySelectorAll('tbody tr');
                
                rows.forEach(row => {
                    const text = row.textContent.toLowerCase();
                    row.style.display = text.includes(filterValue) ? '' : 'none';
                });
            });
        }
        
        // Export to CSV
        function exportToCSV() {
            let csv = 'Metric,Value\\n';
            csv += \`Total Lines,\${metricsData.totalLines}\\n\`;
            csv += \`Code Lines,\${metricsData.codeLines}\\n\`;
            csv += \`Comment Lines,\${metricsData.commentLines}\\n\`;
            csv += \`Blank Lines,\${metricsData.blankLines}\\n\`;
            csv += \`Functions,\${metricsData.functions.length}\\n\`;
            csv += \`Characters,\${metricsData.characters}\\n\`;
            csv += \`Code Density,\${metricsData.codeDensity}\\n\`;
            csv += \`Comment Density,\${metricsData.commentDensity}\\n\`;
            csv += \`Average Line Length,\${metricsData.avgLineLength}\\n\`;
            csv += '\\n';
            
            if (metricsData.functions.length > 0) {
                csv += 'Function Name,Return Type,Parameters,Line Number,Lines of Code\\n';
                metricsData.functions.forEach(func => {
                    csv += \`\${func.name},\${func.returnType},"\${func.parameters}",\${func.lineNumber},\${func.lines}\\n\`;
                });
            }
            
            downloadFile(csv, 'code-metrics.csv', 'text/csv');
        }
        
        // Export to JSON
        function exportToJSON() {
            const json = JSON.stringify(metricsData, null, 2);
            downloadFile(json, 'code-metrics.json', 'application/json');
        }
        
        // Helper function to download files
        function downloadFile(content, filename, contentType) {
            const blob = new Blob([content], { type: contentType });
            const url = URL.createObjectURL(blob);
            const link = document.createElement('a');
            link.href = url;
            link.download = filename;
            document.body.appendChild(link);
            link.click();
            document.body.removeChild(link);
            URL.revokeObjectURL(url);
        }
    </script>
</body>
</html>`;
}

// Main execution
function main() {
    const args = process.argv.slice(2);
    const filePath = args[0] || 'hello.c';
    
    if (!fs.existsSync(filePath)) {
        console.error(`Error: File "${filePath}" not found`);
        process.exit(1);
    }
    
    console.log(`Analyzing ${filePath}...`);
    
    const metrics = analyzeFile(filePath);
    
    // Save JSON metrics
    fs.writeFileSync('metrics.json', JSON.stringify(metrics, null, 2));
    console.log('✓ Metrics saved to metrics.json');
    
    // Generate HTML report
    const html = generateHTML(metrics);
    fs.writeFileSync('metrics-report.html', html);
    console.log('✓ HTML report generated: metrics-report.html');
    
    // Print summary to console
    console.log('\\n=== Code Metrics Summary ===');
    console.log(`File: ${metrics.fileName}`);
    console.log(`Total Lines: ${metrics.totalLines}`);
    console.log(`Code Lines: ${metrics.codeLines}`);
    console.log(`Comment Lines: ${metrics.commentLines}`);
    console.log(`Blank Lines: ${metrics.blankLines}`);
    console.log(`Functions: ${metrics.functions.length}`);
    console.log(`Code Density: ${metrics.codeDensity}`);
    console.log(`Comment Density: ${metrics.commentDensity}`);
    
    if (metrics.functions.length > 0) {
        console.log('\\nFunctions:');
        metrics.functions.forEach(func => {
            console.log(`  - ${func.name}() [${func.returnType}] at line ${func.lineNumber}`);
        });
    }
    
    console.log('\\n✓ Analysis complete!');
    console.log('Open metrics-report.html in a browser to view the interactive dashboard');
}

if (require.main === module) {
    main();
}

module.exports = { analyzeFile, generateHTML };
