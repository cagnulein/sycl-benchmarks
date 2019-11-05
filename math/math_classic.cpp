#include <iostream>
#include <math.h>

const int LENGTH = 0x2FFFFFFF;
int writeResult;

int main()
{
	for(int i=0; i<LENGTH; i++)
		writeResult = static_cast<double>(tan(i)) * static_cast<double>(atan(i));
	return 0;
}

