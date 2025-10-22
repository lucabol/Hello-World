# Hello World
Test repo for JediMaster

## Building the C Program

```bash
gcc -o hello hello.c
./hello
```

Output: `Hello world!`

## Collaborative Editing (Optional)

⚠️ **For development/demonstration only - see security warnings**

This repository includes an **optional** real-time collaborative editing feature for `hello.c`.

### Features
- 🚀 Real-time synchronization across multiple users
- 🔄 Automatic conflict resolution using Operational Transform
- 👥 Live user presence indicators
- 💾 Automatic file persistence with versioned backups
- 🔒 Safe mode: Edits workspace file by default, not source file
- 🌐 Web-based editor interface

### Quick Start

```bash
cd collab
npm install
npm start
```

Then open `http://localhost:3000/editor.html` in your browser.

**See [`collab/README.md`](collab/README.md) for detailed instructions, security information, and configuration options.**

### Important Notes

- The collaborative editor is **isolated in the `collab/` directory**
- By default, it edits a **workspace file** (`hello.c.collab`), not the source file
- Server binds to **localhost only** by default
- **NOT production-ready** without authentication, HTTPS, and additional hardening
- See documentation for unsafe modes and external binding options