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

    double degrees = 180;

    float radians=(2*3.1416*degrees)/360;

    float cosine=(float)cos(radians);
    float sine=(float)sin(radians);

    float Point1x=(-IMAGE_HEIGHT*sine);
    float Point1y=(IMAGE_HEIGHT*cosine);
    float Point2x=(IMAGE_WIDTH*cosine-IMAGE_HEIGHT*sine);
    float Point2y=(IMAGE_HEIGHT*cosine+IMAGE_WIDTH*sine);
    float Point3x=(IMAGE_WIDTH*cosine);
    float Point3y=(IMAGE_WIDTH*sine);

    float minx=std::min((float)0.0,std::min(Point1x,std::min(Point2x,Point3x)));
    float miny=std::min((float)0.0,std::min(Point1y,std::min(Point2y,Point3y)));
    float maxx=std::max(Point1x,std::max(Point2x,Point3x));
    float maxy=std::max(Point1y,std::max(Point2y,Point3y));

    int DestBitmapWidth=(int)ceil(fabs(maxx)-minx);
    int DestBitmapHeight=(int)ceil(fabs(maxy)-miny);

    new_image = new unsigned char[DestBitmapWidth*DestBitmapHeight];
    if(!new_image)
    {
       printf("new_image creation error!\n");
       return 1;
    }

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
            cgh.parallel_for<class simple_test>(sycl::range<1>(DestBitmapWidth*DestBitmapHeight), [=](sycl::id<1> idx) {
                int x = idx[0] / DestBitmapHeight;
                int y = idx[0] % DestBitmapHeight;

                int SrcBitmapx=(int)((x+minx)*cosine+(y+miny)*sine);
                int SrcBitmapy=(int)((y+miny)*cosine-(x+minx)*sine);
                if(SrcBitmapx>=0&&SrcBitmapx<IMAGE_WIDTH&&SrcBitmapy>=0&& SrcBitmapy<IMAGE_HEIGHT)
                   writeResult[x*y]=readImage[SrcBitmapx*SrcBitmapy];

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
 