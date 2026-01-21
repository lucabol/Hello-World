#!/usr/bin/env bash
# Test script to validate documentation completeness
# Ensures all ADR files exist and are properly linked

set -e

# Configuration
MIN_ADR_SIZE=500  # Minimum character count for ADR content

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$REPO_ROOT"

echo "Validating documentation completeness..."

# Check if ADR directory exists
if [ ! -d "docs/adr" ]; then
    echo "ERROR: docs/adr directory does not exist"
    exit 1
fi
echo "✓ docs/adr directory exists"

# Check if ADR index exists
if [ ! -f "docs/adr/README.md" ]; then
    echo "ERROR: docs/adr/README.md does not exist"
    exit 1
fi
echo "✓ docs/adr/README.md exists"

# Check if all three ADR files exist
ADR_FILES=(
    "docs/adr/ADR-001-no-trailing-newline.md"
    "docs/adr/ADR-002-byte-level-validation.md"
    "docs/adr/ADR-003-dual-compiler-support.md"
)

for adr_file in "${ADR_FILES[@]}"; do
    if [ ! -f "$adr_file" ]; then
        echo "ERROR: $adr_file does not exist"
        exit 1
    fi
    echo "✓ $adr_file exists"
done

# Check if ADR files have required sections
REQUIRED_SECTIONS=("Status" "Context" "Decision" "Consequences")

for adr_file in "${ADR_FILES[@]}"; do
    # Read file content once and check all sections
    file_content=$(cat "$adr_file")
    for section in "${REQUIRED_SECTIONS[@]}"; do
        if ! echo "$file_content" | grep -q "## $section"; then
            echo "ERROR: $adr_file missing '## $section' section"
            exit 1
        fi
    done
    echo "✓ $adr_file has required sections"
done

# Check if README.md links to ADRs
if ! grep -q "Architecture Decision Records" "README.md"; then
    echo "ERROR: README.md does not link to ADRs"
    exit 1
fi
echo "✓ README.md links to ADRs"

# Check if copilot-instructions.md links to ADRs
if ! grep -q "Architecture Decision Records" ".github/copilot-instructions.md"; then
    echo "ERROR: copilot-instructions.md does not link to ADRs"
    exit 1
fi
echo "✓ copilot-instructions.md links to ADRs"

# Check if ADR index links to all three ADRs
for adr_file in "${ADR_FILES[@]}"; do
    adr_filename=$(basename "$adr_file")
    if ! grep -q "$adr_filename" "docs/adr/README.md"; then
        echo "ERROR: docs/adr/README.md does not link to $adr_filename"
        exit 1
    fi
done
echo "✓ docs/adr/README.md links to all ADRs"

# Check if ADR files have meaningful content
for adr_file in "${ADR_FILES[@]}"; do
    file_size=$(wc -c < "$adr_file")
    if [ "$file_size" -lt "$MIN_ADR_SIZE" ]; then
        echo "ERROR: $adr_file has insufficient content (less than $MIN_ADR_SIZE characters)"
        exit 1
    fi
    echo "✓ $adr_file has meaningful content ($file_size characters)"
done

echo ""
echo "All documentation validation checks passed!"
exit 0
