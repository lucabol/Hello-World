# Hello World

Test repo for JediMaster

## Overview

A simple C "Hello World" program with Docker containerization for reproducible builds across different environments.

## Quick Start

### Local Build (requires GCC/Clang)

```bash
gcc -o hello hello.c
./hello
```

### Docker Build (reproducible environment)

```bash
# Build and run using Docker
docker build -t hello-world-c .
docker run --rm hello-world-c

# Or use docker-compose
docker compose up hello-world
```

## Docker Containerization

This project includes Docker support to ensure reproducible builds across all development and CI environments. The Docker container uses Ubuntu 24.04 with specific versions of GCC (13.3.0) and Clang (18.1.3).

### Prerequisites

- Docker Engine 20.10 or later
- Docker Compose v2.0 or later (for docker-compose.yml usage)

### Docker Commands

#### Building the Docker Image

```bash
docker build -t hello-world-c .
```

This builds the image and verifies the compilation with both GCC and Clang during the build process.

#### Running the Container

```bash
# Run the default compiled program
docker run --rm hello-world-c

# Run with interactive shell for development
docker run --rm -it -v $(pwd):/app hello-world-c /bin/bash
```

#### Using Docker Compose

The project includes a `docker-compose.yml` file with multiple service configurations:

**Run the compiled program:**
```bash
docker compose up hello-world
```

**Run tests (builds with both GCC and Clang):**
```bash
docker compose run --rm test
```

**Interactive development shell:**
```bash
docker compose run --rm dev
```

This gives you a bash shell inside the container with all build tools available, and your local directory mounted at `/app`.

### Docker Services

- **hello-world**: Runs the pre-compiled program
- **dev**: Interactive shell for development work
- **test**: Runs build tests with both GCC and Clang

### Testing Docker Build Consistency

Run the Docker consistency test suite:

```bash
bash test/test_docker.sh
```

This script verifies:
- Docker image builds successfully
- Correct GCC version (13.3.0)
- Correct Clang version (18.1.3)
- Program output matches expected result
- Both GCC and Clang builds work correctly
- docker-compose services function properly

### Compiler Versions

The Docker container provides:
- **GCC**: 13.3.0 (Ubuntu 13.3.0-6ubuntu2~24.04)
- **Clang**: 18.1.3 (Ubuntu clang version 18.1.3)
- **Make**: GNU Make 4.3
- **Base OS**: Ubuntu 24.04 LTS

These versions are locked to ensure consistent builds across all environments.

## Development Workflow

### Local Development with Docker

1. Start an interactive development container:
   ```bash
   docker compose run --rm dev
   ```

2. Inside the container, edit, compile, and test:
   ```bash
   # Edit hello.c (use your host editor, changes are synced via volume)
   
   # Compile with GCC
   gcc -Wall -Wextra -o hello hello.c
   ./hello
   
   # Compile with Clang
   clang -Wall -Wextra -o hello_clang hello.c
   ./hello_clang
   ```

3. Exit the container when done:
   ```bash
   exit
   ```

### Why Docker?

Docker containerization provides several benefits:

1. **Reproducibility**: Same compiler versions across all environments
2. **Isolation**: No dependency on host system configuration
3. **Consistency**: Identical builds in development, testing, and CI
4. **Portability**: Works on any system with Docker installed
5. **Documentation**: Dockerfile serves as executable documentation of build requirements

## CI/CD Integration

The Docker containerization setup is available for local development and can be integrated into CI/CD pipelines. To add Docker-based builds to your CI workflow, you can:

1. Add a docker-build job that builds and tests the Docker image
2. Run `docker compose run --rm test` to verify both compilers work
3. Use `bash test/test_docker.sh` to run Docker consistency tests

The Docker test script (`test/test_docker.sh`) verifies compiler versions, output correctness, and Docker Compose functionality, ensuring reproducible builds across all environments.

## Architecture Decision Records

For detailed documentation on key architectural decisions, see our [Architecture Decision Records (ADRs)](docs/adr/README.md):

- [ADR-001: No Trailing Newline in Output](docs/adr/ADR-001-no-trailing-newline.md) - Why the program outputs without a trailing newline
- [ADR-002: Byte-Level Validation](docs/adr/ADR-002-byte-level-validation.md) - Our validation methodology using hex dumps
- [ADR-003: Dual Compiler Support](docs/adr/ADR-003-dual-compiler-support.md) - Why we support both GCC and Clang
