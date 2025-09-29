#!/usr/bin/env python3
"""
Simple HTTP server for the Visual C Block Editor
Handles file export functionality to write generated code to hello.c
"""

import http.server
import socketserver
import json
import os
import urllib.parse
from pathlib import Path

class BlockEditorHandler(http.server.SimpleHTTPRequestHandler):
    def do_POST(self):
        if self.path == '/export':
            self.handle_export()
        else:
            self.send_response(404)
            self.end_headers()
    
    def handle_export(self):
        try:
            # Get the content length
            content_length = int(self.headers['Content-Length'])
            
            # Read the POST data
            post_data = self.rfile.read(content_length)
            
            # Parse the JSON data
            data = json.loads(post_data.decode('utf-8'))
            code = data.get('code', '')
            
            if not code:
                self.send_error(400, "No code provided")
                return
            
            # Write the code to hello.c
            hello_path = Path('hello.c')
            with open(hello_path, 'w') as f:
                f.write(code)
            
            # Send success response
            self.send_response(200)
            self.send_header('Content-Type', 'application/json')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.send_header('Access-Control-Allow-Methods', 'POST, OPTIONS')
            self.send_header('Access-Control-Allow-Headers', 'Content-Type')
            self.end_headers()
            
            response = {
                'success': True,
                'message': 'Code successfully exported to hello.c',
                'file_path': str(hello_path.absolute())
            }
            
            self.wfile.write(json.dumps(response).encode('utf-8'))
            
        except Exception as e:
            self.send_error(500, f"Export failed: {str(e)}")
    
    def do_OPTIONS(self):
        # Handle CORS preflight requests
        self.send_response(200)
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        self.end_headers()
    
    def end_headers(self):
        # Add CORS headers to all responses
        self.send_header('Access-Control-Allow-Origin', '*')
        super().end_headers()

def main():
    port = 8081
    
    # Change to the directory containing this script
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    
    with socketserver.TCPServer(("", port), BlockEditorHandler) as httpd:
        print(f"Serving Visual C Block Editor at http://localhost:{port}")
        print(f"Open http://localhost:{port}/block-editor.html in your browser")
        print("Press Ctrl+C to stop the server")
        
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nServer stopped.")

if __name__ == "__main__":
    main()