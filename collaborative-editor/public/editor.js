// Collaborative editor client
class CollaborativeEditor {
    constructor() {
        this.ws = null;
        this.clientId = null;
        this.version = 0;
        this.users = new Map();
        this.reconnectAttempts = 0;
        this.maxReconnectAttempts = 5;
        this.reconnectDelay = 1000;
        this.authenticated = false;
        
        this.editor = document.getElementById('editor');
        this.statusBadge = document.getElementById('connection-status');
        this.versionInfo = document.getElementById('version-info');
        this.usersList = document.getElementById('users-list');
        this.usernameInput = document.getElementById('username');
        this.saveBtn = document.getElementById('save-btn');
        this.reloadBtn = document.getElementById('reload-btn');
        this.saveStatus = document.getElementById('save-status');
        this.cursorPosition = document.getElementById('cursor-position');
        this.charCount = document.getElementById('char-count');
        
        // Login modal elements
        this.loginModal = document.getElementById('login-modal');
        this.authTokenInput = document.getElementById('auth-token');
        this.loginBtn = document.getElementById('login-btn');
        this.loginError = document.getElementById('login-error');
        
        this.setupEventListeners();
        this.initialize();
    }
    
    async initialize() {
        // Check if authentication is required
        try {
            const response = await fetch('/api/content');
            if (response.status === 401) {
                // Authentication required
                this.showLoginModal();
            } else {
                // No auth or already authenticated
                this.authenticated = true;
                this.connect();
            }
        } catch (error) {
            console.error('Initialization error:', error);
            this.showNotification('Failed to initialize editor', 'error');
        }
    }
    
    showLoginModal() {
        this.loginModal.style.display = 'flex';
        this.authTokenInput.focus();
    }
    
    hideLoginModal() {
        this.loginModal.style.display = 'none';
        this.authTokenInput.value = '';
        this.loginError.style.display = 'none';
    }
    
    async authenticate(token) {
        try {
            const response = await fetch('/api/auth', {
                method: 'POST',
                headers: {
                    'Authorization': `Bearer ${token}`,
                    'Content-Type': 'application/json'
                }
            });
            
            if (response.ok) {
                this.authenticated = true;
                this.hideLoginModal();
                this.connect();
                return true;
            } else {
                const data = await response.json();
                this.loginError.textContent = data.error || 'Authentication failed';
                this.loginError.style.display = 'block';
                return false;
            }
        } catch (error) {
            console.error('Authentication error:', error);
            this.loginError.textContent = 'Network error during authentication';
            this.loginError.style.display = 'block';
            return false;
        }
    }
    
    connect() {
        const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
        const wsUrl = `${protocol}//${window.location.host}`;
        
        this.ws = new WebSocket(wsUrl);
        
        this.ws.onopen = () => {
            console.log('Connected to server');
            this.updateConnectionStatus(true);
            this.reconnectAttempts = 0;
            this.showNotification('Connected to server', 'success');
        };
        
        this.ws.onmessage = (event) => {
            this.handleMessage(JSON.parse(event.data));
        };
        
        this.ws.onclose = () => {
            console.log('Disconnected from server');
            this.updateConnectionStatus(false);
            this.attemptReconnect();
        };
        
        this.ws.onerror = (error) => {
            console.error('WebSocket error:', error);
            this.showNotification('Connection error', 'error');
        };
    }
    
    attemptReconnect() {
        if (this.reconnectAttempts < this.maxReconnectAttempts) {
            this.reconnectAttempts++;
            const delay = this.reconnectDelay * this.reconnectAttempts;
            this.showNotification(`Reconnecting in ${delay/1000}s...`, 'info');
            setTimeout(() => this.connect(), delay);
        } else {
            this.showNotification('Failed to reconnect. Please refresh the page.', 'error');
        }
    }
    
    handleMessage(data) {
        switch (data.type) {
            case 'init':
                this.clientId = data.clientId;
                this.version = data.version;
                this.editor.value = data.content;
                this.updateVersionInfo();
                this.updateCharCount();
                
                // Initialize users list
                data.clients.forEach(client => {
                    this.users.set(client.id, client);
                });
                this.updateUsersList();
                break;
                
            case 'update':
                // Update content from another client
                if (data.clientId !== this.clientId) {
                    const scrollTop = this.editor.scrollTop;
                    const selectionStart = this.editor.selectionStart;
                    const selectionEnd = this.editor.selectionEnd;
                    
                    this.editor.value = data.content;
                    this.version = data.version;
                    
                    // Restore cursor position (basic approach)
                    this.editor.selectionStart = selectionStart;
                    this.editor.selectionEnd = selectionEnd;
                    this.editor.scrollTop = scrollTop;
                    
                    this.updateVersionInfo();
                    this.updateCharCount();
                    this.showNotification(`Update from ${this.getUserName(data.clientId)}`, 'info');
                }
                break;
                
            case 'ack':
                this.version = data.version;
                this.updateVersionInfo();
                this.saveStatus.textContent = 'Saved';
                this.saveStatus.className = 'save-status saved';
                setTimeout(() => {
                    this.saveStatus.textContent = '';
                    this.saveStatus.className = 'save-status';
                }, 2000);
                break;
                
            case 'user-joined':
                this.users.set(data.clientId, { id: data.clientId, name: data.name });
                this.updateUsersList();
                this.showNotification(`${data.name} joined`, 'info');
                break;
                
            case 'user-left':
                this.users.delete(data.clientId);
                this.updateUsersList();
                this.showNotification(`${data.name} left`, 'info');
                break;
                
            case 'user-renamed':
                const user = this.users.get(data.clientId);
                if (user) {
                    user.name = data.newName;
                    this.updateUsersList();
                }
                break;
                
            case 'error':
                this.showNotification(data.message, 'error');
                break;
        }
    }
    
