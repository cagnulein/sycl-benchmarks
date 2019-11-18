source ../../export.sh
clang++ -O2 -g -std=c++11 -fsycl rotate.cpp -lOpenCL -o rotate.gpu
g++ -std=c++0x -O2 -g rotate-opencl.cpp -o rotate-opencl.gpu -lOpenCL
