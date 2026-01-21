# Code Coverage

This document describes the code coverage setup for the Hello World C project.

## Overview

The project uses `gcov` and `lcov` to generate code coverage reports. Coverage is automatically generated and uploaded as artifacts in CI, with optional integration to Codecov for coverage tracking over time.

## Local Usage

### Prerequisites

Install lcov (if not already installed):
```bash
sudo apt-get install lcov
```

### Generate Coverage Report

Run the coverage target:
```bash
make coverage
```

This will:
1. Compile the code with coverage instrumentation (`--coverage` flag)
2. Run the program to generate coverage data
3. Generate a text coverage summary using `gcov`
4. Generate an HTML coverage report using `lcov` and `genhtml`

### View Coverage Report

After running `make coverage`, open the HTML report:
```bash
# Linux/macOS
xdg-open coverage/index.html

# Or just navigate to the file
open coverage/index.html
```

The report shows:
- **Line coverage**: Percentage of lines executed
- **Function coverage**: Percentage of functions called
- **Branch coverage**: Percentage of branches taken (if available)

### Clean Coverage Artifacts

Remove coverage files:
```bash
make clean-coverage
```

Or remove all build artifacts including coverage:
```bash
make clean
```

## CI/CD Integration

### GitHub Actions

The CI workflow includes a dedicated `coverage` job that:
1. Installs lcov
2. Generates coverage reports
3. Uploads HTML reports as GitHub Actions artifacts
4. Displays coverage summary in the workflow summary
5. Checks if coverage meets the 80% threshold
6. Optionally uploads to Codecov (if configured)

### Coverage Artifacts

After each CI run, you can download the coverage report:
1. Go to the GitHub Actions run
2. Scroll to "Artifacts" section
3. Download "coverage-report"
4. Extract and open `index.html`

### Coverage Threshold

The project has a coverage threshold of **80%** for line coverage. The CI workflow will display a warning if coverage falls below this threshold, but won't fail the build.

## Codecov Integration (Optional)

The project includes optional Codecov integration for tracking coverage over time.

### Setup Codecov

1. Sign up at [codecov.io](https://codecov.io)
2. Add the repository to Codecov
3. Get the Codecov token from repository settings
4. Add the token as a GitHub secret named `CODECOV_TOKEN`

Once configured, coverage data will be automatically uploaded to Codecov on every CI run.

### Codecov Configuration

The `codecov.yml` file configures:
- Coverage precision and rounding
- Target coverage of 80%
- Threshold of 5% change
- Comment layout on pull requests
- Files to ignore (tests, documentation)

## Current Coverage

The current codebase has:
- **Line coverage**: 100% (2 of 2 lines)
- **Function coverage**: 100% (1 of 1 function)

This meets the 80% threshold requirement.

## Coverage Files

Coverage generation creates these files (all gitignored):
- `*.gcda` - Coverage data files
- `*.gcno` - Coverage notes files
- `*.gcov` - Text coverage reports
- `hello_coverage` - Instrumented binary
- `coverage/` - HTML report directory
  - `index.html` - Main coverage report
  - `coverage.info` - LCOV data file
  - Source file reports

## Troubleshooting

### lcov not found

If you see "lcov/genhtml not found", install lcov:
```bash
sudo apt-get update && sudo apt-get install -y lcov
```

### Coverage data files not found

Ensure you:
1. Compile with the coverage flag: `gcc --coverage hello.c`
2. Run the program to generate `.gcda` files
3. Run gcov with the correct file paths

### Zero coverage reported

This can happen if:
- The program wasn't executed after compiling with coverage
- The `.gcda` files were deleted
- The working directory changed

Solution: Run `make clean-coverage && make coverage`

## Further Reading

- [gcov documentation](https://gcc.gnu.org/onlinedocs/gcc/Gcov.html)
- [lcov documentation](http://ltp.sourceforge.net/coverage/lcov.php)
- [Codecov documentation](https://docs.codecov.com/)
