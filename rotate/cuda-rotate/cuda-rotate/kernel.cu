
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "../../../includes/dimensions.h"
#include <stdio.h>
#include <math.h>

float cosine;
float sine;

float Point1x;
float Point1y;
float Point2x;
float Point2y;
float Point3x;
float Point3y;

float minx;
float miny;
float maxx;
float maxy;

long DestBitmapWidth;
long DestBitmapHeight;


cudaError_t rotateWithCuda(unsigned char *src, unsigned char *dest, unsigned int src_size, unsigned int dest_size);

__global__ void rotateKernel(unsigned char* src, unsigned char* dest, long DestBitmapWidth, long DestBitmapHeight, float sine, float cosine, float minx, float miny)
{
	int i = threadIdx.x;
	int x = i / DestBitmapWidth;
	int y = i % DestBitmapHeight;

	int SrcBitmapx = ((x + minx)*cosine + (y + miny)*sine);
	int SrcBitmapy = ((y + miny)*cosine - (x + minx)*sine);
	if (SrcBitmapx >= 0 && SrcBitmapx < IMAGE_WIDTH && SrcBitmapy >= 0 && SrcBitmapy < IMAGE_HEIGHT)
		dest[(x * DestBitmapWidth) + y] = src[(SrcBitmapx * IMAGE_WIDTH) + SrcBitmapy];
}

unsigned char* old_image;
unsigned char* new_image;

int main(int argc, char *argv[])
{
	old_image = new unsigned char[IMAGE_SIZE];

	double degrees = 180;
	if (argc > 1)
	{
		degrees = atof(argv[1]);
		//printf("%f degrees forced\n", degrees);
	}

	float radians = (2 * 3.1416*degrees) / 360;

	cosine = (float)cos(radians);
	sine = (float)sin(radians);

	Point1x = (-IMAGE_HEIGHT * sine);
	Point1y = (IMAGE_HEIGHT*cosine);
	Point2x = (IMAGE_WIDTH*cosine - IMAGE_HEIGHT * sine);
	Point2y = (IMAGE_HEIGHT*cosine + IMAGE_WIDTH * sine);
	Point3x = (IMAGE_WIDTH*cosine);
	Point3y = (IMAGE_WIDTH*sine);

	minx = min((float)0.0, min(Point1x, min(Point2x, Point3x)));
	miny = min((float)0.0, min(Point1y, min(Point2y, Point3y)));
	maxx = max(Point1x, max(Point2x, Point3x));
	maxy = max(Point1y, max(Point2y, Point3y));

	DestBitmapWidth = (int)ceil(fabs(maxx) - minx);
	DestBitmapHeight = (int)ceil(fabs(maxy) - miny);

	new_image = new unsigned char[DestBitmapWidth*DestBitmapHeight];

	// Add vectors in parallel.
	cudaError_t cudaStatus = rotateWithCuda(old_image, new_image, IMAGE_SIZE, DestBitmapWidth*DestBitmapHeight);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "rotateWithCuda failed!");
		return 1;
	}

	// cudaDeviceReset must be called before exiting in order for profiling and
	// tracing tools such as Nsight and Visual Profiler to show complete traces.
	cudaStatus = cudaDeviceReset();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceReset failed!");
		return 1;
	}

	return 0;
}

// Helper function for using CUDA to add vectors in parallel.
cudaError_t rotateWithCuda(unsigned char *src, unsigned char *dest, unsigned int src_size, unsigned int dest_size)
{
	unsigned char *dev_src = 0;
	unsigned char *dev_dst = 0;
	cudaError_t cudaStatus;

	// Choose which GPU to run on, change this on a multi-GPU system.
	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
		goto Error;
	}

	struct cudaDeviceProp properties;
	cudaGetDeviceProperties(&properties, 0);

	cudaStatus = cudaMalloc((void**)&dev_src, src_size * sizeof(unsigned char));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}

	cudaStatus = cudaMalloc((void**)&dev_dst, dest_size * sizeof(unsigned char));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}
	// Copy input vectors from host memory to GPU buffers.
	cudaStatus = cudaMemcpy(dev_src, src, src_size * sizeof(unsigned char), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		goto Error;
	}

	// Launch a kernel on the GPU with one thread for each element.
	rotateKernel<<<dest_size / 1024, 1024>>>(dev_src, dev_dst, DestBitmapWidth, DestBitmapHeight, sine, cosine, minx, miny);

	// Check for any errors launching the kernel
	cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
		goto Error;
	}
	
	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	cudaStatus = cudaDeviceSynchronize();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
		goto Error;
	}

	// Copy output vector from GPU buffer to host memory.
	cudaStatus = cudaMemcpy(dest, dev_dst, dest_size * sizeof(unsigned char), cudaMemcpyDeviceToHost);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		goto Error;
	}

Error:
	cudaFree(dev_src);
	cudaFree(dev_dst);
	
	return cudaStatus;
}
