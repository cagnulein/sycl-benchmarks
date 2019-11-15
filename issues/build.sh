source ../export.sh
clang++ -O2 -g -std=c++11 -fsycl rotate.cpp -lOpenCL -o rotate.gpu
g++ -g -std=c++0x rotate-opencl.cpp -o rotate-opencl.gpu -lOpenCL
