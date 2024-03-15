CC := gcc
CFLAGS := -Wall -Wextra -O0 -masm=intel -march=native
INCLUDES := -I.
objects := build/flush.o build/gates.o build/eval.o build/main.o build/cache_alloc.o

run: build/main
	./build/main

build/main: $(objects)
	$(CC) -o $@ $^

$(objects): build/%.o: src/%.c | build
	$(CC) $(CFLAGS) $(INCLUDES) $< -c -o $@ 

build:
	mkdir -p build

clean:
	rm -rf build
