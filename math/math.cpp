#include <CL/sycl.hpp>
#include <iostream>
#include <math.h>
 
namespace sycl = cl::sycl;
constexpr int LENGTH = 0x1FFFFFFF;
int data[LENGTH];
 
int main()
{
    // new block scope to ensure all SYCL tasks are completed before exiting block
    {
        // create a queue to enqueue work on cpu device (there is also gpu_selector)
        //sycl::queue myQueue(sycl::host_selector{});
        sycl::queue myQueue(sycl::gpu_selector{});

        // wrap the data variable in a buffer
        sycl::buffer<int, 1> resultBuf(data, sycl::range<1>(LENGTH));

        // submit commands to the queue
        myQueue.submit([&](sycl::handler& cgh) {
            // get access to the buffer for writing
            auto writeResult = resultBuf.get_access<sycl::access::mode::write>(cgh);
            // enqueue a parallel_for task: this is kernel function that will be
            // compiled by a device compiler and executed on a device
            cgh.parallel_for<class simple_test>(sycl::range<1>(LENGTH), [=](sycl::id<1> idx) {
                writeResult[idx[0]] = sycl::tan(static_cast<double>(idx[0])) * sycl::atan(static_cast<double>(idx[0]));
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
 
