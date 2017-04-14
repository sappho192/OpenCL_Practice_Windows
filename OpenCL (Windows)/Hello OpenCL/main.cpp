/*
 * Make "hello" string in OpenCL kernel program.  
 * Created by sappho192 at 2017.Apr.14
 */

#include <iostream>

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include "CL/cl.h"

#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

int main()
{
	cl_int ret;

	cl_device_id device_id = nullptr;
	cl_platform_id platform_id = nullptr;
	// Number of available devices in platform
	cl_uint ret_num_devices;
	// Number of available platforms
	cl_uint ret_num_platforms;
	cl_context context = nullptr;
	// Select first platform, first device
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

	context = clCreateContext(nullptr, 1, &device_id, nullptr, nullptr, &ret);

	cl_command_queue c_queue = nullptr;
	cl_mem memobj = nullptr;
	// Deprecated OpenCL 1.2 API
	c_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	char str[MEM_SIZE];
	FILE * fp;
	char fileName[] = R"(C:\temp\hello.cl)";
	char * source_str;
	size_t source_size;
	cl_program program = nullptr;
	auto error = fopen_s(&fp, fileName, "r");
	if (!fp)
	{
		fprintf(stderr, "Failed to load kernel.\n");
		return 1;
	}
	source_str = static_cast<char *>(malloc(MAX_SOURCE_SIZE));
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
	program = clCreateProgramWithSource(context, 1, const_cast<const char **>(&source_str),
		static_cast<const size_t *>(&source_size), &ret);
	ret = clBuildProgram(program, 1, &device_id, nullptr, nullptr, nullptr);

	memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(str), nullptr, &ret);

	cl_kernel kernel = nullptr;
	kernel = clCreateKernel(program, "hello", &ret);
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), reinterpret_cast<void *>(&memobj));

	// Deprecated OpenCL 1.2 API
	ret = clEnqueueTask(c_queue, kernel, 0, nullptr, nullptr);
	ret = clEnqueueReadBuffer(c_queue, memobj, CL_TRUE, 0, sizeof(str), str, 0, nullptr, nullptr);

	printf(str);

	ret = clFlush(c_queue);
	ret = clFinish(c_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(memobj);
	ret = clReleaseCommandQueue(c_queue);
	ret = clReleaseContext(context);

	free(source_str);

	return 0;
}