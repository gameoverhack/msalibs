/***********************************************************************
 
 OpenCL Buffer Memory Object
 You can either instantiate this class directly. e.g.:
 OpenCLBuffer myBuffer;
 myImage.initFromGLObject(myVbo);
 
 or create it via Instead use OpenCL::createBufferXXXX. e.g.:
 OpenCLBuffer *myBuffer = openCL.createBufferFromGLObject(myVBO);
 (this method is here for backwards compatibility with previous versions of OpenCL)
 
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
#include "MSAOpenCLBuffer.h"

namespace MSA {
	
	OpenCLBuffer::OpenCLBuffer() {
		ofLog(OF_LOG_VERBOSE, "OpenCLBuffer::OpenCLBuffer");
	}
	
	void OpenCLBuffer::initBuffer(int numberOfBytes,
								  cl_mem_flags memFlags,
								  void *dataPtr,
								  bool blockingWrite)
	{
		
		ofLog(OF_LOG_VERBOSE, "OpenCLBuffer::initBuffer");
		
		init();
		
		cl_int err;
		clMemObject = clCreateBuffer(pOpenCL->getContext(), memFlags, numberOfBytes, memFlags & CL_MEM_USE_HOST_PTR ? dataPtr : NULL, &err);
		assert(err == CL_SUCCESS);
		assert(clMemObject);
		
		if(dataPtr) write(dataPtr, 0, numberOfBytes, blockingWrite);
	}
	
	
	void OpenCLBuffer::initFromGLObject(GLuint glBufferObject,
										cl_mem_flags memFlags)
	{	
		ofLog(OF_LOG_VERBOSE, "OpenCLBuffer::initFromGLObject");
		
		init();
		
		cl_int err;
		clMemObject= clCreateFromGLBuffer(pOpenCL->getContext(), memFlags, glBufferObject, &err);
		assert(err != CL_INVALID_CONTEXT);
		assert(err != CL_INVALID_VALUE);
		assert(err != CL_INVALID_GL_OBJECT);
		assert(err != CL_OUT_OF_HOST_MEMORY);
		assert(err == CL_SUCCESS);
		assert(clMemObject);	
	}
	
	
	void OpenCLBuffer::read(void *dataPtr, int startOffsetBytes, int numberOfBytes, bool blockingRead) {
		cl_int err = clEnqueueReadBuffer(pOpenCL->getQueue(), clMemObject, blockingRead, startOffsetBytes, numberOfBytes, dataPtr, 0, NULL, NULL);
		assert(err == CL_SUCCESS);
	}
	
	
	void OpenCLBuffer::write(void *dataPtr, int startOffsetBytes, int numberOfBytes, bool blockingWrite) {
		cl_int err = clEnqueueWriteBuffer(pOpenCL->getQueue(), clMemObject, blockingWrite, startOffsetBytes, numberOfBytes, dataPtr, 0, NULL, NULL);
		assert(err == CL_SUCCESS);
	}
	
	void OpenCLBuffer::copyFrom(OpenCLBuffer &srcBuffer, int srcOffsetBytes, int dstOffsetBytes, int numberOfBytes) {
		cl_int err = clEnqueueCopyBuffer(pOpenCL->getQueue(), srcBuffer.getCLMem(), clMemObject, srcOffsetBytes, dstOffsetBytes, numberOfBytes, 0, NULL, NULL);
		assert(err == CL_SUCCESS);
	}
	
	
	void OpenCLBuffer::init() {
		memoryObjectInit();
	}
}
