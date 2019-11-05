#include <iostream>
#include <math.h>

const int LENGTH = 0x1FFFFFFF;
int writeResult[2];

int main()
{
	for(int i=0; i<LENGTH; i++)
        {
		writeResult[0] = static_cast<double>(tan(i)) * static_cast<double>(atan(i));
		writeResult[1] = static_cast<double>(tan(writeResult[0])) * static_cast<double>(atan(writeResult[0]));
        }
	return 0;
}

