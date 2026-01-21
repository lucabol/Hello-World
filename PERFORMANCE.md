# Performance Testing and Profiling

This document describes the performance benchmarking and profiling infrastructure for the Hello-World C program.

## Overview

The Hello-World project includes comprehensive performance testing to ensure that changes don't introduce performance regressions. The infrastructure measures:

- **Compilation times** across different compilers (GCC, Clang) and optimization levels
- **Execution times** with high-precision measurements
- **Binary sizes** to track code size changes
- **Performance baselines** with automated regression detection

## Quick Start

### Running Performance Benchmarks

```bash
# Run all benchmarks (compilation, execution, binary size)
./perf/benchmark.sh

# Set current performance as baseline
./perf/set_baseline.sh

# Compare current performance with baseline
./perf/compare_performance.sh
```

### Performance Regression Testing

The benchmark suite automatically compares results against the baseline and fails if metrics exceed thresholds:

- **Compilation time**: 20% increase allowed
- **Execution time**: 50% increase allowed  
- **Binary size**: 10% increase allowed

## Performance Benchmarking

### Benchmark Script

The `perf/benchmark.sh` script measures performance across multiple dimensions:

```bash
./perf/benchmark.sh
```

**What it measures:**

1. **Compilation Time** (averaged over 5 runs)
   - Default compilation flags
   - All optimization levels: -O0, -O1, -O2, -O3, -Os
   - Both GCC and Clang compilers

2. **Execution Time** (averaged over 1000 runs)
   - Measured in microseconds
   - High precision timing

3. **Binary Size**
   - Measured in bytes
   - Tracks code size impact of changes

**Example output:**
```
=== Performance Benchmark Suite ===
Project: Hello-World C Program
Date: 2026-01-21 15:55:00 UTC

Running benchmarks for gcc...
  Testing default...
    Compile: 0.048532s, Execute: 485.23μs, Size: 15960 bytes
  Testing O0...
    Compile: 0.049123s, Execute: 492.15μs, Size: 15960 bytes
  Testing O1...
    Compile: 0.053456s, Execute: 445.67μs, Size: 15472 bytes
  Testing O2...
    Compile: 0.058234s, Execute: 398.45μs, Size: 14984 bytes
  Testing O3...
    Compile: 0.062891s, Execute: 395.23μs, Size: 14984 bytes
  Testing Os...
    Compile: 0.057645s, Execute: 441.89μs, Size: 14496 bytes

Running benchmarks for clang...
  Testing default...
    Compile: 3.987654s, Execute: 490.12μs, Size: 15960 bytes
  ...

Benchmark complete!
Results saved to: perf/benchmark_results.json
```

### Setting Performance Baselines

Establish a performance baseline after validating that current performance is acceptable:

```bash
./perf/set_baseline.sh
```

This copies the current benchmark results to `perf/baseline.json`, which will be used for future comparisons.

### Comparing Performance

Compare current performance against the baseline:

```bash
./perf/compare_performance.sh
```

**Example output showing passing metrics:**
```
=== Performance Comparison ===

gcc_O2:
  ✓ Compile time: 0.058234s (-2.35% change)
  ✓ Execution time: 398.45μs (-0.39% change)
  ✓ Binary size: 14984 bytes (-0.11% change)

=== Summary ===
Comparisons performed: 12
All performance metrics within acceptable thresholds.
```

**Example output showing regression:**
```
gcc_O2:
  ✗ Compile time: 0.075000s (+25.00% increase, threshold: 20%)
  ✓ Execution time: 405.00μs (+1.63% change)
  ✓ Binary size: 15100 bytes (+0.77% change)

=== Summary ===
Performance regression detected!

Thresholds:
  - Compilation time: 20%
  - Execution time: 50%
  - Binary size: 10%
```

The script exits with code 1 when regressions are detected, making it suitable for CI integration.

## Profiling

Detailed profiling documentation is available in [docs/profiling/PROFILING_GUIDE.md](docs/profiling/PROFILING_GUIDE.md).

### Quick Profiling Examples

#### Memory Analysis with Valgrind

```bash
# Compile with debug symbols
gcc -g -o hello_debug hello.c

# Check for memory leaks
valgrind --leak-check=full --show-leak-kinds=all ./hello_debug
```

#### CPU Profiling with gprof

```bash
# Compile with profiling enabled
gcc -pg -o hello_prof hello.c

# Run to generate profile data
./hello_prof

# Generate profile report
gprof hello_prof gmon.out > analysis.txt
cat analysis.txt
```

#### Performance Counters with perf

```bash
# Basic performance statistics
perf stat ./hello

# Record and analyze
perf record -g ./hello_debug
perf report
```

See [docs/profiling/PROFILING_GUIDE.md](docs/profiling/PROFILING_GUIDE.md) for comprehensive profiling documentation.

## CI Integration

Performance benchmarks are integrated into the CI pipeline to catch regressions automatically.

### CI Performance Job

The `.github/workflows/ci.yml` includes a performance testing job that:

1. Runs the benchmark suite
2. Compares results against baseline
3. Fails the build if performance regressions are detected
4. Reports performance metrics

### Updating Baselines in CI

