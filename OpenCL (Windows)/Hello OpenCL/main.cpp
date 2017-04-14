#include <iostream>

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include "CL/cl.h"

#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

int main()
{
	cl_device_id		device_id = nullptr;
	cl_context			context = nullptr;
	cl_command_queue	c_queue = nullptr;
	cl_mem				memobj = nullptr;
	cl_program			program = nullptr;
	cl_kernel			kernel = nullptr;
	cl_platform_id		platform_id = nullptr;
	cl_uint				ret_num_devices;
	cl_uint				ret_num_platforms;
	cl_int				ret;

	char str[MEM_SIZE];
	FILE * fp;
	char fileName[] = R"(C:\temp\hello.cl)";
	char * source_str;
	size_t source_size;

	auto error = fopen_s(&fp, fileName, "r");
	if (!fp)
	{
		fprintf(stderr, "Failed to load kernel.\n");
		return 1;
	}

	source_str = static_cast<char *>(malloc(MAX_SOURCE_SIZE));
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

	context = clCreateContext(nullptr, 1, &device_id, nullptr, nullptr, &ret);

	c_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(str), nullptr, &ret);

	program = clCreateProgramWithSource(context, 1, const_cast<const char **>(&source_str),
		static_cast<const size_t *>(&source_size), &ret);

	ret = clBuildProgram(program, 1, &device_id, nullptr, nullptr, nullptr);

	kernel = clCreateKernel(program, "hello", &ret);

	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), reinterpret_cast<void *>(&memobj));

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