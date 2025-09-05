# Hello-World
Test repo for JediMaster

## Features
- Simple C "Hello World" program (`hello.c`) that prints "Ciao, Mondo!" and exit code
- Code metrics analyzer (`metrics.c`) that provides spreadsheet-like analysis of hello.c

## Usage
```bash
# Build both programs
make

# Run hello program
./hello

# Run code metrics analyzer
./metrics

# Clean build artifacts
make clean

# Build and test both programs
make test
```

## Manual Build
```bash
# Build hello program
gcc -Wall -Wextra -o hello hello.c

# Build metrics analyzer
gcc -Wall -Wextra -o metrics metrics.c
```
