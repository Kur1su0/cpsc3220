
CC=clang
CFLAGS=-Wall -g

BINS=mystery mystery2 mystery3 mystery4 shimtest shim.so


all: $(BINS)

mystery: mystery.c
	$(CC) $(CFLAGS) -o mystery mystery.c

mystery2: mystery2.c
	$(CC) $(CFLAGS) -o mystery2 mystery2.c

mystery3: mystery3.c
	$(CC) $(CFLAGS) -o mystery3 mystery3.c

mystery4: mystery4.c
	$(CC) $(CFLAGS) -o mystery4 mystery4.c

shim.so:  shim.c
	$(CC) $(CFLAGS) -fPIC -shared -o shim.so shim.c -ldl

shimtest: shimtest.c
	$(CC) $(CFLAGS) -o shimtest shimtest.c

clean:
	rm $(BINS)
