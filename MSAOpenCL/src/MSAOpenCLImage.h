/***********************************************************************
 
 OpenCL Image Memory Object
 You can either instantiate this class directly. e.g.:
 OpenCLImage myImage;
 myImage.initWithGLTexture(320, 240, GL_RGBA);
 
 or create it via Instead use OpenCL::createImageXXXX. e.g.:
 OpenCLImage *myImage = openCL.createImageWithTexture(320, 240, GL_RGBA);
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


#pragma once

#include <OpenCL/Opencl.h>

#include "MSACore.h"
#include "MSAOpenCLMemoryObject.h"


namespace MSA { 
	
	class OpenCLImage : public OpenCLMemoryObject
//	, public ofBaseDraws
	{
	public:
		
		OpenCLImage();
		
		
		// create an Image with given properties
		// Image is not linked to an OpenGL texture
		// for 2D image, leave depth as 1
		// parameters with default values can be omited
		void initWithoutTexture(int width,
								int height,
								int depth = 1,
								cl_channel_order imageChannelOrder = CL_RGBA,
								cl_channel_type imageChannelDataType = CL_FLOAT,
								cl_mem_flags memFlags = CL_MEM_READ_WRITE,
								void *dataPtr = NULL,
								bool blockingWrite = CL_FALSE);
		
		
		// create a 2D Image from the ofTexture passed in (they share memory space on device)
		// parameters with default values can be omited
		void initFromTexture(ofTexture &tex,
							 cl_mem_flags memFlags = CL_MEM_READ_WRITE,
							 int mipLevel = 0);
		
		
		
		// create both a 2D Image AND an ofTexture at the same time (they share memory space on device)
		// parameters with default values can be omited
		void initWithTexture(int width,
							 int height,
							 int glTypeInternal = GL_RGBA,
							 cl_mem_flags memFlags = CL_MEM_READ_WRITE);
		
		
		
		
		// read from device memory, into main memoy (into dataPtr)
		// if origin and/or region is NULL, entire image is read
		void read(void *dataPtr,
				  bool blockingRead = CL_TRUE,
				  size_t *pOrigin = NULL,
				  size_t *pRegion = NULL,
				  size_t rowPitch = 0,
				  size_t slicePitch = 0);
		
		// write from main memory (dataPtr), into device memory
		// if origin and/or region is NULL, entire image is written
		void write(void *dataPtr,
				   bool blockingWrite = CL_FALSE,
				   size_t *pOrigin = NULL,
				   size_t *pRegion = NULL,
				   size_t rowPitch = 0,
				   size_t slicePitch = 0);
		
		
		// copy data from another image in device memory
		// if origin and/or region is NULL, entire image is written
		void copyFrom(OpenCLImage &srcImage,
					  size_t *pSrcOrigin = NULL, 
					  size_t *pDstOrigin = NULL, 
					  size_t *pRegion = NULL);
		
		
		
		// return reference to related ofTexture
		// this may be NULL if no ofTexture was setup
		ofTexture &getTexture();
		
		// draw the ofTexture
		// this may be NULL if no ofTexture was setup
		// this may not be up-to-date...
		// ...need to make sure the openCL kernels are finished with OpenCL::finish() or sync with cl_events
		void draw(float x, float y);
		void draw(float x, float y, float w, float h);
		
		
		void reset();
		
		
		float getWidth() {
			return width;
		}
		
		float getHeight() {
			return height;
		}
		
		int getDepth() {
			return depth;
		}
		
		
	protected:
		int				width;
		int				height;
		int				depth;
		
		size_t			origin[3];
		size_t			region[3];
		
		ofTexture		*texture;
		
		void init(int width, int height, int depth);
		
	};
}
