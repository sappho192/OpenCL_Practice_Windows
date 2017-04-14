# OpenCL Practice at Windows  
  This project is currently based on Intel OpenCL SDK.  
## Testing environment  

### Environment 1  

* OS: Windows 10  
* CPU: Intel Core i7-4650U
* GPU: Intel HD Graphics 5000
* SDK: Intel OpenCL SDK 6.3
* IDE: Visual Studio 2017 Community

## How to set environment  
Install Intel OpenCL SDK from https://software.intel.com/en-us/intel-opencl  
Note that environment variable of root path of the SDK is $(INTELOCLSDKROOT) in Visual Studio, %INTELOCLSDKROOT% in Windows command line.  
### How to make new OpenCL Project  
* Make new C++ Console Project in Visual Studio
* In project properties,  
  * add "$(INTELOCLSDKROOT)include" into C/C++->General->Additional include directory    
  * add "$(INTELOCLSDKROOT)lib" into Linker->General->Additional library directory  
  * add "OpenCL.lib" into Linker->Input->Additional Dependencies  

## Project list  

### Hello OpenCL  
Making "hello" string in OpenCL kernel program.  

