#include <iostream>
#include <CL/cl.hpp>

#define IMAGE_SIZE (20000*40000)
unsigned char* old_image;
unsigned char* new_image;

int main(int argc, char *argv[]) {

    long DestBitmapWidth=22067;
    long DestBitmapHeight=40992;
    if(argc > 2)
    {
       DestBitmapWidth = atol(argv[1]);
       DestBitmapHeight = atol(argv[2]);
       //printf("%f degrees forced\n", degrees);
    }

    old_image = new unsigned char[IMAGE_SIZE];
    new_image = new unsigned char[DestBitmapWidth*DestBitmapHeight];

    // get all platforms (drivers), e.g. NVIDIA
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);

    if (all_platforms.size()==0) {
        std::cout<<" No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Platform default_platform=all_platforms[0];
    //std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    // get default device (CPUs, GPUs) of the default platform
    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size()==0){
        std::cout<<" No devices found. Check OpenCL installation!\n";
        exit(1);
    }

    // use device[1] because that's a GPU; device[0] is the CPU
    cl::Device default_device=all_devices[0];
    //std::cout<< "Using device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<"\n";

    // a context is like a "runtime link" to the device and platform;
    // i.e. communication is possible
    cl::Context context({default_device});

    // create the program that we want to execute on the device
    cl::Program::Sources sources;

    // calculates for each element; C = A + B
    std::string kernel_code=
        "   void kernel simple_add(global const int* A, global const int* B, global int* C, "
        "                          global const int* N) {"
        ""
        "   }";
    sources.push_back({kernel_code.c_str(), kernel_code.length()});

    cl::Program program(context, sources);
    if (program.build({default_device}) != CL_SUCCESS) {
        std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << std::endl;
        exit(1);
    }


    // create buffers on device (allocate space on GPU)
    cl::Buffer buffer_A(context, CL_MEM_READ_WRITE, sizeof(char) * IMAGE_SIZE);
    cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, sizeof(char) * DestBitmapWidth * DestBitmapHeight);

    // create a queue (a queue of commands that the GPU will execute)
    cl::CommandQueue queue(context, default_device);

    // push write commands to queue
    queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof(char)*IMAGE_SIZE, old_image);
    queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, sizeof(char)*DestBitmapWidth*DestBitmapHeight, new_image);

    // RUN ZE KERNEL
    cl::Kernel simple_add(program, "simple_add");
    simple_add.setArg(0, buffer_A);
    simple_add.setArg(1, buffer_B);
    queue.enqueueNDRangeKernel(simple_add,cl::NullRange,cl::NDRange(DestBitmapWidth*DestBitmapHeight),cl::NullRange);

    // read result from GPU to here
    queue.enqueueReadBuffer(buffer_A, CL_TRUE, 0, sizeof(char)*IMAGE_SIZE, old_image);
    queue.enqueueReadBuffer(buffer_B, CL_TRUE, 0, sizeof(char)*DestBitmapWidth*DestBitmapHeight, new_image);

    /*std::cout << "result: {";
    for (int i=0; i<n; i++) {
        std::cout << C[i] << " ";
    }
    std::cout << "}" << std::endl;*/

    return 0;
}


