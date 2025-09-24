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
            # Validate Content-Length header
            content_length = self.headers.get('Content-Length')
            if not content_length:
                self.send_json_response({'error': 'Missing Content-Length header'}, 400)
                return
                
            try:
                content_length = int(content_length)
            except ValueError:
                self.send_json_response({'error': 'Invalid Content-Length header'}, 400)
                return
                
            if content_length > 10240:  # Limit to 10KB for safety
                self.send_json_response({'error': 'Content too large'}, 413)
                return
            
            # Read and parse POST data
            post_data = self.rfile.read(content_length)
            
            try:
                data = json.loads(post_data.decode('utf-8'))
            except json.JSONDecodeError as e:
                self.send_json_response({'error': f'Invalid JSON: {str(e)}'}, 400)
                return
            except UnicodeDecodeError as e:
                self.send_json_response({'error': f'Invalid UTF-8 encoding: {str(e)}'}, 400)
                return
            
            # Validate the code content
            code = data.get('code', '')
            if not code or not code.strip():
                self.send_json_response({'error': 'No code provided or code is empty'}, 400)
                return
            
            # Basic validation - ensure it looks like C code
            if not ('#include' in code or 'main' in code):
                self.send_json_response({'error': 'Code does not appear to be valid C code'}, 400)
                return
            
            # Write to hello.c with proper error handling
            try:
                with open('hello.c', 'w', encoding='utf-8') as f:
                    f.write(code)
                    f.flush()  # Ensure data is written
                    os.fsync(f.fileno())  # Force write to disk
            except IOError as e:
                self.send_json_response({
                    'error': f'Failed to write hello.c: {str(e)}'
                }, 500)
                return
            except Exception as e:
                self.send_json_response({
                    'error': f'Unexpected error writing file: {str(e)}'
                }, 500)
                return
            
            # Verify the file was written correctly
            try:
                with open('hello.c', 'r', encoding='utf-8') as f:
                    written_content = f.read()
                if written_content != code:
                    self.send_json_response({
                        'error': 'File verification failed - content mismatch'
                    }, 500)
                    return
            except Exception as e:
                # File was written but verification failed - still report success
                # as the main operation succeeded
                print(f"Warning: File verification failed: {e}")
            
            self.send_json_response({
                'success': True,
                'message': 'Code saved to hello.c successfully!',
                'bytes_written': len(code.encode('utf-8'))
            })
            
        except Exception as e:
            print(f"Unexpected error in handle_save_code: {e}")
            self.send_json_response({
                'error': f'Server error: {str(e)}'
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