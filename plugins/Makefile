# Makefile for sample plugins
CC ?= gcc
CFLAGS = -Wall -Wextra -std=c99 -fPIC -shared -I..
PLUGIN_DIR = .

# Plugin source files
UPPERCASE_SRC = examples/uppercase_filter.c
TIMESTAMP_SRC = examples/timestamp_postprocessor.c
BANNER_SRC = examples/banner_preprocessor.c

# Plugin targets
PLUGINS = uppercase_filter.so timestamp_postprocessor.so banner_preprocessor.so

all: $(PLUGINS)

uppercase_filter.so: $(UPPERCASE_SRC)
	$(CC) $(CFLAGS) -o $(PLUGIN_DIR)/$@ $(UPPERCASE_SRC)

timestamp_postprocessor.so: $(TIMESTAMP_SRC)
	$(CC) $(CFLAGS) -o $(PLUGIN_DIR)/$@ $(TIMESTAMP_SRC)

banner_preprocessor.so: $(BANNER_SRC)
	$(CC) $(CFLAGS) -o $(PLUGIN_DIR)/$@ $(BANNER_SRC)

clean:
	rm -f $(PLUGIN_DIR)/*.so

.PHONY: all clean