#!/usr/bin/env python3
"""
Simple HTTP server for the Visual C Code Editor
Serves the visual-editor.html file and handles file operations
"""

import http.server
import socketserver
import json
import os
import sys
from urllib.parse import urlparse, parse_qs
from pathlib import Path

class VisualEditorHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        """Handle GET requests"""
        if self.path == '/' or self.path == '':
            # Serve the main editor page
            self.path = '/visual-editor.html'
        
        return super().do_GET()
    
    def do_POST(self):
        """Handle POST requests for file operations"""
        if self.path == '/save-code':
            self.handle_save_code()
        else:
            self.send_error(404, "Not Found")
    
    def handle_save_code(self):
        """Handle saving generated C code to hello.c"""
        try:
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length)
            data = json.loads(post_data.decode('utf-8'))
            
            code = data.get('code', '')
            if not code.strip():
                self.send_json_response({'error': 'No code provided'}, 400)
                return
            
            # Write to hello.c
            with open('hello.c', 'w') as f:
                f.write(code)
            
            self.send_json_response({
                'success': True,
                'message': 'Code saved to hello.c successfully!'
            })
            
        except Exception as e:
            print(f"Error saving code: {e}")
            self.send_json_response({
                'error': f'Failed to save code: {str(e)}'
            }, 500)
    
    def send_json_response(self, data, status_code=200):
        """Send JSON response"""
        self.send_response(status_code)
        self.send_header('Content-type', 'application/json')
        self.send_header('Access-Control-Allow-Origin', '*')
        self.end_headers()
        self.wfile.write(json.dumps(data).encode('utf-8'))

def main():
    port = 8080
    if len(sys.argv) > 1:
        try:
            port = int(sys.argv[1])
        except ValueError:
            print("Invalid port number, using default 8080")
    
    # Change to the script directory
    script_dir = Path(__file__).parent
    os.chdir(script_dir)
    
    with socketserver.TCPServer(("", port), VisualEditorHandler) as httpd:
        print(f"Visual C Code Editor server starting...")
        print(f"Server running at: http://localhost:{port}")
        print(f"Serving from: {os.getcwd()}")
        print("Press Ctrl+C to stop the server")
        
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nServer stopped.")

if __name__ == "__main__":
    main()