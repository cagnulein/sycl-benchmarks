#include <CL/sycl.hpp>
#include <iostream>
#include <math.h>
#include <chrono>
#include "../../includes/dimensions.h"
 
unsigned char* old_image;
unsigned char* new_image;
namespace sycl = cl::sycl;

int main(int argc, char *argv[]) {
    old_image = new unsigned char[IMAGE_SIZE];
    long DestBitmapWidth=22067;
    long DestBitmapHeight=40992;
    if(argc > 2)
    {
       DestBitmapWidth = atol(argv[1]);
       DestBitmapHeight = atol(argv[2]);
       //printf("%f degrees forced\n", degrees);
    }

    new_image = new unsigned char[DestBitmapWidth*DestBitmapHeight];

    //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    // new block scope to ensure all SYCL tasks are completed before exiting block
    {
        // create a queue to enqueue work on cpu device (there is also gpu_selector)
        //sycl::queue myQueue(sycl::host_selector{});
        sycl::queue myQueue(sycl::gpu_selector{});

        // wrap the data variable in a buffer
        sycl::buffer<unsigned char, 2> resultBuf(new_image, sycl::range<2>(DestBitmapWidth,DestBitmapHeight));
        sycl::buffer<unsigned char, 2> inputBuf(old_image, sycl::range<2>(IMAGE_WIDTH,IMAGE_HEIGHT));

        // submit commands to the queue
        myQueue.submit([&](sycl::handler& cgh) {
            // get access to the buffer for writing
            auto writeResult = resultBuf.get_access<sycl::access::mode::write>(cgh);
            auto readImage = inputBuf.get_access<sycl::access::mode::read>(cgh);
            // enqueue a parallel_for task: this is kernel function that will be
            // compiled by a device compiler and executed on a device
            cgh.parallel_for<class simple_test>(sycl::range<1>(1), [=](sycl::id<1> idx) {
            });
            // end of the kernel function
        });
        // end of the queue commands
    }
    // end of scope, so wait for the queued work to complete
    //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;

    return 0;
}
 