    send(message) {
        if (this.ws && this.ws.readyState === WebSocket.OPEN) {
            this.ws.send(JSON.stringify(message));
        }
    }
    
    setupEventListeners() {
        // Login button
        this.loginBtn.addEventListener('click', () => {
            const token = this.authTokenInput.value.trim();
            if (token) {
                this.authenticate(token);
            }
        });
        
        // Login on Enter key
        this.authTokenInput.addEventListener('keypress', (e) => {
            if (e.key === 'Enter') {
                const token = this.authTokenInput.value.trim();
                if (token) {
                    this.authenticate(token);
                }
            }
        });
        
        // Editor content changes
        let saveTimeout;
        this.editor.addEventListener('input', () => {
            this.updateCharCount();
            
            // Debounce save
            clearTimeout(saveTimeout);
            this.saveStatus.textContent = 'Saving...';
            this.saveStatus.className = 'save-status saving';
            
            saveTimeout = setTimeout(() => {
                this.saveContent();
            }, 500);
        });
        
        // Track cursor position
        this.editor.addEventListener('keyup', () => this.updateCursorPosition());
        this.editor.addEventListener('click', () => this.updateCursorPosition());
        
        // Save button
        this.saveBtn.addEventListener('click', () => {
            this.saveContent();
        });
        
        // Reload button
        this.reloadBtn.addEventListener('click', () => {
            this.reloadContent();
        });
        
        // Username change
        this.usernameInput.addEventListener('change', (e) => {
            const newName = e.target.value.trim();
            if (newName) {
                this.send({
                    type: 'set-name',
                    name: newName
                });
                
                // Update local user info
                const user = this.users.get(this.clientId);
                if (user) {
                    user.name = newName;
                    this.updateUsersList();
                }
            }
        });
    }
    
    saveContent() {
        this.send({
            type: 'edit',
            content: this.editor.value
        });
    }
    
    async reloadContent() {
        try {
            const response = await fetch('/api/content');
            const data = await response.json();
            this.editor.value = data.content;
            this.version = data.version;
            this.updateVersionInfo();
            this.updateCharCount();
            this.showNotification('Content reloaded', 'success');
        } catch (error) {
            console.error('Error reloading content:', error);
            this.showNotification('Failed to reload content', 'error');
        }
    }
    
    updateConnectionStatus(connected) {
        if (connected) {
            this.statusBadge.textContent = 'Connected';
            this.statusBadge.className = 'status-badge connected';
            this.editor.disabled = false;
        } else {
            this.statusBadge.textContent = 'Disconnected';
            this.statusBadge.className = 'status-badge disconnected';
            this.editor.disabled = true;
        }
    }
    
    updateVersionInfo() {
        this.versionInfo.textContent = `Version: ${this.version}`;
    }
    
    updateUsersList() {
        this.usersList.innerHTML = '';
        this.users.forEach((user, id) => {
            const li = document.createElement('li');
            // Use textContent instead of innerHTML to prevent XSS
            li.textContent = user.name;
            if (id === this.clientId) {
                li.classList.add('you');
            }
            this.usersList.appendChild(li);
        });
    }
    
    updateCursorPosition() {
        const text = this.editor.value;
        const cursorPos = this.editor.selectionStart;
        const textBeforeCursor = text.substring(0, cursorPos);
        const lines = textBeforeCursor.split('\n');
        const line = lines.length;
        const col = lines[lines.length - 1].length + 1;
        
        this.cursorPosition.textContent = `Line ${line}, Col ${col}`;
    }
    
    updateCharCount() {
        const count = this.editor.value.length;
        this.charCount.textContent = `${count} character${count !== 1 ? 's' : ''}`;
    }
    
    getUserName(clientId) {
        const user = this.users.get(clientId);
        return user ? user.name : 'Unknown';
    }
    
    showNotification(message, type = 'info') {
        const notification = document.createElement('div');
        notification.className = `notification ${type}`;
        // Use textContent to prevent XSS
        notification.textContent = message;
        
        const container = document.getElementById('notifications');
        container.appendChild(notification);
        
        setTimeout(() => {
            notification.classList.add('removing');
            setTimeout(() => {
                container.removeChild(notification);
            }, 300);
        }, 3000);
    }
}

// Initialize editor when DOM is ready
document.addEventListener('DOMContentLoaded', () => {
    new CollaborativeEditor();
});
