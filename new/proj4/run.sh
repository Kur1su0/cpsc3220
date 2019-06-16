make
./a.out < sample/trace1  > out/out1
./a.out -v < sample/trace1  > out/out1_v

./a.out < sample/trace2  > out/out2
./a.out -v < sample/trace2  > out/out2_v

./a.out < sample/trace3  > out/out3
./a.out -v < sample/trace3  > out/out3_v

diff out/out1 sample/trace1_out
diff out/out1_v sample/trace1_out_v

diff out/out2 sample/trace2_out
diff out/out2_v sample/trace2_out_v

diff out/out3 sample/trace3_out
diff out/out3_v sample/trace3_out_v

echo "identical!"
