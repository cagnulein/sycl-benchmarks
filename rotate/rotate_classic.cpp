#include <stdio.h>
#include <math.h>

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

    for (int x = 0; x < IMAGE_WIDTH; x++) {
        for (int y = 0; y < IMAGE_HEIGHT; y++) {
            float xPos = _cos * (x - x0) - _sin * (y - y0) + x0;
            float yPos = _sin * (x - x0) + _cos * (y - y0) + y0;

            if (xPos >= 0 && yPos >= 0 && xPos < IMAGE_WIDTH && yPos < IMAGE_HEIGHT)
             new_image[y * IMAGE_WIDTH + x] = old_image[  (int)(floor(yPos) * IMAGE_WIDTH + floor(xPos)) ];
        }
    }

    delete old_image;
    delete new_image;
    return 0;
}
