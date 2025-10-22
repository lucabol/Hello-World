# Quick Start Guide - Collaborative Editing

## Get Started in 3 Steps

### 1. Install Dependencies
```bash
npm install
```

### 2. Start the Server
```bash
npm start
```

You should see:
```
Loaded hello.c successfully
Collaborative editing server running on http://localhost:3000
Edit hello.c in real-time with multiple users!
```

### 3. Open the Editor
Open your browser and go to:
```
http://localhost:3000/editor.html
```

Or just:
```
http://localhost:3000
```

## Test Collaborative Editing

To see collaborative editing in action:

1. Open the editor in **multiple browser tabs or windows**
2. Start typing in one window
3. Watch your changes appear **instantly** in all other windows!

You can also open the URL on different computers on the same network.

## Features at a Glance

- 📝 **Real-time sync** - Changes appear instantly across all users
- 👥 **User counter** - See how many people are editing
- 🔄 **Auto-save** - Every change is saved to hello.c automatically
- ⚡ **Fast** - Sub-second latency for most operations
- 🛡️ **Secure** - Rate-limited and secure by default

## What You Can Do

- **Edit together**: Multiple people can type at the same time
- **See changes live**: No need to refresh - changes sync automatically
- **Manual controls**: Use 💾 Save and 🔄 Reload buttons if needed
- **Version tracking**: See the current file version in the toolbar

## Building the C Program

The collaborative editor modifies `hello.c`. You can still build and run it:

```bash
gcc -o hello hello.c
./hello
```

Output: `Hello world!`

## Need Help?

- Check the [full documentation](COLLAB_DOCS.md)
- View the [README](README.md)
- Make sure port 3000 is not in use
- Check the server logs for errors

## Stop the Server

Press `Ctrl+C` in the terminal where the server is running.

## Advanced Usage

### Custom Port
```bash
PORT=8080 npm start
```

### Production Mode
For production, consider:
- Using a process manager (PM2)
- Setting up HTTPS
- Adding authentication
- Using a reverse proxy (nginx)

See [COLLAB_DOCS.md](COLLAB_DOCS.md) for details.
