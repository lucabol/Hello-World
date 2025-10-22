# Migration and Rollback Guide

## Overview

This guide explains how to safely migrate to/from the collaborative editing system and how to recover from issues.

## Migration Scenarios

### Scenario 1: Start Using Collaborative Editing

**Recommended (Safe Mode)**:
```bash
cd collab
npm install
npm start
```

This creates `hello.c.collab` as a workspace copy and leaves the original `hello.c` untouched.

**Changes**:
- ✓ Original `hello.c` untouched
- ✓ New `hello.c.collab` created in `/collab/`
- ✓ Backups stored in `/collab/backups/`
- ✓ Easy to revert - just delete `/collab/` directory

**Rollback**: Simply stop using the collaborative editor. Original file unchanged.

---

### Scenario 2: Merge Workspace Changes Back to Source

If you've made edits in `hello.c.collab` and want to merge them to `hello.c`:

**Manual Merge**:
```bash
cd collab

# Review differences
diff hello.c.collab ../hello.c

# If satisfied, copy changes
cp hello.c.collab ../hello.c

# Or use git merge tools
git difftool hello.c.collab ../hello.c
```

**Automated Script** (create if needed):
```bash
#!/bin/bash
# merge-to-source.sh
set -e

echo "⚠️  This will overwrite hello.c with collaborative changes"
read -p "Are you sure? (yes/no): " confirm

if [ "$confirm" != "yes" ]; then
    echo "Cancelled"
    exit 1
fi

# Create backup
cp ../hello.c ../hello.c.backup.$(date +%Y%m%d-%H%M%S)

# Copy workspace to source
cp hello.c.collab ../hello.c

echo "✓ Merged hello.c.collab → hello.c"
echo "✓ Backup created: hello.c.backup.*"
```

---

### Scenario 3: Using Unsafe Direct Write Mode

**Not Recommended**, but if needed:

```bash
cd collab
npm run start:unsafe
```

**What happens**:
- ⚠️ Server edits `../hello.c` directly
- ✓ Automatic backups created in `backups/` directory
- ⚠️ All connected users edit source file simultaneously

**Rollback**: Restore from backup (see Rollback section below)

---

## Rollback Procedures

### Rollback 1: Revert to Clean State (No Collaborative Editing)

If you want to completely remove collaborative editing:

```bash
# Stop server
# Ctrl+C or pkill -f "node server.js"

# Remove collaborative directory
rm -rf collab/

# Restore original README
git checkout HEAD -- README.md

# Verify original hello.c is intact
gcc -o hello hello.c
./hello
```

**Result**: Repository back to original state, as if collaborative editing was never added.

---

### Rollback 2: Restore from Automatic Backup

If file corruption occurs:

```bash
cd collab/backups

# List available backups
ls -lt *.bak

# Pick a backup (e.g., most recent)
BACKUP_FILE="hello.c.v5.2025-10-22T12-00-00-000Z.bak"

# Restore to workspace
cp "$BACKUP_FILE" ../hello.c.collab

# Or restore to source (if using unsafe mode)
cp "$BACKUP_FILE" ../../hello.c

echo "✓ Restored from $BACKUP_FILE"
```

**Backup file naming**: `hello.c.v{version}.{timestamp}.bak`
- `{version}`: File version number
- `{timestamp}`: ISO 8601 timestamp

---

### Rollback 3: Restore from Git

If you committed changes and need to revert:

```bash
# See what changed
git log --oneline hello.c

# Restore to specific commit
git checkout <commit-hash> -- hello.c

# Or restore to previous commit
git checkout HEAD~1 -- hello.c

# Verify
gcc -o hello hello.c
./hello
```

---

### Rollback 4: Emergency Recovery

If both workspace and backups are corrupted:

**Option A: Use Git History**
```bash
# Find last good version
git log -p hello.c | less

# Restore from git
git checkout <good-commit> -- hello.c
```

**Option B: Reconstruct Manually**
```bash
# Original hello.c content
cat > hello.c << 'EOF'
# include <stdio.h>

int main(){
    printf("Hello world!");
}
EOF

# Verify
gcc -o hello hello.c
./hello
```

---

## Testing Rollback Procedures

### Test 1: Safe Mode Rollback
```bash
# Start collaborative editing
cd collab
npm start
# Make some edits...
# Stop server (Ctrl+C)

# Remove collaborative directory
cd ..
rm -rf collab/

# Verify hello.c untouched
git status hello.c  # Should show "nothing to commit"
```

### Test 2: Backup Restoration
```bash
cd collab

# Start server and make edits to create backups
npm start
# ... make edits ...
# Stop server

# Verify backups created
ls -la backups/

# Test restore
cp backups/$(ls -t backups/ | head -1) hello.c.collab.test
diff hello.c.collab hello.c.collab.test
rm hello.c.collab.test
```

