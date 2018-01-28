source ../export.sh
g++ -O2 -g -o fill_array_classic fill_array_classic.cpp
clang++ -O2 -g -std=c++11 -fsycl fill_array.cpp -lOpenCL -o fill_array.gpu
