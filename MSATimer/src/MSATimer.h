/***********************************************************************
 
 Copyright (c) 2008, 2009, 2010 Memo Akten, www.memo.tv
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

/***************
 DEPENDENCIES:
 - MSACore
 ***************/ 

#pragma once

#include "MSACore.h"

#ifdef MSA_TARGET_OSX
#include <mach/mach_time.h>
#endif

namespace MSA {
	
	class Timer {
	public:
		Timer();
		
		inline double getTotalSeconds();			// elapsed seconds since beginning of time
		inline double getAppSeconds();				// elapsed seconds since start of app
		inline double getSecondsSinceLastCall();	// elapsed seconds since last time this function was called
		
		inline void start();	
		inline void stop();
		inline double getSeconds();			// elapsed seconds since you called start()
		
	protected:
		double appStartTime;
		double lastCallTime;
		double startTime;
		double stopTime;
		bool isRunning;

#ifdef MSA_TARGET_OSX
		double machMultiplier;
#endif
	};

	
	/******************** IMPLEMENTATION ************************/

	inline Timer::Timer() {
#ifdef MSA_TARGET_OSX
		mach_timebase_info_data_t info;
		mach_timebase_info(&info); 
		machMultiplier = info.numer / 1000000000.0 / info.denom;
		appStartTime = mach_absolute_time() * machMultiplier;
#else
		appStartTime = MSA::getElapsedSeconds();
#endif		
		
		startTime = lastCallTime = appStartTime;
		
		isRunning = false;
	}
	
	
	inline double Timer::getTotalSeconds() {
#ifdef MSA_TARGET_OSX
		return mach_absolute_time() * machMultiplier;
#else
		return ofGetElapsedTimef();
#endif		
	}
	
	inline double Timer::getAppSeconds() {
		return getTotalSeconds() - appStartTime;
	}
	
	inline double Timer::getSecondsSinceLastCall() {
		float nowTime = getTotalSeconds();
		float diff = nowTime - lastCallTime;
		lastCallTime = nowTime;
		return diff;
	}
	
	inline void Timer::start() {
		isRunning = true;
		startTime = getTotalSeconds();
	}
	
	inline void Timer::stop() {
		stopTime = getTotalSeconds();
		isRunning = false;
	}
	
	inline double Timer::getSeconds() {
		if(isRunning) {
			return getTotalSeconds() - startTime;
		} else {
			return stopTime - startTime;
		}
	}
	
}