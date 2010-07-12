/***********************************************************************
 
 OpenCL Kernel
 Do not instanciate this directly.
 Instead use OpenCL::loadKernel or OpenCLProgram::loadKernel
 
 /***********************************************************************
 Copyright (c) 2008, 2009, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * ***********************************************************************/ 


#pragma once

#include <OpenCL/Opencl.h>

#include "MSACore.h"
#include "MSAOpenCLMemoryObject.h"


namespace MSA { 
	class OpenCL;
	
	class OpenCLKernel {
		friend class OpenCLProgram;
		
	public:
		
		~OpenCLKernel();
		
		// assign buffer to arguments
		//	void setArg(int argNumber, cl_mem clMem);
		//	void setArg(int argNumber, float f);
		//	void setArg(int argNumber, int i);
		
		template<class T>
		void setArg(int argNumber, T &arg){
			//		ofLog(OF_LOG_VERBOSE, "OpenCLKernel::setArg " + name + ": " + ofToString(argNumber));	
			
			assert(clKernel);
			
			cl_int err  = clSetKernelArg(clKernel, argNumber, sizeof(T), &arg);
			assert(err != CL_INVALID_KERNEL);
			assert(err != CL_INVALID_ARG_INDEX);
			assert(err != CL_INVALID_ARG_VALUE);
			assert(err != CL_INVALID_MEM_OBJECT);
			assert(err != CL_INVALID_SAMPLER);
			assert(err != CL_INVALID_ARG_SIZE);
			assert(err == CL_SUCCESS);
		}
		
		// run the kernel
		// globalSize and localSize should be int arrays with same number of dimensions as numDimensions
		// leave localSize blank to let OpenCL determine optimum
		void	run(int numDimensions, size_t *globalSize, size_t *localSize = NULL);
		
		// some wrappers for above to create the size arrays on the run
		void	run1D(size_t globalSize, size_t localSize = 0);
		void	run2D(size_t globalSizeX, size_t globalSizeY, size_t localSizeX = 0, size_t localSizeY = 0);
		void	run3D(size_t globalSizeX, size_t globalSizeY, size_t globalSizeZ, size_t localSizeX = 0, size_t localSizeY = 0, size_t localSizeZ = 0);
		
		cl_kernel& getCLKernel();
		string getName();
		
	protected:
		string			name;
		OpenCL*		pOpenCL;
		cl_kernel		clKernel;
		
		OpenCLKernel(OpenCL *pOpenCL, cl_kernel clKernel, string name);
	};
}