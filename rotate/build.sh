source ../export.sh
g++ -O0 -g -o rotate_classic rotate_classic.cpp
clang++ -std=c++11 -fsycl rotate.cpp -lOpenCL -o rotate.gpu
