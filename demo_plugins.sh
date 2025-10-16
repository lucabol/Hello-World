#!/bin/bash
# Demo script for the Hello World Plugin System
# 
# This script demonstrates the plugin architecture by showing:
# 1. Basic program execution without plugins
# 2. Single plugin usage
# 3. Plugin chaining
# 4. Multiple transformation examples

set -e

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${BLUE}============================================${NC}"
echo -e "${BLUE}Hello World Plugin System Demo${NC}"
echo -e "${BLUE}============================================${NC}"
echo ""

# Build if needed
if [ ! -f hello-plugin ] || [ ! -f plugins/uppercase_plugin.so ]; then
    echo -e "${YELLOW}Building hello-plugin and example plugins...${NC}"
    make hello-plugin plugins > /dev/null 2>&1
    echo -e "${GREEN}✓ Build complete${NC}"
    echo ""
fi

# Demo 1: No plugins
echo -e "${YELLOW}1. Basic execution (no plugins):${NC}"
echo -n "   Output: "
./hello-plugin
echo ""
echo ""

# Demo 2: Uppercase plugin
echo -e "${YELLOW}2. Uppercase transformation:${NC}"
echo -n "   Output: "
HELLO_PLUGINS=./plugins/uppercase_plugin.so ./hello-plugin
echo ""
echo ""

# Demo 3: Bracket plugin
echo -e "${YELLOW}3. Bracket formatting:${NC}"
echo -n "   Output: "
HELLO_PLUGINS=./plugins/bracket_plugin.so ./hello-plugin
echo ""
echo ""

# Demo 4: Reverse plugin
echo -e "${YELLOW}4. Reverse transformation:${NC}"
echo -n "   Output: "
HELLO_PLUGINS=./plugins/reverse_plugin.so ./hello-plugin
echo ""
echo ""

# Demo 5: Chain plugins
echo -e "${YELLOW}5. Chained plugins (bracket → uppercase):${NC}"
echo -n "   Output: "
HELLO_PLUGINS=./plugins/bracket_plugin.so,./plugins/uppercase_plugin.so ./hello-plugin
echo ""
echo ""

# Demo 6: Different order
echo -e "${YELLOW}6. Chained plugins (uppercase → bracket):${NC}"
echo -n "   Output: "
HELLO_PLUGINS=./plugins/uppercase_plugin.so,./plugins/bracket_plugin.so ./hello-plugin
echo ""
echo ""

# Demo 7: Three plugins
echo -e "${YELLOW}7. Three plugins (bracket → reverse → uppercase):${NC}"
echo -n "   Output: "
HELLO_PLUGINS=./plugins/bracket_plugin.so,./plugins/reverse_plugin.so,./plugins/uppercase_plugin.so ./hello-plugin
echo ""
echo ""

echo -e "${BLUE}============================================${NC}"
echo -e "${GREEN}Demo complete!${NC}"
echo ""
echo "See PLUGIN_SYSTEM.md for full documentation on:"
echo "  - Creating custom plugins"
echo "  - Plugin API reference"
echo "  - Advanced usage examples"
echo -e "${BLUE}============================================${NC}"
