/***********************************************************************
 
 Copyright (c) 2008, 2009, 2010, Memo Akten, www.memo.tv
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


/**************************** 3D InterpolatorT (of Vec3) ****************************/

#pragma once

#include "MSAInterpolatorT.h"

namespace MSA {
	
	typedef InterpolatorT<Vec3f> Interpolator3D;
	
	
	inline float lengthOf(const Vec3f& v) {
		return v.length();
	}
	
	
	// OpenGL ES compatibility added by Rob Seward
	// http://www.openframeworks.cc/forum/viewtopic.php?f=25&t=3767&p=19865
	
	
	inline void drawInterpolatorRaw(Interpolator3D spline, int dotSize = 20, int lineWidth = 4){
		int numItems = spline.size();
		
		if(lineWidth) {
			glLineWidth(lineWidth);
			GLfloat vertex[numItems * 3];
			for(int i=0; i<numItems; i++) {
				vertex[i*3]		= spline.at(i).x;
				vertex[(i*3)+1] = spline.at(i).y;
				vertex[(i*3)+2] = spline.at(i).z;
			}
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, vertex);
			glDrawArrays(GL_LINE_STRIP, 0, numItems);
		}
		
		if(dotSize) {
			glPointSize(dotSize);
			GLfloat vertex[numItems * 3];
			for(int i=0; i<numItems; i++) {
				vertex[i*3]		= spline.at(i).x;
				vertex[(i*3)+1] = spline.at(i).y;
				vertex[(i*3)+2] = spline.at(i).z;
			}
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, vertex);
			glDrawArrays(GL_POINTS, 0, numItems);
		}		
	}
	
	
	inline void drawInterpolatorSmooth(Interpolator3D spline, int numSteps, int dotSize = 8, int lineWidth = 2) {
		float spacing = 1.0/numSteps;
		if(lineWidth) {
			glLineWidth(lineWidth);
			
			GLfloat vertex[numSteps * 3];
			int i=0;
			for(float f=0; f<1; f+= spacing) {
				Vec3f v			= spline.sampleAt(f);
				vertex[i*3]		= v.x;
				vertex[(i*3)+1] = v.y;
				vertex[(i*3)+2] = v.z;
				i++;
			}
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, vertex);
			glDrawArrays(GL_LINE_STRIP, 0, numSteps);
		}
		
		if(dotSize) {
			glPointSize(dotSize);
			GLfloat vertex[numSteps * 3];
			int i=0;
			for(float f=0; f<1; f+= spacing) {
				Vec3f v			= spline.sampleAt(f);
				vertex[i*3]		= v.x;
				vertex[(i*3)+1] = v.y;
				vertex[(i*3)+2] = v.z;
				i++;
			}
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, vertex);
			glDrawArrays(GL_POINTS, 0, numSteps);
		}
	}
	
}