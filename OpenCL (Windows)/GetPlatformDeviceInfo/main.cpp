/*
* Retrive information of OpenCL platforms and devices.
* Created by sappho192 at 2017.Apr.15
*/

#include <iostream>

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include "CL/cl.h"

inline void check_error(cl_int error, char* name) {
	if (error != CL_SUCCESS) {
		fprintf(stderr, "Non-successful return code %d for %s.  Exiting.\n", error, name);
		exit(1);
	}
}

void printPlatformInfo(cl_int& err, cl_uint nplatforms, _cl_platform_id** platforms);
void printDeviceInfo(_cl_platform_id* platform);

int main()
{
	cl_int err = 0;

	// Discover the number of platforms:
	cl_uint nplatforms;
	err = clGetPlatformIDs(0, nullptr, &nplatforms);
	check_error(err, "clGetPlatformIds");

	// Now ask OpenCL for the platform IDs:
	auto platforms = static_cast<cl_platform_id*>(malloc(sizeof(cl_platform_id) * nplatforms));
	err = clGetPlatformIDs(nplatforms, platforms, nullptr);
	check_error(err, "clGetPlatformIds");
	fprintf(stdout, "OpenCL reports %d platforms.\n\n", nplatforms);

	printPlatformInfo(err, nplatforms, platforms);

	free(platforms);

	return 0;
}

void printPlatformInfo(cl_int& err, cl_uint nplatforms, _cl_platform_id** platforms)
{
	for (cl_uint i = 0; i < nplatforms; i++) {
		char * name = nullptr;
		char * profile = nullptr;
		char * vendor = nullptr;
		char * version = nullptr;
		char * extensions = nullptr;

		size_t ret_size = 0;
		clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, 0, vendor, &ret_size);
		if (ret_size) {
			vendor = new char[ret_size];
			err |= clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, ret_size, vendor, &ret_size);
		}

		ret_size = 0;
		clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 0, name, &ret_size);
		if (ret_size) {
			name = new char[ret_size];
			err |= clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, ret_size, name, &ret_size);
		}

		ret_size = 0;
		clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, 0, version, &ret_size);
		if (ret_size) {
			version = new char[ret_size];
			err |= clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, ret_size, version, &ret_size);
		}

		ret_size = 0;
		clGetPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, 0, profile, &ret_size);
		if (ret_size) {
			profile = new char[ret_size];
			err |= clGetPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, ret_size, profile, &ret_size);
		}

		ret_size = 0;
		clGetPlatformInfo(platforms[i], CL_DEVICE_EXTENSIONS, 0, nullptr, &ret_size);
		if (ret_size) {
			extensions = new char[ret_size];
			err |= clGetPlatformInfo(platforms[i], CL_DEVICE_EXTENSIONS, ret_size, extensions, &ret_size);
		}
		check_error(err, "clGetPlatformInfo");

		printf(
			"[Platform %d]\nVendor: %s\nName: %s\nVersion: %s\nProfile: %s\nExtensions: %s\n",
			i, vendor, name, version, profile, extensions);

		if (vendor != nullptr) { delete[] vendor; }
		if (name != nullptr) { delete[] name; }
		if (version != nullptr) { delete[] version; }
		if (profile != nullptr) { delete[] profile; }
		if (extensions != nullptr) { delete[] extensions; }

		printDeviceInfo(platforms[i]);
		std::cout << std::endl;
	}
}

