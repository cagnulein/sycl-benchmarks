#include <cassert>
#include <iostream>
#include <utility>
#include <CL/sycl.hpp>
 
namespace sycl = cl::sycl;
 
sycl::string_class get_type(const sycl::device& dev) {
    if (dev.is_host()) {
        return "host";
    } else if (dev.is_gpu()) {
        return "OpenCL.GPU";
    } else if (dev.is_accelerator()) {
        return "OpenCL.ACC";
    } else {
        return "OpenCL.CPU";
    }
}
 
int main() {
    sycl::device d;
    std::cout << "Default device type: " << get_type(d) << std::endl;
 
    int i = 1;
    for (const auto& dev : sycl::device::get_devices()) {
        std::cout << "Device " << i++ << " is available of type: " << get_type(dev) << std::endl;
    }
}
 
