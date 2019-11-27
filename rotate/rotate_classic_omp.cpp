#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <chrono>
#include <iostream>
#include "../includes/dimensions.h"
#include <omp.h>

#define VERIFY		0
unsigned char** old_image;
unsigned char** new_image;

int main(int argc, char *argv[]) {
    old_image = new unsigned char*[IMAGE_WIDTH];
    for(int i=0; i<IMAGE_WIDTH; i++)
    {
        old_image[i] = new unsigned char[IMAGE_HEIGHT];
#if VERIFY==1
        for(int l=0; l<IMAGE_HEIGHT; l++)
        {
            old_image[i][l] = i;
        }
#endif
    }

    double degrees = 180;
    if(argc > 1)
    {
       degrees = atof(argv[1]);
       //printf("%f degrees forced\n", degrees);
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

    int DestBitmapWidth=(int)ceil(fabs(maxx)-minx);
    int DestBitmapHeight=(int)ceil(fabs(maxy)-miny);

    new_image = new unsigned char*[DestBitmapWidth];
    for(int i=0; i<DestBitmapWidth; i++)
        new_image[i] = new unsigned char[DestBitmapHeight];

//    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
#pragma omp parallel for
    for(int x=0;x<DestBitmapWidth;x++)
    {
       for(int y=0;y<DestBitmapHeight;y++)
       {
          int SrcBitmapx=(int)((x+minx)*cosine+(y+miny)*sine);
          int SrcBitmapy=(int)((y+miny)*cosine-(x+minx)*sine);
          if(SrcBitmapx>=0&&SrcBitmapx<IMAGE_WIDTH&&SrcBitmapy>=0&& SrcBitmapy<IMAGE_HEIGHT)
              new_image[x][y]=old_image[SrcBitmapx][SrcBitmapy];
       }
    }
//    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;

#if VERIFY==1
    unsigned char old = new_image[0][0];
    int rep = 0;
    for(int i=0; i<DestBitmapWidth; i++)
    {
        for(int l=0; l<DestBitmapHeight; l++)
        {
            if(old == new_image[i][l])
                rep++;
            else
            {
                if(rep>0)
                   printf("%d. Char %d repeated for %d times\n", i, old, rep);
                old = new_image[i][l];
                rep = 0;
            }
        }
    }
#endif

    delete old_image;
    delete new_image;
    return 0;
}
