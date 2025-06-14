#!/usr/bin/env python3
"""
Simple web server for the Code Metrics Spreadsheet
Serves the HTML interface and provides API endpoint for metrics
"""

import http.server
import socketserver
import json
import urllib.parse
import os
from analyze_metrics import analyze_c_file


class MetricsHandler(http.server.SimpleHTTPRequestHandler):
    """Custom handler to serve files and API endpoints"""
    
    def do_GET(self):
        """Handle GET requests"""
        if self.path == '/':
            # Serve the main HTML page
            self.path = '/metrics_spreadsheet.html'
            return super().do_GET()
        elif self.path == '/api/metrics':
            # Serve metrics JSON
            self.serve_metrics()
        else:
            # Serve static files
            return super().do_GET()
    
    def serve_metrics(self):
        """Serve metrics data as JSON"""
        try:
            metrics = analyze_c_file('hello.c')
            
            self.send_response(200)
            self.send_header('Content-Type', 'application/json')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.end_headers()
            
            json_data = json.dumps(metrics, indent=2)
            self.wfile.write(json_data.encode('utf-8'))
            
        except Exception as e:
            self.send_response(500)
            self.send_header('Content-Type', 'application/json')
            self.end_headers()
            
            error_data = json.dumps({"error": str(e)})
            self.wfile.write(error_data.encode('utf-8'))


def start_server(port=8000):
    """Start the web server"""
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    
    with socketserver.TCPServer(("", port), MetricsHandler) as httpd:
        print(f"Code Metrics Spreadsheet server running at http://localhost:{port}")
        print("Press Ctrl+C to stop the server")
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nServer stopped.")


if __name__ == "__main__":
    start_server()