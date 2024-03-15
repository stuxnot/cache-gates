CC := gcc
CFLAGS := -Wall -Wextra -O0 -masm=intel -march=native
INCLUDES := -I.
objects := build/flush.o build/gates.o build/eval.o build/bitmap.o build/cache_line_alloc.o

targets := build/main
test_targets := build/alloc-test build/bitmap-test

target_objs := $(addsuffix .o, $(targets))
test_objs := $(addsuffix .o, $(test_targets))

run: $(targets)
	@for f in $^; do echo "./$$f"; "./$$f"; done

test: $(test_targets)
	@for f in $^; do echo "./$$f"; "./$$f"; done

$(targets): build/%: build/%.o $(objects)
	$(CC) -o $@ $^

$(test_targets): build/%: build/%.o $(objects)
	$(CC) -o $@ $^

$(test_objs): build/%.o: tests/%.c | build
	$(CC) $(CFLAGS) $(INCLUDES) $< -c -o $@

$(objects) $(target_objs): build/%.o: src/%.c | build
	$(CC) $(CFLAGS) $(INCLUDES) $< -c -o $@ 

build:
	mkdir -p build

clean:
	rm -rf build
