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
- gettext tools (for building translations)
- UTF-8 locale support

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