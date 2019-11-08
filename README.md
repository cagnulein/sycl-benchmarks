## System Used:
CPU: Intel(R) Core(TM) i7-7820EQ CPU @ 3.00GHz (family: 0x6, model: 0x9e, stepping: 0x9)

GPU: Intel HD Graphics 630 (Device ID 0x591B)

## Benchmarks
### fill_array
#### cpu 1 core

real    0m1.426s
user    0m0.589s
sys     0m0.837s
#### gpu

real    0m1.293s
user    0m0.368s
sys     0m0.924s


### math
#### cpu 1 core

real    1m26.930s
user    1m26.925s
sys     0m0.004s
#### gpu

real    0m5.833s
user    0m0.170s
sys     0m0.070s


### math_x2
#### cpu 1 core

real    2m10.083s
user    2m10.070s
sys     0m0.004s
#### gpu

real    0m7.533s
user    0m0.214s
sys     0m0.033s


### rotate
#### cpu 1 core

real    0m2.440s
user    0m2.087s
sys     0m0.352s
#### gpu

real    0m1.801s
user    0m0.301s
sys     0m0.147s

## Resources:
1) http://pramodkumbhar.com/2019/07/intels-one-api-what-we-know-and-how-to-get-ready/
2) https://github.com/intel/llvm/releases/tag/expoclcpu-1.0.0
3) https://github.com/intel/llvm/releases/tag/2019-09
4) http://www.leunen.com/cbuilder/rotbmp.html
