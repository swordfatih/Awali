OBJECTS := client server game
OUTPUT := awali
C := gcc
CFLAGS := -std=c11 -Wall
LDFLAGS := -lc
INCLUDE_DIR := include

main: $(OBJECTS)

.SECONDEXPANSION:

$(OBJECTS): src/$$@/*.c
	mkdir -p output
	$(C) -o output/$@ $^ -I $(INCLUDE_DIR)/$@

debug: CFLAGS := $(CFLAGS) -DMAP
debug: main

clean:
	rm -rf $(OBJECTS) $(OUTPUT)