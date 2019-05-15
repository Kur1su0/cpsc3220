


#CC=clang
CC=gcc -std=c99
CFLAGS=-Wall -g

BINS=leakcount memory_shim.so sctracer leakcount sctracer


all: $(BINS)

memory_shim.so:  memory_shim.c
	$(CC) $(CFLAGS) -fPIC -shared -o memory_shim.so memory_shim.c -ldl

leakcount: leakcount.c
	$(CC) $(CFLAGS) -o leakcount leakcount.c

#leaktest: leaktest.c
#	$(CC) $(CFLAGS) -o leaktest leaktest.c

sctracer: sctracer.c
	$(CC) $(CFLAGS) -o sctracer sctracer.c

#sctracertest: sctracertest.c
#	$(CC) $(CFLAGS) -o sctracertest sctracertest.c

clean:
	rm $(BINS)
