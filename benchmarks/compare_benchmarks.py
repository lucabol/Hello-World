#!/usr/bin/env python3
"""
Benchmark comparison script
Compares current benchmark results with baseline and detects regressions
"""

import json
import sys
import os
from pathlib import Path

class Colors:
    RED = '\033[0;31m'
    GREEN = '\033[0;32m'
    YELLOW = '\033[1;33m'
    BLUE = '\033[0;34m'
    NC = '\033[0m'  # No Color

def log_info(msg):
    print(f"{Colors.BLUE}[INFO]{Colors.NC} {msg}")

def log_success(msg):
    print(f"{Colors.GREEN}[SUCCESS]{Colors.NC} {msg}")

def log_warning(msg):
    print(f"{Colors.YELLOW}[WARNING]{Colors.NC} {msg}")

def log_error(msg):
    print(f"{Colors.RED}[ERROR]{Colors.NC} {msg}")

def load_json(filepath):
    """Load JSON file"""
    try:
        with open(filepath, 'r') as f:
            return json.load(f)
    except FileNotFoundError:
        return None
    except json.JSONDecodeError as e:
        log_error(f"Failed to parse JSON from {filepath}: {e}")
        return None

def calculate_percentage_change(baseline_value, current_value):
    """Calculate percentage change from baseline to current"""
    if baseline_value == 0:
        return 0 if current_value == 0 else float('inf')
    return ((current_value - baseline_value) / baseline_value) * 100

def compare_benchmarks(baseline_file, results_file, thresholds):
    """
    Compare benchmark results with baseline
    Returns: (has_regression, report)
    """
    baseline = load_json(baseline_file)
    results = load_json(results_file)
    
    if not baseline:
        log_warning(f"Baseline file not found: {baseline_file}")
        return False, "No baseline for comparison"
    
    if not results:
        log_error(f"Results file not found: {results_file}")
        return True, "Failed to load results"
    
    baseline_benchmarks = baseline.get('benchmarks', {})
    current_benchmarks = results.get('benchmarks', {})
    
    has_regression = False
    report_lines = []
    regression_details = []
    
    report_lines.append("Benchmark Comparison Report")
    report_lines.append("=" * 80)
    report_lines.append("")
    report_lines.append(f"Baseline: {baseline.get('timestamp', 'unknown')}")
    report_lines.append(f"Current:  {results.get('timestamp', 'unknown')}")
    report_lines.append("")
    report_lines.append("Thresholds:")
    report_lines.append(f"  - Compilation time: +{thresholds['compile_time']}%")
    report_lines.append(f"  - Binary size:      +{thresholds['binary_size']}%")
    report_lines.append(f"  - Execution time:   +{thresholds['execution_time']}%")
    report_lines.append("")
    report_lines.append("=" * 80)
    report_lines.append("")
    
    # Compare each configuration
    for config_name in sorted(current_benchmarks.keys()):
        current = current_benchmarks[config_name]
        baseline_config = baseline_benchmarks.get(config_name)
        
        if not baseline_config:
            log_warning(f"No baseline for configuration: {config_name}")
            continue
        
        report_lines.append(f"Configuration: {config_name}")
        report_lines.append("-" * 80)
        
        # Compare compile time
        compile_change = calculate_percentage_change(
            baseline_config['compile_time_ms'],
            current['compile_time_ms']
        )
        compile_regression = compile_change > thresholds['compile_time']
        
        status = "❌ REGRESSION" if compile_regression else "✓"
        report_lines.append(f"  Compile time: {baseline_config['compile_time_ms']:>6d} ms -> {current['compile_time_ms']:>6d} ms "
                          f"({compile_change:+6.1f}%) {status}")
        
        if compile_regression:
            has_regression = True
            regression_details.append(f"{config_name}: compile time increased by {compile_change:.1f}%")
        
        # Compare binary size
        size_change = calculate_percentage_change(
            baseline_config['binary_size_bytes'],
            current['binary_size_bytes']
        )
        size_regression = size_change > thresholds['binary_size']
        
        status = "❌ REGRESSION" if size_regression else "✓"
        report_lines.append(f"  Binary size:  {baseline_config['binary_size_bytes']:>6d} B  -> {current['binary_size_bytes']:>6d} B  "
                          f"({size_change:+6.1f}%) {status}")
        
        if size_regression:
            has_regression = True
            regression_details.append(f"{config_name}: binary size increased by {size_change:.1f}%")
        
        # Compare execution time
        exec_change = calculate_percentage_change(
            baseline_config['execution_time_us'],
            current['execution_time_us']
        )
        exec_regression = exec_change > thresholds['execution_time']
        
        status = "❌ REGRESSION" if exec_regression else "✓"
        report_lines.append(f"  Exec time:    {baseline_config['execution_time_us']:>6d} μs -> {current['execution_time_us']:>6d} μs "
                          f"({exec_change:+6.1f}%) {status}")
        
        if exec_regression:
            has_regression = True
            regression_details.append(f"{config_name}: execution time increased by {exec_change:.1f}%")
        
        report_lines.append("")
    
    report_lines.append("=" * 80)
    report_lines.append("")
    
    if has_regression:
        report_lines.append("❌ PERFORMANCE REGRESSIONS DETECTED:")
        for detail in regression_details:
            report_lines.append(f"  - {detail}")
    else:
        report_lines.append("✓ No performance regressions detected")
    
    report_lines.append("")
    
    return has_regression, "\n".join(report_lines)

def main():
    """Main entry point"""
    script_dir = Path(__file__).parent
    results_file = script_dir / "benchmark_results.json"
    baseline_file = script_dir / "baseline.json"
    
    # Default thresholds (percentage increase allowed)
    thresholds = {
        'compile_time': int(os.environ.get('COMPILE_TIME_THRESHOLD', 20)),
        'binary_size': int(os.environ.get('BINARY_SIZE_THRESHOLD', 10)),
        'execution_time': int(os.environ.get('EXECUTION_TIME_THRESHOLD', 50))
    }
    
    log_info("Comparing benchmark results with baseline...")
    
    has_regression, report = compare_benchmarks(
        str(baseline_file),
        str(results_file),
        thresholds
    )
    
    print(report)
    
    if has_regression:
        log_error("Performance regression detected!")
        sys.exit(1)
    else:
        log_success("All benchmarks within acceptable thresholds")
        sys.exit(0)

if __name__ == '__main__':
    main()
