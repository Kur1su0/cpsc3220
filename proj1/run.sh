#$1
make
LD_PRELOAD=./memory_shim.so ./leakcount $1
