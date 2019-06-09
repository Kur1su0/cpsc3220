CC=gcc
flag=Wall

BINS=a.out



hybrid: hybrid.c main.c
	gcc -g -Wall hybrid.c main.c


#sctracertest: sctracertest.c
#	$(CC) $(CFLAGS) -o sctracertest sctracertest.c

run: a.out
	./a.out

clean:
	rm $(BINS)
