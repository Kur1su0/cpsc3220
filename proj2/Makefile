CC=gcc
CFLAGS=-Wall -g

BINS=mythreads.o libmythreads.a


all: $(BINS)

mythreads.o:  mythreads.c mythreads.h
	$(CC) -Wall -c mythreads.c
libmythreads.a: mythreads.o
	ar -cvrs libmythreads.a mythreads.o
	

clean:
	rm $(BINS)
