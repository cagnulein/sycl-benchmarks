#include <CL/sycl.hpp>
#include <iostream>
#include <math.h>
#include <chrono>
#include "../includes/dimensions.h"

#define FIXED_DIMENSION 0
#define CORRECT_EVENESS 1
#define DIVIDED_BY_16   1
#define EVENESS_DIVISOR 10
#define VERIFY_INPUT	0
#define VERIFY		0
unsigned char* old_image;
unsigned char* new_image;
namespace sycl = cl::sycl;

unsigned char count_divisors(long n) {
    int count=0;
    const int limit=256;
    for (int i=1;i<=limit;i++)
        if (n%i==0)
            count++;
    //printf("%d %d\n", n, count);
    return count;
}

int main(int argc, char *argv[]) {
    int only_print=0;
    old_image = new unsigned char[IMAGE_SIZE];
    if(!old_image)
    {
       printf("old_image creation error!\n");
       return 1;
    }
#if VERIFY==1
    for(int i=0; i<IMAGE_WIDTH; i++)
    {
        for(int l=0; l<IMAGE_HEIGHT; l++)
        {
            old_image[(i*IMAGE_HEIGHT)+l] = i;
        }
    }
#if VERIFY_INPUT==1
    unsigned char old = old_image[0];
    int rep = 0;
    for(int i=0; i<IMAGE_WIDTH; i++)
    {
        for(int l=0; l<IMAGE_HEIGHT; l++)
        {
            if(old == old_image[(i*IMAGE_HEIGHT)+l])
                rep++;
            else
            {
                if(rep>0)
                   printf("%d. Char %d repeated for %d times\n", i, old, rep);
                old = old_image[(i*IMAGE_HEIGHT)+l];
                rep = 0;
            }
        }
    }
#endif
#endif

    double degrees = 180;
    if(argc > 1)
    {
       	degrees = atof(argv[1]);
       	//printf("%f degrees forced\n", degrees);
       	if(argc > 2 && atoi(argv[2])==1)
		only_print=1;
    }

    FLOAT_PRECISION radians=(2*3.1416*degrees)/360;

    FLOAT_PRECISION cosine=(FLOAT_PRECISION)cos(radians);
    FLOAT_PRECISION sine=(FLOAT_PRECISION)sin(radians);

    FLOAT_PRECISION Point1x=(-IMAGE_HEIGHT*sine);
    FLOAT_PRECISION Point1y=(IMAGE_HEIGHT*cosine);
    FLOAT_PRECISION Point2x=(IMAGE_WIDTH*cosine-IMAGE_HEIGHT*sine);
    FLOAT_PRECISION Point2y=(IMAGE_HEIGHT*cosine+IMAGE_WIDTH*sine);
    FLOAT_PRECISION Point3x=(IMAGE_WIDTH*cosine);
    FLOAT_PRECISION Point3y=(IMAGE_WIDTH*sine);

    FLOAT_PRECISION minx=std::min((FLOAT_PRECISION)0.0,std::min(Point1x,std::min(Point2x,Point3x)));
    FLOAT_PRECISION miny=std::min((FLOAT_PRECISION)0.0,std::min(Point1y,std::min(Point2y,Point3y)));
    FLOAT_PRECISION maxx=std::max(Point1x,std::max(Point2x,Point3x));
    FLOAT_PRECISION maxy=std::max(Point1y,std::max(Point2y,Point3y));

    long DestBitmapWidth=(int)ceil(fabs(maxx)-minx);
    long DestBitmapHeight=(int)ceil(fabs(maxy)-miny);

#if CORRECT_EVENESS==1
    if(DestBitmapWidth%2)
       DestBitmapWidth++;
    if(DestBitmapHeight%2)
       DestBitmapHeight++;

#if DIVIDED_BY_16==0
    while(count_divisors(DestBitmapWidth*DestBitmapHeight)<EVENESS_DIVISOR || count_divisors(DestBitmapHeight)<EVENESS_DIVISOR || count_divisors(DestBitmapWidth)<EVENESS_DIVISOR) {
#else
    while((DestBitmapWidth*DestBitmapHeight)%16) {
#endif
	    if((++DestBitmapWidth)%2)
	       DestBitmapWidth++;
	    if((++DestBitmapHeight)%2)
	       DestBitmapHeight++;
    }
#endif

    if(only_print) {
       printf("%ld %ld %ld\n",DestBitmapWidth,DestBitmapHeight,DestBitmapWidth*DestBitmapHeight);
       return 0;
    }

#if FIXED_DIMENSION==1
    DestBitmapWidth=45000;
    DestBitmapHeight=45000;
#endif

    new_image = new unsigned char[DestBitmapWidth*DestBitmapHeight];
    if(!new_image)
    {
       printf("new_image creation error!\n");
       return 1;
    }
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
            auto writeResult = resultBuf.get_access<sycl::access::mode::discard_write>(cgh);
            auto readImage = inputBuf.get_access<sycl::access::mode::read>(cgh);
            // enqueue a parallel_for task: this is kernel function that will be
            // compiled by a device compiler and executed on a device
            cgh.parallel_for<class simple_test>(sycl::range<2>(DestBitmapWidth,DestBitmapHeight), [=](sycl::id<2> idx) {
                int x = idx[0]; //idx[0] / DestBitmapHeight;
                int y = idx[1]; //idx[0] % DestBitmapHeight;

                int SrcBitmapx=(int)((x+minx)*cosine+(y+miny)*sine);
                int SrcBitmapy=(int)((y+miny)*cosine-(x+minx)*sine);
                if(SrcBitmapx >= 0 && SrcBitmapx < IMAGE_WIDTH && SrcBitmapy >= 0 && SrcBitmapy < IMAGE_HEIGHT)
                   writeResult[x][y]=readImage[SrcBitmapx][SrcBitmapy];

            });
            // end of the kernel function
        });
        // end of the queue commands
    }
    // end of scope, so wait for the queued work to complete
    //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;

#if VERIFY==1
#if VERIFY_INPUT==0
    unsigned char old;
    unsigned int rep;
#endif
    old = new_image[0];
    rep = 0;
    for(int i=0; i<DestBitmapWidth; i++)
    {
        for(int l=0; l<DestBitmapHeight; l++)
        {
            if(old == new_image[(i*DestBitmapHeight)+l])
                rep++;
            else
            {
                //if(rep>0)
                   printf("%d. Char %d repeated for %d times\n", i, old, rep);
                old = new_image[(i*DestBitmapHeight)+l];
                rep = 0;
            }
        }
    }
#endif

    return 0;
}
 
