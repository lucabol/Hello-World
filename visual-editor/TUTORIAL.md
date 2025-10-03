# Visual Editor Tutorial

This guide demonstrates how to use the visual block-based editor to create C programs.

## Example 1: Hello World (Default)

The editor starts with a simple Hello World program:

1. Open `visual-editor/index.html` in your browser
2. You'll see a "Print Statement" block already added with "Hello world!"
3. The generated C code appears on the right:
   ```c
   # include <stdio.h>

   int main(){
       printf("Hello world!");
   }
   ```
4. Click "Download hello.c" to save the file

## Example 2: Multiple Prints

To create a program with multiple print statements:

1. Click "➕ Print (no newline)" in the Block Palette
2. Edit the text in each block's input field
3. The code updates automatically

Example result:
```c
# include <stdio.h>

int main(){
    printf("Hello ");
    printf("world!");
}
```

## Example 3: Print with Newline

To add newlines to your output:

1. Click "➕ Print (with newline)" in the Block Palette
2. Enter your text (newline is added automatically)
3. The generated code will include `\n` in the printf

Example result:
```c
# include <stdio.h>

int main(){
    printf("Hello world!\n");
}
```

## Example 4: Adding Comments

To document your code:

1. Click "➕ Comment" in the Block Palette
2. Enter your comment text
3. Comments appear as `//` style comments in the C code

Example result:
```c
# include <stdio.h>

int main(){
    // This is my first C program
    printf("Hello world!");
}
```

## Tips and Tricks

- **Delete blocks**: Click the "✖ Delete" button on any block (except main function)
- **Reset workspace**: Click "🔄 Reset" to clear all blocks and start fresh
- **Real-time updates**: Code generates automatically as you edit
- **No installation**: Everything runs in your browser - no dependencies!

## Using Generated Code

After downloading `hello.c`:

1. Copy it to the repository root:
   ```bash
   cp ~/Downloads/hello.c .
   ```

2. Compile and run:
   ```bash
   gcc -o hello hello.c
   ./hello
   ```

3. Verify with tests:
   ```bash
   bash test/validate.sh
   ```

## Learning Path

Start with these exercises:

1. **Exercise 1**: Modify the default "Hello world!" to say "Hello Universe!"
2. **Exercise 2**: Create a program that prints your name
3. **Exercise 3**: Make a program that prints multiple lines using "Print with newline" blocks
4. **Exercise 4**: Add comments to explain what your program does

Happy coding! 🎉
