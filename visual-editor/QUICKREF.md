# Visual Editor Quick Reference

## 🚀 Getting Started (5 seconds)

```bash
cd visual-editor
open index.html    # macOS
# or
firefox index.html  # Linux
# or
start index.html   # Windows
```

## 📦 Available Blocks

| Block Type | Description | Generated Code |
|------------|-------------|----------------|
| **Print (no newline)** | Print text without adding newline | `printf("text");` |
| **Print (with newline)** | Print text with automatic newline | `printf("text\n");` |
| **Comment** | Add documentation | `// comment` |

## 🎨 Interface Elements

- **Block Palette**: Click to add blocks (top-left)
- **Workspace**: Arrange and edit blocks (left)
- **Code Preview**: See generated C code (right)
- **Download Button**: Save as hello.c (bottom-right)
- **Reset Button**: Clear workspace (bottom-right)

## ⚡ Quick Actions

| Action | How To |
|--------|--------|
| Add a block | Click button in Block Palette |
| Edit text | Click input field and type |
| Delete block | Click ✖ Delete button |
| Save code | Click "💾 Download hello.c" |
| Start over | Click "🔄 Reset" |

## 📝 Example Programs

### Simple Hello World
```c
# include <stdio.h>

int main(){
    printf("Hello world!");
}
```
*Blocks: 1 Print (no newline) with "Hello world!"*

### Multiple Prints
```c
# include <stdio.h>

int main(){
    printf("Hello ");
    printf("world!");
}
```
*Blocks: 2 Print (no newline) blocks*

### With Newline
```c
# include <stdio.h>

int main(){
    printf("Hello world!\n");
}
```
*Blocks: 1 Print (with newline) with "Hello world!"*

### Documented Code
```c
# include <stdio.h>

int main(){
    // This greets the world
    printf("Hello world!");
}
```
*Blocks: 1 Comment + 1 Print (no newline)*

## ✅ Testing Your Code

### Quick Test
```bash
gcc -o hello hello.c && ./hello
```

### Strict Test (CI-style)
```bash
gcc -Wall -Wextra -Wpedantic -Werror -std=c99 -o hello hello.c
./hello
```

### Run Integration Tests
```bash
bash visual-editor/test-integration.sh
```

## 🔧 Troubleshooting

| Problem | Solution |
|---------|----------|
| Blocks not appearing | Refresh the page (F5) |
| Download not working | Check browser's download settings |
| Code not compiling | Verify all quotes are properly closed |
| Missing text | Check input fields aren't empty |

## 💡 Tips

1. **Start Simple**: Begin with the default Hello World
2. **Experiment**: Try different block combinations
3. **Test Often**: Download and compile frequently
4. **Add Comments**: Document complex programs
5. **Save Versions**: Download multiple versions as you work

## 📚 Documentation Files

- `README.md` - Full feature documentation
- `TUTORIAL.md` - Step-by-step examples
- `INTERFACE.md` - UI layout and components
- `QUICKREF.md` - This file
- `test-integration.sh` - Automated testing

## 🎯 Learning Path

1. ✅ Open the editor
2. ✅ Modify the default text
3. ✅ Add a second print block
4. ✅ Try adding a comment
5. ✅ Download your code
6. ✅ Compile and run it

## ⚙️ Technical Details

- **No installation required** - runs in browser
- **No external dependencies** - fully self-contained
- **Real-time updates** - code generates as you type
- **Standards compliant** - generates C99-compatible code
- **Strict mode tested** - compiles with -Werror

## 🆘 Need Help?

- Check TUTORIAL.md for detailed examples
- See INTERFACE.md for UI explanation
- Run test-integration.sh to verify setup
- Review README.md for complete documentation

---

**Ready to code?** Open `index.html` and start creating! 🎉
