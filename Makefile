OBJECTS := client server
OUTPUT := awali
C := gcc
CFLAGS := -std=c11 -Wall
LDFLAGS := -lc
INCLUDE_DIR := include

main: $(OBJECTS)

.SECONDEXPANSION:

$(OBJECTS): src/$$@/*.c
	mkdir -p output
	$(C) -o $(subst src/$@, output, $(patsubst %.c, %, $^)) $^ -I $(INCLUDE_DIR)/$@

debug: CFLAGS := $(CFLAGS) -DMAP
debug: main

clean:
	rm -rf $(OBJECTS) $(OUTPUT)