# Hello World
Test repo for JediMaster

## Binary Size and Performance

This repository includes automated checks to ensure the Hello World program remains minimal and performant:

### Current Baseline
- **Binary size**: ~16KB (15,960 bytes) when compiled with `gcc -Wall -Wextra`
- **Runtime**: <1ms (typically unmeasurable on modern systems)
- **Size threshold**: 50KB (conservative limit to detect regressions)

### Running Checks Locally

**Binary size check:**
```bash
./test/check_binary_size.sh hello
```

**Performance benchmark:**
```bash
./test/benchmark.sh hello --iterations=100
```

### Updating Thresholds

If legitimate changes require larger binaries or affect performance:

1. **Binary size threshold**: Edit `SIZE_THRESHOLD_KB` in `test/check_binary_size.sh`
2. **Performance thresholds**: Edit `STARTUP_TIME_THRESHOLD_MS` and `RUNTIME_THRESHOLD_MS` in `test/benchmark.sh`
3. **Update this README** with new baseline values
4. **Commit changes** with explanation of why thresholds needed adjustment

### CI Integration

The CI automatically:
- Builds the binary and checks size against 50KB threshold
- Runs micro-benchmarks (50 iterations in CI for speed)
- Fails if size grows unexpectedly or performance degrades
- Uploads benchmark results as artifacts for historical comparison
- Shows linked symbols (`ldd`, `nm`) if size threshold is exceeded