#clear
make clean
make

gcc -o cooperative_test cooperative_test.c libmythreads.a

./cooperative_test
