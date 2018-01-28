#include <iostream>
#include "../includes/dimensions.h"

const int LENGTH = IMAGE_SIZE;
int* writeResult;

int main()
{
	writeResult = (int*)malloc(LENGTH*sizeof(int));
	for(int i=0; i<LENGTH; i++)
		writeResult[i] = i;
    return 0;
}

