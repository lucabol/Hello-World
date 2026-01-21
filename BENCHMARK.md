# Performance Benchmarking Suite

This directory contains the performance benchmarking suite for measuring C compilation times across different optimization levels and compilers.

## Overview

The benchmarking suite measures compilation times for:
- **Optimization Levels**: O0, O1, O2, O3, Os
- **Compilers**: GCC, Clang (if available)

Results are output in structured JSON format suitable for tracking performance over time.

## Usage

### Quick Start

Run a benchmark with default settings (5 iterations):
```bash
make benchmark
```

### Options

**Quick benchmark** (3 iterations, faster):
```bash
make benchmark-quick
```

**Extensive benchmark** (10 iterations, more accurate):
```bash
make benchmark-extensive
```

**Custom benchmark**:
```bash
./benchmark.sh <source_file> <num_iterations> <output_file>
```

Example:
```bash
./benchmark.sh hello.c 5 my_results.json
```

## Output Format

Results are saved as JSON with the following structure:

```json
{
  "compiler": "gcc",
  "compiler_version": "gcc (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0",
  "optimization_level": "O2",
  "source_file": "hello.c",
  "iterations": 5,
  "average_time": 0.0300,
  "min_time": 0.0200,
  "max_time": 0.0400,
  "std_dev": 0.0089,
  "individual_times": [0.03, 0.02, 0.04, 0.03, 0.03],
  "timestamp": "2026-01-21T08:00:00Z"
}
```

### Fields

- **compiler**: Compiler used (gcc, clang)
- **compiler_version**: Full version string of the compiler
- **optimization_level**: Optimization flag (O0, O1, O2, O3, Os)
- **source_file**: Source file that was compiled
- **iterations**: Number of compilation runs performed
- **average_time**: Average compilation time in seconds
- **min_time**: Minimum compilation time in seconds
- **max_time**: Maximum compilation time in seconds
- **std_dev**: Standard deviation of compilation times
- **individual_times**: Array of individual compilation times
- **timestamp**: ISO 8601 timestamp when benchmark was run

## Testing

### Validate Benchmark Accuracy

Run tests to ensure the benchmark produces consistent and accurate results:
```bash
make test-benchmark
```

### Full Test Suite

Run all tests including validation and benchmarking:
```bash
make test
make test-benchmark
```

## Requirements

The benchmark suite requires:
- `gcc` - GNU C Compiler
- `clang` - LLVM C Compiler (optional, will skip if not available)
- `bc` - Command-line calculator for floating-point math
- `jq` - JSON processor for output formatting
- `/usr/bin/time` - Accurate time measurement utility

## Performance Expectations

Based on the simple hello.c program:

### GCC Compilation Times
- **O0**: ~0.02-0.03s (no optimization, fastest compilation)
- **O1**: ~0.03s (basic optimizations)
- **O2**: ~0.03s (moderate optimizations)
- **O3**: ~0.03s (aggressive optimizations)
- **Os**: ~0.03s (optimize for size)

### Clang Compilation Times
- **O0**: ~0.05s (no optimization)
- **O1**: ~0.05s (basic optimizations)
- **O2**: ~0.05-0.06s (moderate optimizations)
- **O3**: ~0.05s (aggressive optimizations)
- **Os**: ~0.05s (optimize for size)

*Note: Actual times may vary based on system load and hardware. These are typical values for the simple hello.c program.*

## Tracking Performance Over Time

To track performance changes:

1. Run benchmarks on each commit:
   ```bash
   make benchmark
   ```

2. Store results with version information:
   ```bash
   git rev-parse HEAD > commit.txt
   ./benchmark.sh hello.c 10 benchmark_$(git rev-parse --short HEAD).json
   ```

3. Compare results across versions:
   ```bash
   jq -s '[.[][] | select(.compiler == "gcc" and .optimization_level == "O2")] | 
          map({commit: "...", avg: .average_time})' benchmark_*.json
   ```

## CI Integration

The benchmark suite can be integrated into CI workflows:

```yaml
- name: Run performance benchmarks
  run: make benchmark-quick

- name: Upload benchmark results
  uses: actions/upload-artifact@v3
  with:
    name: benchmark-results
    path: benchmark_results.json
```

## Troubleshooting

**Error: Required command 'bc' not found**
```bash
# Ubuntu/Debian
sudo apt-get install bc

# macOS
brew install bc
```

**Error: Required command 'jq' not found**
```bash
# Ubuntu/Debian
sudo apt-get install jq

# macOS
brew install jq
```

**Inconsistent timing results**
- Reduce system load (close other applications)
- Increase iterations for more stable averages: `./benchmark.sh hello.c 20 results.json`
- Consider running on dedicated CI infrastructure for consistent environment

## License

Same as the main project.
