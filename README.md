# Hello World
Test repo for JediMaster

## Collaborative Editing

This repository now supports **real-time collaborative editing** for `hello.c`!

### Features
- 🚀 Real-time synchronization across multiple users
- 🔄 Automatic conflict resolution using Operational Transform
- 👥 Live user presence indicators
- 💾 Automatic file persistence
- 🌐 Web-based editor interface

### Getting Started

1. **Install dependencies:**
   ```bash
   npm install
   ```

2. **Start the collaborative editing server:**
   ```bash
   npm start
   ```

3. **Open the editor:**
   Open your browser and navigate to `http://localhost:3000/editor.html`

4. **Collaborate:**
   - Multiple users can open the same URL and edit simultaneously
   - Changes are synchronized in real-time
   - File changes are automatically saved to `hello.c`

### Building the C Program

The original C program still works as expected:

```bash
gcc -o hello hello.c
./hello
```

Output: `Hello world!`