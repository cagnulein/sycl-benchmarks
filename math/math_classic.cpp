#include <iostream>
#include <math.h>
#include "../includes/dimensions.h"

const int LENGTH = IMAGE_SIZE;
int writeResult;

int main()
{
	for(int i=0; i<LENGTH; i++)
		writeResult = static_cast<double>(tan(i)) * static_cast<double>(atan(i));
	return 0;
}

