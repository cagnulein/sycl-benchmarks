## System Used:
CPU: Intel(R) Core(TM) i7-7820EQ CPU @ 3.00GHz (family: 0x6, model: 0x9e, stepping: 0x9)

GPU: Intel HD Graphics 630 (Device ID 0x591B)

## Benchmarks
### fill_array
#### CPU 1 core with SYCL
real	1m0.262s
user	0m59.397s
sys	0m0.860s

### CPU 1 core without SYCL
real	0m3.620s
user	0m2.847s
sys	0m0.772s

#### GPU
real	0m9.376s
user	0m0.386s
sys	0m0.173s

### math
#### CPU 1 core with SYCL
real	3m3.545s
user	3m3.522s
sys	0m0.020s

### CPU 1 core without SYCL
real	1m51.823s
user	1m51.808s
sys	0m0.000s

#### GPU
real	0m43.278s
user	0m0.270s
sys	0m0.025s

### math x2
#### CPU 1 core with SYCL
real	3m56.758s
user	3m56.723s
sys	0m0.016s

### CPU 1 core without SYCL
real	1m12.483s
user	1m12.483s
sys	0m0.000s

#### GPU
real	0m4.820s
user	0m0.405s
sys	0m0.036s


## Resources:
1) http://pramodkumbhar.com/2019/07/intels-one-api-what-we-know-and-how-to-get-ready/
2) https://github.com/intel/llvm/releases/tag/expoclcpu-1.0.0
3) https://github.com/intel/llvm/releases/tag/2019-09
