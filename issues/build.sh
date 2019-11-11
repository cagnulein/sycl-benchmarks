source ../export.sh
clang++ -O2 -g -std=c++11 -fsycl rotate.cpp -lOpenCL -o rotate.gpu
