#!/bin/bash
# Top-level validation script wrapper
# Calls the actual validation script in the test directory

exec "$(dirname "$0")/test/validate.sh" "$@"