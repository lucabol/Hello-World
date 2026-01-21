# Hello World with Internationalization (i18n)

A simple Hello World program with multilingual support using gettext.

## Features

- **Multilingual Support**: Translations for English (default), Spanish, French, German, and Japanese
- **Environment Variable Support**: Set language via `LANG` or `LC_ALL` environment variables
- **Command-Line Override**: Use `--lang` argument to specify language
- **UTF-8 Encoding**: All translations properly encoded in UTF-8
- **Backward Compatible**: Default behavior remains English output

## Building

```bash
make          # Build the program and compile translations
make clean    # Clean all build artifacts
```

## Usage

### Default (English)
```bash
./hello
# Output: Hello world!
```

### Using Environment Variables
```bash
LANG=es_ES.UTF-8 ./hello
# Output: ¡Hola mundo!

LANG=fr_FR.UTF-8 ./hello
# Output: Bonjour le monde!

LANG=de_DE.UTF-8 ./hello
# Output: Hallo Welt!

LANG=ja_JP.UTF-8 ./hello
# Output: こんにちは世界!
```

### Using Command-Line Argument
```bash
./hello --lang es_ES.UTF-8
# Output: ¡Hola mundo!

./hello --lang fr_FR.UTF-8
# Output: Bonjour le monde!
```

## Supported Languages

| Language | Code | Translation |
|----------|------|-------------|
| English (default) | en | Hello world! |
| Spanish | es_ES.UTF-8 | ¡Hola mundo! |
| French | fr_FR.UTF-8 | Bonjour le monde! |
| German | de_DE.UTF-8 | Hallo Welt! |
| Japanese | ja_JP.UTF-8 | こんにちは世界! |

## Testing

```bash
make test               # Run all validation tests
bash test/validate.sh   # Run validation tests with output
```

## Requirements

- GCC or Clang compiler
- **gettext tools** (optional - required only for building/updating translations):
  - `xgettext` - for extracting translatable strings
  - `msgfmt` - for compiling translation catalogs
  - `msginit`, `msgmerge` - for managing translation files
  - On Ubuntu/Debian: `sudo apt-get install gettext`
  - On macOS: `brew install gettext`
- UTF-8 locale support (for testing translations)

**Note**: The program will compile and run without gettext installed. Gettext is only needed if you want to build or update translations from source. Pre-compiled translation files (`.mo` files) are included in the repository.

## CI Setup

For CI/CD environments, additional setup may be required. See [CI_SETUP.md](CI_SETUP.md) for detailed instructions on configuring the CI workflow to support internationalization testing.

## Directory Structure

```
.
├── hello.c              # Main program with i18n support
├── Makefile             # Build system
├── po/                  # Translation source files (.po)
│   ├── hello.pot        # Translation template
│   ├── es/hello.po      # Spanish translations
│   ├── fr/hello.po      # French translations
│   ├── de/hello.po      # German translations
│   └── ja/hello.po      # Japanese translations
├── locale/              # Compiled translations (.mo)
│   ├── es/LC_MESSAGES/hello.mo
│   ├── fr/LC_MESSAGES/hello.mo
│   ├── de/LC_MESSAGES/hello.mo
│   └── ja/LC_MESSAGES/hello.mo
└── test/                # Test suite
    └── validate.sh      # Validation tests for all languages
```