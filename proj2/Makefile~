CC=gcc
CFLAGS=-Wall -g

BINS=mythreads.o libmythreads.a test1 #cooperative_test


all: $(BINS)

mythreads.o:  mythreads.c mythreads.h
	$(CC) -Wall -c mythreads.c
libmythreads.a: mythreads.o
	ar -cvrs libmythreads.a mythreads.o


test1: test1.c libmythreads.a
	$(CC) -o test1 test1.c libmythreads.a
	
cooperative_test: cooperative_test.c libmythreads.a
	$(CC) -o cooperative_test cooperative_test.c libmythreads.a


clean:
	rm $(BINS)
