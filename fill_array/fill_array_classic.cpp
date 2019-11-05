#include <iostream>

const int LENGTH = 0x2FFFFFFF;
int* writeResult;

int main()
{
	writeResult = (int*)malloc(LENGTH*sizeof(int));
	for(int i=0; i<LENGTH; i++)
		writeResult[i] = i;
    return 0;
}

