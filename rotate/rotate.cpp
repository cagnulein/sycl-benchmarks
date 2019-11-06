#include <CL/sycl.hpp>
#include <iostream>
#include <math.h>
 
#define IMAGE_WIDTH     (20000L)
#define IMAGE_HEIGHT    (40000L)
#define IMAGE_SIZE      (IMAGE_WIDTH*IMAGE_HEIGHT)
unsigned char* old_image;
unsigned char* new_image;
namespace sycl = cl::sycl;

int main(void) {
    old_image = new unsigned char[IMAGE_SIZE];
    if(!old_image)
    {
       printf("old_image creation error!\n");
       return 1;
    }
    new_image = new unsigned char[IMAGE_SIZE];
    if(!new_image)
    {
       printf("new_image creation error!\n");
       return 1;
    }

    double degrees = 180;

    const float _cos = cos(degrees);
    const float _sin = sin(degrees);

    const int x0 = floor(IMAGE_WIDTH / 2);
    const int y0 = floor(IMAGE_HEIGHT / 2);

    // new block scope to ensure all SYCL tasks are completed before exiting block
    {
        // create a queue to enqueue work on cpu device (there is also gpu_selector)
        //sycl::queue myQueue(sycl::host_selector{});
        sycl::queue myQueue(sycl::gpu_selector{});

        // wrap the data variable in a buffer
        sycl::buffer<unsigned char, 1> resultBuf(new_image, sycl::range<1>(IMAGE_SIZE));
        sycl::buffer<unsigned char, 1> inputBuf(old_image, sycl::range<1>(IMAGE_SIZE));

        // submit commands to the queue
        myQueue.submit([&](sycl::handler& cgh) {
            // get access to the buffer for writing
            auto writeResult = resultBuf.get_access<sycl::access::mode::write>(cgh);
            auto readImage = inputBuf.get_access<sycl::access::mode::read>(cgh);
            // enqueue a parallel_for task: this is kernel function that will be
            // compiled by a device compiler and executed on a device
            cgh.parallel_for<class simple_test>(sycl::range<1>(IMAGE_SIZE), [=](sycl::id<1> idx) {
                int x = idx[0] / IMAGE_HEIGHT;
                int y = idx[0] % IMAGE_WIDTH;
                float xPos = _cos * (x - x0) - _sin * (y - y0) + x0;
                float yPos = _sin * (x - x0) + _cos * (y - y0) + y0;

                if (xPos >= 0 && yPos >= 0 && xPos < IMAGE_WIDTH && yPos < IMAGE_HEIGHT)
                    writeResult[y * IMAGE_WIDTH + x] = readImage[  (int)(sycl::floor(yPos) * IMAGE_WIDTH + sycl::floor(xPos)) ];

            });
            // end of the kernel function
        });
        // end of the queue commands
    }
    // end of scope, so wait for the queued work to complete

    // buffer will be up-to-date, print result
    /*for (long int i = 0; i < LENGTH; i++) {
        std::cout << "data[" << i << "] = " << data[i] << std::endl;
    }*/
    return 0;
}
 
