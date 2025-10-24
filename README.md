# Hello World
 Test repo for JediMaster

## Real-Time Collaborative Editing

This repository now includes a real-time collaborative editor for `hello.c` that allows multiple users to edit the file simultaneously with live updates and automatic conflict resolution.

### Quick Start

```bash
cd collaborative-editor
npm install
npm start
```

Then open http://localhost:3000 in multiple browser windows to test collaborative editing.

### Features

- **Real-time synchronization**: Changes propagate instantly to all connected users
- **Multi-user support**: See who else is editing
- **Auto-save**: Changes are saved automatically
- **WebSocket-based**: Low-latency communication
- **Conflict resolution**: Simple last-write-wins strategy

For detailed documentation, see [collaborative-editor/README.md](collaborative-editor/README.md)