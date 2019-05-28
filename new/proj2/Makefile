CC=gcc
flag=Wall

BINS=a.out



plock: main.c plock.c
	gcc -Wall plock.c main.c -pthread


#sctracertest: sctracertest.c
#	$(CC) $(CFLAGS) -o sctracertest sctracertest.c

run: a.out
	./a.out
  

check_leak: a.out
	echo "Mem Check..."
	valgrind --tool=memcheck ./a.out
	echo "Helgrind..."
	valgrind --tool=helgrind ./a.out

clean:
	rm $(BINS)
