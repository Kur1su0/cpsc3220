#clear
make clean
make

gcc -o cooperative_test cooperative_test.c libmythreads.a
gcc -o preemtive_test preemtive_test.c libmythreads.a


gcc -o  test_list test_list.c libmythreads.a
./cooperative_test
#./preemtive_test
#./test_list
