# Makefile for hello world with i18n support

CC = gcc
CFLAGS = -Wall -Wextra
PACKAGE = hello
LOCALEDIR = $(PWD)/locale
LANGUAGES = es fr de ja

# Main targets
all: hello
	@$(MAKE) translations 2>/dev/null || echo "Note: Skipping translations (gettext tools not available)"

hello: hello.c
	$(CC) $(CFLAGS) -DLOCALEDIR=\"$(LOCALEDIR)\" -o hello hello.c

# Translation targets
po/$(PACKAGE).pot: hello.c
	@command -v xgettext >/dev/null 2>&1 || { echo "Error: xgettext not found. Install gettext to build translations."; exit 1; }
	mkdir -p po
	xgettext --keyword=_ --language=C --add-comments --sort-output \
		-o po/$(PACKAGE).pot hello.c

translations: po/$(PACKAGE).pot
	@command -v msgfmt >/dev/null 2>&1 || { echo "Error: msgfmt not found. Install gettext to build translations."; exit 1; }
	@for lang in $(LANGUAGES); do \
		mkdir -p po/$$lang; \
		if [ ! -f po/$$lang/$(PACKAGE).po ]; then \
			command -v msginit >/dev/null 2>&1 || { echo "Error: msginit not found. Install gettext to build translations."; exit 1; }; \
			msginit --input=po/$(PACKAGE).pot --locale=$$lang \
				--output=po/$$lang/$(PACKAGE).po --no-translator; \
		else \
			command -v msgmerge >/dev/null 2>&1 || { echo "Error: msgmerge not found. Install gettext to build translations."; exit 1; }; \
			msgmerge --update po/$$lang/$(PACKAGE).po po/$(PACKAGE).pot; \
		fi; \
		mkdir -p locale/$$lang/LC_MESSAGES; \
		msgfmt -o locale/$$lang/LC_MESSAGES/$(PACKAGE).mo \
			po/$$lang/$(PACKAGE).po; \
	done

# Test targets
test: all
	@echo "Running validation tests..."
	@bash test/validate.sh

unit-test: hello
	@echo "Running unit tests..."
	@if [ -f test/test_hello.c ]; then \
		$(CC) $(CFLAGS) -I. -c -o hello_lib.o hello.c -DUNIT_TEST; \
		$(CC) $(CFLAGS) -I. -o test_hello_runner test/test_hello.c hello_lib.o; \
		./test_hello_runner; \
		rm -f hello_lib.o test_hello_runner; \
	else \
		echo "Unit tests not yet implemented"; \
	fi

# Build variants
strict: hello.c
	$(CC) $(CFLAGS) -Wpedantic -Werror -DLOCALEDIR=\"$(LOCALEDIR)\" -o hello hello.c

debug: hello.c
	$(CC) $(CFLAGS) -g -DLOCALEDIR=\"$(LOCALEDIR)\" -o hello_debug hello.c

clang: hello.c
	clang $(CFLAGS) -DLOCALEDIR=\"$(LOCALEDIR)\" -o hello_clang hello.c

# Clean targets
clean:
	rm -f hello hello_debug hello_clang hello_strict *.o
	rm -rf po locale

clean-all: clean
	rm -rf test_hello_runner

.PHONY: all translations test unit-test strict debug clang clean clean-all
