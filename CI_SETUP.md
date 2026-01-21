# CI Setup Instructions for i18n Support

This document contains the necessary changes to the CI workflow to support internationalization testing.

## Required Changes to `.github/workflows/ci.yml`

Since the workflow file requires special permissions to modify, these changes must be applied manually by a repository administrator.

### For the `build-and-test-gcc` job

Add the following step after "Checkout code" and before "Setup Node.js":

```yaml
    - name: Install dependencies
      run: |
        sudo apt-get update -qq
        sudo apt-get install -y gettext locales
        sudo locale-gen es_ES.UTF-8 fr_FR.UTF-8 de_DE.UTF-8 ja_JP.UTF-8
```

### For the `build-and-test-clang` job

Add the same step after "Checkout code" and before "Build with Clang":

```yaml
    - name: Install dependencies
      run: |
        sudo apt-get update -qq
        sudo apt-get install -y gettext locales
        sudo locale-gen es_ES.UTF-8 fr_FR.UTF-8 de_DE.UTF-8 ja_JP.UTF-8
```

## Why These Changes Are Needed

1. **gettext**: Required to compile the translation files (`.po` → `.mo`)
2. **locales**: Required for locale support in the system
3. **locale-gen**: Generates the specific locales needed for testing translations

## Note

If these dependencies are not installed in the CI environment:
- The program will still compile and run with default English output
- Translation tests in `test/validate.sh` will fail for non-English languages
- The `make translations` step will fail if gettext is not available

## Verification

After applying these changes, verify that:
- All build steps complete successfully
- Translation tests pass for all languages (es, fr, de, ja)
- UTF-8 encoding validation passes
