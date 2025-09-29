#!/usr/bin/env python3
"""
Simple HTTP server for the Visual C Block Editor
Handles file export functionality to write generated code to hello.c

SECURITY NOTICE: This server is designed for local development only.
It binds to localhost and should not be exposed to external networks.
"""

import http.server
import socketserver
import json
import os
import re
import urllib.parse
from pathlib import Path

class CodeValidator:
    """Validates and sanitizes user input for C code generation"""
    
    # Regex patterns for validation
    C_IDENTIFIER_PATTERN = re.compile(r'^[a-zA-Z_][a-zA-Z0-9_]*$')
    HEADER_NAME_PATTERN = re.compile(r'^[a-zA-Z0-9_\./]+\.h$|^[a-zA-Z0-9_]+$')
    
    @staticmethod
    def validate_header_name(header):
        """Validate include header names"""
        if not header or len(header) > 100:
            return False
        return bool(CodeValidator.HEADER_NAME_PATTERN.match(header))
    
    @staticmethod
    def validate_identifier(identifier):
        """Validate C identifiers (variable names, function names)"""
        if not identifier or len(identifier) > 50:
            return False
        return bool(CodeValidator.C_IDENTIFIER_PATTERN.match(identifier))
    
    @staticmethod
    def escape_string_literal(text):
        """Escape string for use in C string literals"""
        if not text:
            return ""
        # Escape backslashes first, then quotes
        escaped = text.replace('\\', '\\\\').replace('"', '\\"')
        # Remove or escape potentially dangerous characters
        escaped = escaped.replace('\0', '\\0')
        escaped = escaped.replace('\n', '\\n')
        escaped = escaped.replace('\r', '\\r')
        escaped = escaped.replace('\t', '\\t')
        # Limit length to prevent overly long strings
        if len(escaped) > 1000:
            escaped = escaped[:1000] + "..."
        return escaped
    
    @staticmethod
    def validate_c_code(code):
        """Basic validation of generated C code"""
        if not code or len(code) > 10000:  # Reasonable size limit
            return False
        
        # Check for basic C structure
        if 'int main()' not in code:
            return False
            
        # Check for balanced braces
        open_braces = code.count('{')
        close_braces = code.count('}')
        if open_braces != close_braces:
            return False
            
        return True
class BlockEditorHandler(http.server.SimpleHTTPRequestHandler):
    def do_POST(self):
        if self.path == '/export':
            self.handle_export()
        else:
            self.send_response(404)
            self.send_header('Content-Type', 'application/json')
            self.end_headers()
            error_response = {'success': False, 'message': 'Endpoint not found'}
            self.wfile.write(json.dumps(error_response).encode('utf-8'))
    
    def handle_export(self):
        try:
            # Validate content length
            if 'Content-Length' not in self.headers:
                self.send_error_response(400, "Missing Content-Length header")
                return
                
            content_length = int(self.headers['Content-Length'])
            if content_length > 50000:  # Limit request size
                self.send_error_response(400, "Request too large")
                return
            
            # Read and parse POST data
            post_data = self.rfile.read(content_length)
            
            try:
                data = json.loads(post_data.decode('utf-8'))
            except (json.JSONDecodeError, UnicodeDecodeError):
                self.send_error_response(400, "Invalid JSON data")
                return
            
            code = data.get('code', '')
            if not code:
                self.send_error_response(400, "No code provided")
                return
            
            # Validate the C code
            if not CodeValidator.validate_c_code(code):
                self.send_error_response(400, "Invalid C code structure")
                return
            
            # Secure file path - only allow writing to hello.c in current directory
            hello_path = Path('./hello.c').resolve()
            expected_path = Path(os.getcwd()).resolve() / 'hello.c'
            
            if hello_path != expected_path:
                self.send_error_response(400, "Invalid file path")
                return
            
            # Create backup of existing file
            backup_path = None
            if hello_path.exists():
                backup_path = Path(f'hello.c.backup')
                try:
                    hello_path.rename(backup_path)
                except OSError:
                    self.send_error_response(500, "Failed to create backup")
                    return
            
            # Write the code to hello.c
            try:
                with open(hello_path, 'w', encoding='utf-8') as f:
                    f.write(code)
            except OSError as e:
                # Restore backup if write failed
                if backup_path and backup_path.exists():
                    backup_path.rename(hello_path)
                self.send_error_response(500, f"Failed to write file: {str(e)}")
                return
            
            # Clean up backup after successful write
            if backup_path and backup_path.exists():
                try:
                    backup_path.unlink()
                except OSError:
                    pass  # Backup cleanup failure is not critical
            
            # Send success response
            self.send_success_response({
                'success': True,
                'message': 'Code successfully exported to hello.c',
                'file_path': str(hello_path),
                'backup_created': backup_path is not None
            })
            
        except Exception as e:
            self.send_error_response(500, f"Export failed: {str(e)}")
    
    def send_success_response(self, data):
        """Send a successful JSON response"""
        self.send_response(201)  # Created
        self.send_header('Content-Type', 'application/json')
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        self.end_headers()
        self.wfile.write(json.dumps(data).encode('utf-8'))
    
    def send_error_response(self, status_code, message):
        """Send an error JSON response"""
        self.send_response(status_code)
        self.send_header('Content-Type', 'application/json')
        self.send_header('Access-Control-Allow-Origin', '*')
        self.end_headers()
        error_response = {'success': False, 'message': message}
        self.wfile.write(json.dumps(error_response).encode('utf-8'))
    
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
    host = 'localhost'  # Bind to localhost only for security
    
    # Change to the directory containing this script
    script_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(script_dir)
    
    print(f"Visual C Block Editor Server")
    print(f"SECURITY: Server bound to {host} only (local development)")
    print(f"Serving at http://{host}:{port}")
    print(f"Open http://{host}:{port}/block-editor.html in your browser")
    print("Press Ctrl+C to stop the server")
    print()
    
    with socketserver.TCPServer((host, port), BlockEditorHandler) as httpd:
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nServer stopped.")

if __name__ == "__main__":
    main()