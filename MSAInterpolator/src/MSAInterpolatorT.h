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

/**************************** Interpolator Template Class ****************************/
#pragma once


#include "MSACore.h"
#include "MSAInterpolationTypes.h"

namespace MSA {
	
#define kLengthCalcSegments		100
	
	template <typename Type>
	class Interpolator {
	public:
		
		bool verbose;
		
		Interpolator() {
			setInterpolation(kInterpolationCubic);
			setUseLength(false);
			verbose = false;
		}
		
		
		// use catmull rom interpolation to re-sample At normT position along the spline
		// where normT: 0....1 based on length of spline
		Type sampleAt(float t) {
			int numItems = size();
			if(numItems == 0) {
//				if(verbose) printf("Interpolator: not enough samples", t);
				return Type();
			}
			
			if(t>1) t = 1;
			else if(t<0) t=0;
			int i0, i1, i2, i3;
			float mu;
			
			findPosition(t, i1, mu);
			
			// if less than 4 data points, force linear interpolation
			InterpolationType it = _interpolationMethod;
			if(numItems<4) it = kInterpolationLinear;
			
			switch(it) {
				case kInterpolationCubic:
					i0 = i1 - 1;
					i2 = i1 + 1;
					i3 = i2 + 1;
					
					if(i0 < 0) i0 = 0;
					if(i3 >= numItems) i3 = numItems-1;
					
					return cubicInterpolate(at(i0), at(i1), at(i2), at(i3), mu);
					break;
					
				case kInterpolationLinear:
					i2 = i1 + 1;
					if(i2 >= numItems) i2 = numItems-1;
					return linearInterpolate(at(i1), at(i2), mu);
					break;
			}
		}
		
		void setInterpolation(InterpolationType i) {
			_interpolationMethod = i;
			updateAllLengths();
		}
		
		int getInterpolation() {
			return _interpolationMethod;
		}
		
		void setUseLength(bool b) {
			_useLength = b;
			updateAllLengths();
		}
		
		bool getUseLength() {
			return _useLength;
		}
		
		
		/******************* stl::vector wrapper functions *******************/
		void push_back(const Type& newData) {
			_data.push_back(newData);						// add data
			
			if(getUseLength()) {
				float segmentLength;
				float totalLength;
				
				if(size() > 1) {
					//				Type distT		= newData - _data.at(prevIndex);	// get offset to previous node
					//				float dist		= lengthOf(distT);					// actual Length to node
					
					segmentLength	= calcSegmentLength(size()-1);
					totalLength		= segmentLength + _dist.at(size()-2);
				} else {
					segmentLength	= 0;
					totalLength		= 0;
				}

				_dist.push_back(totalLength);
				
//				if(verbose) printf("segment length = %f | total length = %f\n", segmentLength, totalLength);
			}
		}
		
		int size() {
			return _data.size();
		}
		
		void reserve(int i) {
			_data.reserve(i);
			_dist.reserve(i);
		}
		
		void clear() {
			_data.clear();
			_dist.clear();		
		}
		
		const Type& at(int i) {
			return _data.at(constrain(i, 0, size()-1));
		}
		
		vector<Type> getData() {
			return _data;
		}
		
		
		
	protected:
		InterpolationType _interpolationMethod;
		bool _useLength;
		vector<Type> _data;				// vector of all data
		vector<float> _dist;			// vector of cumulative Lengths from i'th data point to beginning of spline
		
		// calculates length of segment prior to (leading up to) i'th point
		float calcSegmentLength(int i) {
			int numItems		= size();
			
			if(numItems < 2 || i < 1 || i >= numItems) return 0;
			
			bool saveUseLength	= _useLength;
			_useLength = false;

			float startPerc = (i-1) * 1.0f/(numItems-1);
			float endPerc	= (i) * 1.0f/(numItems-1);
			float incPerc	= (endPerc - startPerc)/kLengthCalcSegments;
			
			Type prev = sampleAt(startPerc);
			Type cur;
			
			float segmentLength = 0;
			for(float f = startPerc; f <= endPerc; f+= incPerc) {
				cur = sampleAt(f);
				segmentLength += lengthOf(cur - prev);
				prev = cur;
			}
			
			_useLength = saveUseLength;
			
			if(verbose) printf("segment length for %i is %f\n", i, segmentLength);

			
			return segmentLength;
		}
		
		// update all Lengths in _dist array
		void updateAllLengths() {
			_dist.clear();
			
			float curTotal = 0;
			
			for(int i=0; i<size(); i++) {
				curTotal += calcSegmentLength(i);
				_dist.push_back(curTotal);
			}
			
		}
		
		// given t(0...1) find the node index directly to the left of the point
		void findPosition(float t, int &leftIndex, float &mu) {
			int numItems = size();
			
			switch(numItems) {
				case 0: 
					leftIndex = 0;
					mu = 0;
					break;
					
				case 1:
					leftIndex = 0;
					mu = 0;
					break;
					
				case 2:
					leftIndex = 0;
					mu = t;
					break;
					
				default:
					if(_useLength) {									// need to use 
						float totalLengthOfInterpolator = _dist.at(numItems-1);
						float tDist = totalLengthOfInterpolator * t;			// the Length we want to be from the start
						int startIndex = floor(t * (numItems - 1));			// start approximation here
						int i1 = startIndex;
						int limitLeft = 0;
						int limitRight = numItems-1;
						
						float distAt1, distAt2;
						//						do {
						for(int iterations = 0; iterations < 100; iterations ++) {	// limit iterations
							distAt1 = _dist.at(i1);
							if(distAt1 <= tDist) {							// if Length at i1 is less than desired Length (this is good)
								distAt2 = _dist.at(constrain(i1+1, 0, (int)_dist.size()-1));
								if(distAt2 > tDist) {
									leftIndex = i1;
									mu = (tDist - distAt1) / (distAt2-distAt1);
									return;
								} else {
									limitLeft = i1;
								}
							} else {
								limitRight = i1;
							}
							i1 = (limitLeft + limitRight)>>1;
						}
						//						} while(true);
						
					} else {
						float actT = t * (numItems - 1);
						leftIndex = floor(actT);
						mu = actT - leftIndex;
					}
			}
		}
		
		
		Type linearInterpolate(const Type& y1, const Type& y2, float mu) {
			return (y2-y1) * mu + y1;
		}
		
		
		// this function is from Paul Bourke's site
		// http://local.wasp.uwa.edu.au/~pbourke/miscellaneous/interpolation/
		Type cubicInterpolate(const Type& y0, const Type& y1, const Type& y2, const Type& y3, float mu) {
			float mu2 = mu * mu;
			Type a0 = y3 - y2 - y0 + y1;
			Type a1 = y0 - y1 - a0;
			Type a2 = y2 - y0;
			Type a3 = y1;
			
			return(a0 * mu * mu2 + a1 * mu2 + a2 * mu + a3);
		}
	};
	
}