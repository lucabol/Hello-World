# Hello World
Test repo for JediMaster

## Usage

Basic usage:
```bash
./hello
```
Output: `Hello world!` (without trailing newline)

### Newline Option

To add a trailing newline for better terminal usability:

**Using command-line flag:**
```bash
./hello -n
# or
./hello --newline
```

**Using environment variable:**
```bash
HELLO_NEWLINE=1 ./hello
# or
HELLO_NEWLINE=true ./hello
```

## Building

```bash
gcc -o hello hello.c
```