void printDeviceInfo(_cl_platform_id* platform)
{
	cl_int err;
	cl_uint ret_num_devices;
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 0, nullptr, &ret_num_devices);
	auto device_ids = static_cast<cl_device_id*>(malloc(sizeof(cl_device_id) * ret_num_devices));

	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, device_ids, &ret_num_devices);
	check_error(err, "clGetDeviceIDs");
	for (cl_uint i = 0; i < ret_num_devices; i++)
	{
		cl_device_type type = 0;
		char * name = nullptr;
		char * vendor = nullptr;
		cl_bool available = CL_FALSE;
		cl_uint units = 0;
		cl_uint address_bits = 0;
		cl_ulong max_mem = 0;
		cl_bool uni_mem = CL_FALSE;
		char * driver_version = nullptr;
		char * device_version = nullptr;
		char * extensions = nullptr;
		char * built_in_kernels = nullptr;

		size_t ret_size = 0;
		clGetDeviceInfo(device_ids[i], CL_DEVICE_TYPE, 0, nullptr, &ret_size);
		if(ret_size)
		{
			err |= clGetDeviceInfo(device_ids[i], CL_DEVICE_TYPE, ret_size, &type, &ret_size);
		}

		ret_size = 0;
		clGetDeviceInfo(device_ids[i], CL_DEVICE_NAME, 0, nullptr, &ret_size);
		if (ret_size)
		{
			name = new char[ret_size];
			err |= clGetDeviceInfo(device_ids[i], CL_DEVICE_NAME, ret_size, name, &ret_size);
		}

		ret_size = 0;
		clGetDeviceInfo(device_ids[i], CL_DEVICE_VENDOR, 0, nullptr, &ret_size);
		if (ret_size)
		{
			vendor = new char[ret_size];
			err |= clGetDeviceInfo(device_ids[i], CL_DEVICE_VENDOR, ret_size, vendor, &ret_size);
		}

		ret_size = 0;
		clGetDeviceInfo(device_ids[i], CL_DEVICE_AVAILABLE, 0, nullptr, &ret_size);
		if (ret_size)
		{
			err |= clGetDeviceInfo(device_ids[i], CL_DEVICE_AVAILABLE, ret_size, &available, &ret_size);
		}

		ret_size = 0;
		clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_COMPUTE_UNITS, 0, nullptr, &ret_size);
		if (ret_size)
		{
			err |= clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_COMPUTE_UNITS, ret_size, &units, &ret_size);
		}

		ret_size = 0;
		clGetDeviceInfo(device_ids[i], CL_DEVICE_ADDRESS_BITS, 0, nullptr, &ret_size);
		if (ret_size)
		{
			err |= clGetDeviceInfo(device_ids[i], CL_DEVICE_ADDRESS_BITS, ret_size, &address_bits, &ret_size);
		}

		ret_size = 0;
		clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_MEM_ALLOC_SIZE, 0, nullptr, &ret_size);
		if (ret_size)
		{
			err |= clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_MEM_ALLOC_SIZE, ret_size, &max_mem, &ret_size);
		}

		ret_size = 0;
		clGetDeviceInfo(device_ids[i], CL_DEVICE_HOST_UNIFIED_MEMORY, 0, nullptr, &ret_size);
		if (ret_size)
		{
			err |= clGetDeviceInfo(device_ids[i], CL_DEVICE_HOST_UNIFIED_MEMORY, ret_size, &uni_mem, &ret_size);
		}

		ret_size = 0;
		clGetDeviceInfo(device_ids[i], CL_DRIVER_VERSION, 0, nullptr, &ret_size);
		if (ret_size)
		{
			driver_version = new char[ret_size];
			err |= clGetDeviceInfo(device_ids[i], CL_DRIVER_VERSION, ret_size, driver_version, &ret_size);
		}

		ret_size = 0;
		clGetDeviceInfo(device_ids[i], CL_DEVICE_VERSION, 0, nullptr, &ret_size);
		if (ret_size)
		{
			device_version = new char[ret_size];
			err |= clGetDeviceInfo(device_ids[i], CL_DEVICE_VERSION, ret_size, device_version, &ret_size);
		}

		ret_size = 0;
		clGetDeviceInfo(device_ids[i], CL_DEVICE_EXTENSIONS, 0, nullptr, &ret_size);
		if (ret_size)
		{
			extensions = new char[ret_size];
			err |= clGetDeviceInfo(device_ids[i], CL_DEVICE_EXTENSIONS, ret_size, extensions, &ret_size);
		}
		
		ret_size = 0;
		clGetDeviceInfo(device_ids[i], CL_DEVICE_BUILT_IN_KERNELS, 0, nullptr, &ret_size);
		if (ret_size)
		{
			built_in_kernels = new char[ret_size];
			err |= clGetDeviceInfo(device_ids[i], CL_DEVICE_BUILT_IN_KERNELS, ret_size, built_in_kernels, &ret_size);
		}

		printf("->[Device %d]\n->Type:", i);
		if (type & CL_DEVICE_TYPE_CPU) { printf(" CPU"); }
		if (type & CL_DEVICE_TYPE_GPU) { printf(" GPU"); }
		if (type & CL_DEVICE_TYPE_ACCELERATOR) { printf(" ACCELERATOR"); } printf("\n");
		printf("->Name: %s\n", name);
		printf("->Vendor: %s\n", vendor);
		printf("->Device is %s\n", available == CL_TRUE ? "available" : "unavailable");
		printf("->Compute unit(s): %d\n", units);
		printf("->Memory address bits: %d\n", address_bits);
		printf("->Maximum memory size: %llu(byte)\n", max_mem);
		printf("->Device & Host %s have unified memory\n", uni_mem == CL_TRUE ? "" : "don't");
		printf("->Driver version: %s\n", driver_version);
		printf("->Device version: %s\n", device_version);
		printf("->Device extensions: %s\n", extensions);
		printf("->Built-in kernels: %s\n", built_in_kernels);

		if (name != nullptr) { delete[] name; }
		if (vendor != nullptr) { delete[] vendor; }
		if (driver_version != nullptr) { delete[] driver_version; }
		if (device_version != nullptr) { delete[] device_version; }
		if (extensions != nullptr) { delete[] extensions; }
		if (built_in_kernels != nullptr) { delete[] built_in_kernels; }
	}
}
