# Dockerfile for Hello-World C project
# Provides reproducible build environment with specific compiler versions

FROM ubuntu:24.04

# Set non-interactive mode for apt
ENV DEBIAN_FRONTEND=noninteractive

# Install build tools with specific versions
# Ubuntu 24.04 provides GCC 13.3.0 and Clang 18.1.3 by default
RUN apt-get update && apt-get install -y \
    gcc \
    g++ \
    clang \
    make \
    && rm -rf /var/lib/apt/lists/*

# Verify installed versions
RUN gcc --version && clang --version

# Set working directory
WORKDIR /app

# Copy source files
COPY hello.c .

# Build the application
RUN gcc -Wall -Wextra -o hello hello.c

# Verify GCC build
RUN ./hello

# Build with Clang for verification
RUN clang -Wall -Wextra -o hello_clang hello.c && ./hello_clang

# Default command runs the compiled program
CMD ["./hello"]