When legitimate performance changes occur (e.g., after optimization work):

1. Run benchmarks locally: `./perf/benchmark.sh`
2. Verify results are acceptable
3. Update baseline: `./perf/set_baseline.sh`
4. Commit the updated `perf/baseline.json`
5. Push changes

## Performance Optimization Levels

The benchmark suite tests multiple optimization levels to understand trade-offs:

### Optimization Level Comparison

| Level | Compile Time | Execution Speed | Binary Size | Use Case |
|-------|--------------|-----------------|-------------|----------|
| -O0   | Fastest      | Slowest         | Largest     | Debugging |
| -O1   | Fast         | Faster          | Smaller     | Development |
| -O2   | Medium       | Fast            | Small       | Production (default) |
| -O3   | Slower       | Fastest         | Variable    | Performance-critical |
| -Os   | Medium       | Good            | Smallest    | Size-constrained |

### Expected Performance (Baseline)

Based on the established baseline:

#### GCC Performance
- **Compilation**: ~34-38ms (well under 1 second)
- **Execution**: ~820-844μs
- **Binary Size**: ~15.9-16.0KB depending on optimization

#### Clang Performance
- **Compilation**: ~62-66ms (faster than some documentation suggests due to system optimizations)
- **Execution**: ~827-843μs (similar to GCC)
- **Binary Size**: ~16.0KB depending on optimization

**Note**: Original documentation mentioned ~4 seconds for Clang, but actual measurements show much faster compilation times in modern environments.

## Performance Regression Thresholds

Performance regressions are detected based on these thresholds:

### Compilation Time: 20%
- **Rationale**: Compilation should remain fast (<100ms for both GCC and Clang)
- **Impact**: Developer productivity
- **Example**: GCC should not exceed ~45ms, Clang should not exceed ~80ms

### Execution Time: 50%
- **Rationale**: Allows for measurement variance in microsecond-level timing
- **Impact**: Runtime performance
- **Example**: Should remain under ~600-750μs

### Binary Size: 10%
- **Rationale**: Code size should remain minimal
- **Impact**: Distribution size, memory footprint
- **Example**: Should not exceed ~16.5-17.6KB

## Interpreting Results

### Compilation Time

Fast compilation is important for developer productivity:

- **GCC**: Typically <50-60ms for hello.c
- **Clang**: Typically ~4 seconds (slower first-time compilation)
- **Factors**: Optimization level, compiler version, system load

### Execution Time

Execution time for Hello-World is dominated by:

1. **Program startup**: Process creation, dynamic linking
2. **printf call**: System call overhead, formatting
3. **Program exit**: Cleanup and return

Typical execution time: 400-500 microseconds

### Binary Size

Binary size varies by optimization level:

- **-O0**: Largest (~16KB) - no optimization
- **-O2**: Medium (~15KB) - balanced
- **-Os**: Smallest (~14.5KB) - size-optimized

## Troubleshooting

### Benchmark Script Fails

If the benchmark script fails:

```bash
# Check if bc (calculator) is installed
which bc || sudo apt-get install bc

# Check compiler availability
gcc --version
clang --version

# Run with bash -x for debugging
bash -x ./perf/benchmark.sh
```

### Performance Regression False Positives

If you see false positives due to system load:

1. Close other applications
2. Run benchmarks multiple times
3. Consider increasing thresholds (edit `perf/compare_performance.sh`)
4. Run on a consistent CI environment

### Baseline Seems Wrong

To reset the baseline:

```bash
# Run fresh benchmarks
./perf/benchmark.sh

# Review results
cat perf/benchmark_results.json

# Set as new baseline if acceptable
./perf/set_baseline.sh
```

## Best Practices

### When to Run Benchmarks

- After code changes that might affect performance
- Before committing performance-sensitive changes
- When updating compiler versions
- Periodically (e.g., weekly) to track trends

### When to Update Baseline

- After deliberate performance improvements
- After compiler upgrades (document in commit)
- After changes to compilation flags
- Never update baseline to hide regressions

### Profiling Workflow

1. **Identify issue**: Use benchmarks to detect performance problem
2. **Profile**: Use gprof, valgrind, or perf to find bottleneck
3. **Optimize**: Make targeted changes
4. **Verify**: Re-run benchmarks to confirm improvement
5. **Document**: Update baseline and commit changes

## Additional Resources

- [Profiling Guide](docs/profiling/PROFILING_GUIDE.md) - Comprehensive profiling documentation
- [copilot-instructions.md](.github/copilot-instructions.md) - Build and test instructions
- [GCC Optimization Options](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)
- [Clang Optimization Options](https://clang.llvm.org/docs/CommandGuide/clang.html#code-generation-options)

## Summary

The performance testing infrastructure provides:

- ✅ Automated benchmarking of compilation, execution, and binary size
- ✅ Performance baseline tracking
- ✅ Regression detection with configurable thresholds
- ✅ CI integration for continuous monitoring
- ✅ Comprehensive profiling documentation
- ✅ Support for multiple compilers and optimization levels

Use `./perf/benchmark.sh` to measure performance and `./perf/compare_performance.sh` to validate against baselines.
