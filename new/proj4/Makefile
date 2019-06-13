CC=gcc
flag=Wall

BINS=a.out


all: paging run

paging: paging.c
	gcc -Wall paging.c


#sctracertest: sctracertest.c
#	$(CC) $(CFLAGS) -o sctracertest sctracertest.c

run: a.out
	./a.out

clean:
	rm $(BINS)
