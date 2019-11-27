source ../export.sh
g++ -O2 -g -o rotate_classic rotate_classic.cpp
g++ -O2 -g -o rotate_classic_omp rotate_classic_omp.cpp -fopenmp
clang++ -O2 -g -std=c++11 -fsycl rotate.cpp -lOpenCL -o rotate.gpu
