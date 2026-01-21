# Performance Testing Quick Reference

## Quick Commands

### Run Performance Benchmarks
```bash
./perf/benchmark.sh
```

### Check for Regressions
```bash
./perf/compare_performance.sh
```

### Set New Baseline
```bash
./perf/set_baseline.sh
```

### Run Profiling Examples
```bash
# gprof profiling
./docs/profiling/example_gprof.sh

# Valgrind memory analysis (requires valgrind)
./docs/profiling/example_valgrind.sh

# perf performance analysis (requires permissions)
./docs/profiling/example_perf.sh
```

## Performance Thresholds

| Metric | Threshold | Rationale |
|--------|-----------|-----------|
| Compilation Time | 20% | Developer productivity |
| Execution Time | 50% | Measurement variance |
| Binary Size | 10% | Code size impact |

## Expected Performance (Baseline)

### GCC
- Compilation: ~34-38ms
- Execution: ~820-844μs
- Binary Size: ~15.9-16.0KB

### Clang
- Compilation: ~62-66ms
- Execution: ~827-843μs
- Binary Size: ~16.0KB

## Documentation

- [PERFORMANCE.md](../PERFORMANCE.md) - Comprehensive performance testing guide
- [docs/profiling/PROFILING_GUIDE.md](../docs/profiling/PROFILING_GUIDE.md) - Detailed profiling documentation
- [perf/README.md](./README.md) - Performance scripts documentation

## CI Integration

The CI pipeline automatically:
1. Runs performance benchmarks
2. Compares against baseline
3. Fails on regressions
4. Uploads results as artifacts

## Troubleshooting

### "bc: command not found"
```bash
sudo apt-get install bc
```

### Inconsistent Results
- Close other applications
- Run benchmarks multiple times
- Use consistent CI environment

### False Positives
- Check system load
- Verify no background processes
- Consider adjusting thresholds in `compare_performance.sh`
