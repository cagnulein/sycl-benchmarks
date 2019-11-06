#include <stdio.h>
#include <math.h>
#include <algorithm>

#define IMAGE_WIDTH	(20000L)
#define IMAGE_HEIGHT	(40000L)
#define IMAGE_SIZE	(IMAGE_WIDTH*IMAGE_HEIGHT)
unsigned char* old_image;
unsigned char* new_image;

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

    for(int x=0;x<DestBitmapWidth;x++)
    {
       for(int y=0;y<DestBitmapHeight;y++)
       {
          int SrcBitmapx=(int)((x+minx)*cosine+(y+miny)*sine);
          int SrcBitmapy=(int)((y+miny)*cosine-(x+minx)*sine);
          if(SrcBitmapx>=0&&SrcBitmapx<IMAGE_WIDTH&&SrcBitmapy>=0&& SrcBitmapy<IMAGE_HEIGHT)
              new_image[x*y]=old_image[SrcBitmapx*SrcBitmapy]; 
       }
    }

    delete old_image;
    delete new_image;
    return 0;
}
