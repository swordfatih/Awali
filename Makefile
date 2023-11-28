OBJECTS := client server game
OUTPUT := awali
C := gcc
CFLAGS := -std=c11 -Wall
LDFLAGS := -lc
INCLUDE_DIR := include
SOURCE_DIR := src

main: $(OBJECTS)

.SECONDEXPANSION:

$(OBJECTS): $(SOURCE_DIR)/$$@/*.c
	mkdir -p output
	$(C) $(CFLAGS) -o output/$@ $^ -I $(INCLUDE_DIR)/$@ -I $(INCLUDE_DIR)/shared

debug: CFLAGS := $(CFLAGS) -DMAP
debug: main

clean:
	rm -rf output