#include <CL/sycl.hpp>
#include <iostream>
#include <math.h>
#include <chrono>

#define IMAGE_WIDTH     (20000L)
#define IMAGE_HEIGHT    (40000L)
#define IMAGE_SIZE      (IMAGE_WIDTH*IMAGE_HEIGHT)

unsigned char* old_image;
namespace sycl = cl::sycl;

int main(int argc, char *argv[]) {
    old_image = new unsigned char[IMAGE_SIZE];
    {
        sycl::queue myQueue(sycl::gpu_selector{});
        sycl::buffer<unsigned char, 1> inputBuf(old_image, sycl::range<1>(IMAGE_WIDTH*IMAGE_HEIGHT));
        myQueue.submit([&](sycl::handler& cgh) {
            auto readImage = inputBuf.get_access<sycl::access::mode::read>(cgh);
            //cgh.parallel_for<class simple_test>(sycl::range<1>(1861303172), [=](sycl::id<1> idx) {  // FASTEST PERFORMANCE!!!
            cgh.parallel_for<class simple_test>(sycl::range<1>(1761303172), [=](sycl::id<1> idx) {    // SLOWER PERFORMANCE!!!
            });
        });
    }
    return 0;
}
 
