/***********************************************************************
 
 OpenCL Program
 Do not instanciate this directly.
 Instead use OpenCL::loadProgram
 
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
#include "MSAOpenCLProgram.h"
#include "MSAOpenCLKernel.h"

namespace MSA { 
	
	char *OpenCL_textFileRead(char *fn);
	
	
	OpenCLProgram::OpenCLProgram() {
		ofLog(OF_LOG_VERBOSE, "OpenCLProgram::OpenCLProgram");
		this->pOpenCL = pOpenCL;
		pOpenCL = NULL;
		clProgram = NULL;
	}
	
	
	OpenCLProgram::~OpenCLProgram() {
		ofLog(OF_LOG_VERBOSE, "OpenCLProgram::~OpenCLProgram");
		//	clReleaseProgram(clProgram);		// this crashes it for some reason
	}
	
	
	void OpenCLProgram::loadFromFile(std::string filename, bool isBinary) { 
		ofLog(OF_LOG_VERBOSE, "OpenCLProgram::loadFromFile " + filename + ", isBinary: " + ofToString(isBinary));
		
		string fullPath = ofToDataPath(filename.c_str());
		
		if(isBinary) {
			//		clCreateProgramWithBinary
			ofLog(OF_LOG_ERROR, "Binary programs not implemented yet\n");
			assert(false);
			
		} else {
			
			char *source = OpenCL_textFileRead((char*)fullPath.c_str());
			if(source == NULL) {
				ofLog(OF_LOG_ERROR, "Error loading program file: " + fullPath);
				assert(false); 
			}
			
			loadFromSource(source);
			
			free(source);
		}
	}
	
	
	
	void OpenCLProgram::loadFromSource(std::string source) {
		ofLog(OF_LOG_VERBOSE, "OpenCLProgram::loadFromSource ");// + source);
		
		cl_int err;
		
		pOpenCL = OpenCL::currentOpenCL;
		
		clProgram = clCreateProgramWithSource(pOpenCL->getContext(), 1, (const char**)&source, NULL, &err);
		
		build();
	} 
	
	
	OpenCLKernel* OpenCLProgram::loadKernel(string kernelName) {
		ofLog(OF_LOG_VERBOSE, "OpenCLProgram::loadKernel " + kernelName);
		assert(clProgram);
		
		cl_int err;
		
		OpenCLKernel *k = new OpenCLKernel(pOpenCL, clCreateKernel(clProgram, kernelName.c_str(), &err), kernelName);
		
		if(err != CL_SUCCESS) {
			ofLog(OF_LOG_ERROR, string("Error creating kernel: ") + kernelName);
			assert(false);
		}
		
		return k;
	}
	
	
	void OpenCLProgram::getBinary()
	{
		cl_uint program_num_devices;
		cl_int err;
		err = clGetProgramInfo(clProgram, CL_PROGRAM_NUM_DEVICES, sizeof(cl_uint), &program_num_devices, NULL);
		assert(err == CL_SUCCESS);
		
		if (program_num_devices == 0) {
			std::cerr << "no valid binary was found" << std::endl;
			return;
		}
		
		size_t binaries_sizes[program_num_devices];
		
		err = clGetProgramInfo(clProgram, CL_PROGRAM_BINARY_SIZES, program_num_devices*sizeof(size_t), binaries_sizes, NULL);
		assert(err = CL_SUCCESS);
		
		char **binaries = new char*[program_num_devices];
		
		for (size_t i = 0; i < program_num_devices; i++)
			binaries[i] = new char[binaries_sizes[i]+1];
		
		err = clGetProgramInfo(clProgram, CL_PROGRAM_BINARIES, program_num_devices*sizeof(size_t), binaries, NULL);
		assert(err = CL_SUCCESS);
		
		for (size_t i = 0; i < program_num_devices; i++) {
			binaries[i][binaries_sizes[i]] = '\0';
			std::cout << "Program " << i << ":" << std::endl;
			std::cout << binaries[i];
		}
		
		for (size_t i = 0; i < program_num_devices; i++)
			delete [] binaries[i];
		
		delete [] binaries;
	}
	
	
	void OpenCLProgram::build() {
		if(clProgram == NULL) {
			ofLog(OF_LOG_ERROR, "Error creating program object.");
			assert(false); 
		}	
		
		cl_int err = clBuildProgram(clProgram, 0, NULL, NULL, NULL, NULL);
		if(err != CL_SUCCESS) {
			size_t len;
			char buffer[2048];
			
			ofLog(OF_LOG_ERROR, "\n\n ***** Error building program. ***** \n ***********************************\n\n");
			clGetProgramBuildInfo(clProgram, pOpenCL->getDevice(), CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
			ofLog(OF_LOG_ERROR, buffer);
			assert(false);
		}	
	}
	
	cl_program& OpenCLProgram::getCLProgram(){
		return clProgram;	
	}
	
	//---------------------------------------------------------
	// below is from: www.lighthouse3d.com
	// you may use these functions freely. they are provided as is, and no warranties, either implicit, or explicit are given
	//---------------------------------------------------------
	
	char *OpenCL_textFileRead(char *fn) {
		
		FILE *fp;
		char *content 	= 	NULL;
		int count		=	0;
		
		if (fn != NULL) {
			fp = fopen(fn,"rt");
			if (fp != NULL) {
				
				fseek(fp, 0, SEEK_END);
				count = ftell(fp);
				rewind(fp);
				
				if (count > 0) {
					content = (char *)malloc(sizeof(char) * (count+1));
					count = fread(content,sizeof(char),count,fp);
					content[count] = '\0';
				}
				fclose(fp);
			}
		}
		
		return content;
	}
}
