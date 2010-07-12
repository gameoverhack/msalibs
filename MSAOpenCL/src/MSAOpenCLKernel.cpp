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


#include "MSAOpenCL.h"
#include "MSAOpenCLKernel.h"

namespace MSA { 
	
	OpenCLKernel::OpenCLKernel(OpenCL* pOpenCL, cl_kernel clKernel, string name) {
		ofLog(OF_LOG_VERBOSE, "OpenCLKernel::OpenCLKernel " + ofToString((int)pOpenCL) + ", " + name);
		this->pOpenCL	= pOpenCL;
		this->name		= name;
		this->clKernel	= clKernel;
	}
	
	
	OpenCLKernel::~OpenCLKernel() {
		ofLog(OF_LOG_VERBOSE, "OpenCLKernel::~OpenCLKernel " + name);
		clReleaseKernel(clKernel);
	}
	
	/*
	 void OpenCLKernel::setArg(int argNumber, cl_mem clMem) {
	 ofLog(OF_LOG_VERBOSE, "OpenCLKernel::setArg " + name + ": " + ofToString(argNumber));	
	 
	 assert(clKernel);
	 
	 cl_int err  = clSetKernelArg(clKernel, argNumber, sizeof(cl_mem), &clMem);
	 assert(err == CL_SUCCESS);
	 }*/
	
	void OpenCLKernel::run(int numDimensions, size_t *globalSize, size_t *localSize) {
		assert(clKernel);
		
		cl_int err;
		
		//	size_t localSize = MIN(n, info.maxWorkGroupSize);
		
		err = clEnqueueNDRangeKernel(pOpenCL->getQueue(), clKernel, numDimensions, NULL, globalSize, localSize, 0, NULL, NULL);
		assert(err == CL_SUCCESS);
	}
	
	void OpenCLKernel::run1D(size_t globalSize, size_t localSize) {
		size_t globalSizes[1];
		globalSizes[0] = globalSize;
		if(localSize) {
			size_t localSizes[1];
			localSizes[0] = localSize;
			run(1, globalSizes, localSizes);
		} else {
			run(1, globalSizes, NULL);
		}
	}
	
	void OpenCLKernel::run2D(size_t globalSizeX, size_t globalSizeY, size_t localSizeX, size_t localSizeY) {
		size_t globalSizes[2];
		globalSizes[0] = globalSizeX;
		globalSizes[1] = globalSizeY;
		if(localSizeY && localSizeX) {
			size_t localSizes[2];
			localSizes[0] = localSizeX;
			localSizes[1] = localSizeY;
			run(2, globalSizes, localSizes);
		} else {
			run(2, globalSizes, NULL);
		}
	}
	
	void OpenCLKernel::run3D(size_t globalSizeX, size_t globalSizeY, size_t globalSizeZ, size_t localSizeX, size_t localSizeY, size_t localSizeZ) {
		size_t globalSizes[3];
		globalSizes[0] = globalSizeX;
		globalSizes[1] = globalSizeY;
		globalSizes[2] = globalSizeZ;
		if(localSizeZ && localSizeY && localSizeX) {
			size_t localSizes[3];
			localSizes[0] = localSizeX;
			localSizes[1] = localSizeY;
			localSizes[2] = localSizeZ;
			run(3, globalSizes, localSizes);
		} else {
			run(3, globalSizes, NULL);
		}
	}
	
	
	cl_kernel& OpenCLKernel::getCLKernel() {
		return clKernel;
	}
	
	string OpenCLKernel::getName() {
		return name;
	}
}