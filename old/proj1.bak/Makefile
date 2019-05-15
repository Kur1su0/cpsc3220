
CC=clang
CFLAGS=-Wall -g

BINS=leakcount memory_shim.so


all: $(BINS)

memory_shim.so:  memory_shim.c
	$(CC) $(CFLAGS) -fPIC -shared -o memory_shim.so memory_shim.c -ldl

leakcount: leakcount.c
	$(CC) $(CFLAGS) -o leakcount leakcount.c

clean:
	rm $(BINS)
