source ../export.sh
g++ -O2 -g -o math_classic math_classic.cpp
clang++ -O2 -g -std=c++11 -fsycl math.cpp -lOpenCL -o math.gpu