### Test 3: Unsafe Mode Recovery
```bash
cd collab

# Create test backup of hello.c
cp ../hello.c ../hello.c.test-backup

# Start in unsafe mode
npm run start:unsafe
# Make a change...
# Stop server

# Verify backup was created
ls -la backups/

# Restore from backup
cp backups/$(ls -t backups/ | head -1) ../hello.c

# Verify restoration
gcc -o ../hello ../hello.c
../hello

# Cleanup
rm ../hello.c.test-backup
```

---

## Backup Management

### Automatic Backup Policy

- **When**: Before every save operation
- **Format**: `hello.c.v{version}.{timestamp}.bak`
- **Location**: `/collab/backups/`
- **Retention**: Last 10 versions (older ones automatically deleted)

### Manual Backup

Before risky operations:

```bash
# Create manual backup
cp hello.c hello.c.manual-backup.$(date +%Y%m%d-%H%M%S)

# Or with git
git add hello.c
git commit -m "Backup before collaborative editing"
```

### Backup Verification

Verify backups are being created:

```bash
cd collab

# Start server
npm start

# In another terminal, watch backups
watch -n 1 'ls -lth backups/ | head -10'

# Make edits and observe backups being created
```

---

## Disaster Recovery Plan

### Scenario: Complete Data Loss

If both working files and backups are lost:

1. **Check Git History**:
   ```bash
   git log --all --full-history -- hello.c
   git show <commit>:hello.c
   ```

2. **Check System Backups**:
   - Time Machine (macOS)
   - File History (Windows)
   - rsync backups (Linux)

3. **Check Remote Repository**:
   ```bash
   git fetch origin
   git checkout origin/main -- hello.c
   ```

4. **Reconstruct from Memory**:
   If it's a simple file like hello.c, recreate manually.

### Scenario: Server Crash During Write

Atomic writes prevent corruption:

1. **Check for Temp Files**:
   ```bash
   ls -la collab/*.tmp
   ```

2. **If temp file exists**:
   ```bash
   # Server crashed during write
   # Temp file is incomplete - delete it
   rm collab/*.tmp
   ```

3. **Restore from Last Backup**:
   ```bash
   cd collab/backups
   cp $(ls -t *.bak | head -1) ../hello.c.collab
   ```

4. **Verify Integrity**:
   ```bash
   # Check file is valid
   file hello.c.collab
   # Should show "ASCII text"
   
   # Try to compile (for C files)
   gcc -fsyntax-only hello.c.collab
   ```

---

## Migration Checklist

Before using collaborative editing:

- [ ] Original `hello.c` is committed to git
- [ ] No uncommitted changes in repository
- [ ] Git remote is up to date (`git push`)
- [ ] Tested compilation: `gcc -o hello hello.c`
- [ ] Tested execution: `./hello`
- [ ] Backup created: `cp hello.c hello.c.backup`

After using collaborative editing (merging changes):

- [ ] Reviewed diffs: `diff collab/hello.c.collab hello.c`
- [ ] Tested compilation after merge
- [ ] Committed changes to git
- [ ] Pushed to remote
- [ ] Verified CI/tests pass

---

## Rollback Decision Matrix

| Situation | Recommended Rollback |
|-----------|---------------------|
| Just started testing | Delete `/collab/` directory |
| Made edits, want to keep | Merge workspace to source |
| Made edits, want to discard | Delete `hello.c.collab` |
| File corrupted (safe mode) | Restore from backup or git |
| File corrupted (unsafe mode) | Restore from backup immediately |
| Server won't start | Check logs, reinstall dependencies |
| Git conflicts | Use git merge tools |

---

## Prevention Best Practices

### 1. Use Safe Mode
Default safe mode prevents most issues:
```bash
npm start  # Not npm run start:unsafe
```

### 2. Regular Git Commits
Commit frequently so git history is available for recovery:
```bash
git add hello.c
git commit -m "Working state before collaborative editing"
```

### 3. Manual Backups for Important Changes
Before major edits:
```bash
cp hello.c hello.c.important-backup
```

### 4. Test in Safe Environment
Test collaborative editing in a separate branch:
```bash
git checkout -b test-collab
# ... test collaborative editing ...
git checkout main  # Go back if issues
```

### 5. Monitor Backups
Ensure backups are working:
```bash
cd collab/backups
ls -lth | head -10
```

---

## Support

### Check Server Status
```bash
# Is server running?
ps aux | grep "node server.js"

# Check server logs
cd collab
npm start 2>&1 | tee server.log
```

### Verify File Integrity
```bash
# Check file is readable
cat hello.c

# Check file encoding
file hello.c

# Check for syntax errors (C files)
gcc -fsyntax-only hello.c
```

### Get Help
1. Review `collab/README.md`
2. Review `collab/SECURITY.md`
3. Check git history: `git log hello.c`
4. Examine backups: `ls -la collab/backups/`

---

## Summary

**Safe Mode** (default):
- ✓ Original file never touched
- ✓ Easy rollback: just delete `/collab/`
- ✓ No risk to source code

**Unsafe Mode** (explicit flag):
- ⚠️ Modifies source file
- ✓ Automatic backups created
- ✓ Can rollback from backups
- ⚠️ Higher risk, use with caution

**Best Practice**: Always use safe mode. Merge changes manually when ready.
