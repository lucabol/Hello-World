# Performance Benchmarking

This document describes the performance benchmarking system for the Hello World C program.

## Overview

The benchmarking system measures and tracks:
- **Compilation time** - How long it takes to compile the program
- **Binary size** - Size of the compiled executable
- **Execution time** - How long it takes to run the program

These metrics are tracked across multiple configurations:
- **Compilers**: GCC and Clang
- **Optimization levels**: -O0, -O1, -O2, -O3, -Os

## Quick Start

### Running Benchmarks

```bash
# Run benchmarks
make benchmark

# Run benchmarks and create/update baseline
make benchmark-update-baseline

# Compare current results with baseline
make benchmark-compare
```

### Testing Benchmark Accuracy

```bash
# Run benchmark accuracy tests
make test-benchmarks
```

## Benchmark Results

### Latest Results

The most recent benchmark results are stored in `benchmarks/benchmark_results.json`.

Example results:

```json
{
  "timestamp": "2026-01-21T08:17:49Z",
  "git_commit": "febda693166cd18f8a0523780f8363add043fb17",
  "benchmarks": {
    "gcc_O0": {
      "compiler": "gcc",
      "optimization": "O0",
      "compile_time_ms": 31,
      "binary_size_bytes": 15960,
      "execution_time_us": 807
    },
    "gcc_O2": {
      "compiler": "gcc",
      "optimization": "O2",
      "compile_time_ms": 35,
      "binary_size_bytes": 15968,
      "execution_time_us": 825
    }
    // ... more configurations
  }
}
```

### Performance Characteristics

Based on current measurements:

#### Compilation Time
- **GCC**: 31-37ms (very fast, consistent across optimization levels)
- **Clang**: 62-406ms (slower, especially with -O0)

#### Binary Size
- **GCC**: ~15.6-16KB
- **Clang**: ~16KB
- Optimization level has minimal impact on binary size for this simple program

#### Execution Time
- **All configurations**: ~800-830 microseconds
- Performance is consistent across compilers and optimization levels
- This is expected for such a simple program (just prints and exits)

### Key Insights

1. **GCC is faster for compilation** - Up to 6x faster than Clang for -O0
2. **Optimization has minimal impact** - For this simple program, optimization doesn't significantly affect runtime
3. **Binary size is stable** - Varies by less than 1% across configurations
4. **Execution is fast** - Sub-millisecond execution time consistently

## Baseline and Regression Detection

### Baseline

The baseline is stored in `benchmarks/baseline.json` and represents the expected performance characteristics. New benchmark results are compared against this baseline to detect regressions.

### Regression Thresholds

Performance regressions are detected when metrics exceed these thresholds:

| Metric | Threshold | Description |
|--------|-----------|-------------|
| Compilation Time | +20% | More than 20% increase in compile time |
| Binary Size | +10% | More than 10% increase in binary size |
| Execution Time | +50% | More than 50% increase in execution time |

**Note**: Execution time threshold is higher because micro-benchmarks can have more variance.

### Customizing Thresholds

You can customize thresholds using environment variables:

```bash
# Set custom thresholds
export COMPILE_TIME_THRESHOLD=30
export BINARY_SIZE_THRESHOLD=15
export EXECUTION_TIME_THRESHOLD=100

# Run comparison
make benchmark-compare
```

### Updating the Baseline

When you intentionally make changes that affect performance, update the baseline:

```bash
# Run benchmarks and update baseline in one step
make benchmark-update-baseline

# Or manually
make benchmark
cp benchmarks/benchmark_results.json benchmarks/baseline.json
git add benchmarks/baseline.json
git commit -m "Update performance baseline"
```

## CI Integration

The benchmarking system is integrated into the CI pipeline. The benchmark job:

1. Runs performance benchmarks
2. Compares results with the baseline
3. **Fails the build** if performance regressions are detected
4. Archives benchmark results as artifacts

### CI Workflow

The benchmark job runs on:
- Pull requests to `main`
- Pushes to `main`

### Viewing CI Results

1. Go to the Actions tab in GitHub
2. Select the workflow run
3. Check the "Performance Benchmarks" job
4. Download benchmark artifacts to see detailed results

## Benchmark Scripts

### run_benchmarks.sh

Main benchmarking script that:
- Compiles the program with different configurations
- Measures compilation time (average of 5 runs)
- Measures binary size
- Measures execution time (average of 1000 runs)
- Generates JSON results

**Usage:**
```bash
./benchmarks/run_benchmarks.sh [OPTIONS]

Options:
  --update-baseline   Update the baseline with current results
  --skip-comparison   Skip comparison with baseline
  --help             Show help message
```

### compare_benchmarks.py

Comparison script that:
- Loads baseline and current results
- Calculates percentage changes
- Detects regressions based on thresholds
- Generates detailed comparison report

**Usage:**
```bash
python3 benchmarks/compare_benchmarks.py
```

**Exit codes:**
- 0: No regressions detected
- 1: Performance regression detected

## Testing

### Benchmark Accuracy Tests

The test suite in `test/test_benchmarks.sh` validates:

1. Benchmark script runs successfully
2. Results file is created
3. Results file is valid JSON
4. Results contain expected fields
5. Benchmarks contain GCC results
6. Benchmarks contain Clang results
7. Compilation times are reasonable
8. Binary sizes are reasonable
9. Execution times are measured
10. Comparison script detects regressions

Run the tests:
```bash
make test-benchmarks
```

## Troubleshooting

### Benchmark Variance

Micro-benchmarks can be affected by:
- System load
- CPU frequency scaling
- Background processes
- Thermal throttling

**Tips for accurate measurements:**
- Run benchmarks on a quiet system
- Run multiple times and compare
- CI environment provides more consistent results

### Compilation Time Variations

First-time compilation may be slower due to:
- Cold caches
- Disk I/O
- System startup

The benchmark script runs multiple iterations and averages results to minimize variance.

### False Positives

If you get false regression detections:
1. Check system load
2. Run benchmarks again
3. Consider adjusting thresholds
4. Update baseline if changes are expected

## Architecture

```
benchmarks/
├── run_benchmarks.sh       # Main benchmark script
├── compare_benchmarks.py   # Comparison and regression detection
├── baseline.json          # Baseline results (committed)
└── benchmark_results.json # Latest results (gitignored)

test/
├── validate.sh            # Basic validation tests
└── test_benchmarks.sh     # Benchmark accuracy tests
```

## Future Enhancements

Potential improvements:
- Add memory usage tracking
- Track optimization impact on code size
- Add historical trend visualization
- Integrate with performance monitoring tools
- Add more statistical analysis (stddev, confidence intervals)
- Support for custom compiler flags

## References

- [copilot-instructions.md](../.github/copilot-instructions.md) - Build and test instructions
- [Makefile](../Makefile) - Build targets including benchmarks
- [CI Workflow](../.github/workflows/ci.yml) - Continuous integration configuration
