/***********************************************************************
 
 Super simple Objective C style Smart Pointer which keeps reference count 
 and frees memory when reference count is zero.
 
 Any object you want this functionality, should extend ofxObjCPointer
 
 
 If someone passes you a pointer and you only need the data temporarily
 (you do NOT need the data beyond the scope of the function)
 just use the pointer/data and do NOT call any of the special methods (retain or release).
 
 If you DO need to keep the data beyond the scope of the function,
 call myObjPointer->retain() on the pointer to indicate that you need the data, 
 and call myObjPointer->release() when you are done with it.
 
 If you allocated the pointer with 'new MyObject' or retained it with myObj->retain()
 then you must ALWAYS release it 'myObj->release()' when you are done.
 You do not need to worry about whether anyone else is using the data or not, 
 the reference count will take care of that.
 
 Never use 'delete myObj', only use 'myObj->release();
 
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

/***************
 DEPENDENCIES:
 - MSACore
 ***************/ 

#pragma once

#include "MSACore.h"

namespace MSA {
	
	class ObjCPointer {
	public:
		bool verbose;
		
		ObjCPointer();
		virtual ~ObjCPointer();
		
		// use this to indicate you are using the object and want to keep it safe in memory
		void retain();
		
		// use this to indicate you are done with the object and as far as you are concerned, it can be deleted
		void release();	
		
		void setInstanceName(string n);
		void setInstanceName(const char* sz); 
		
		string getClassName();
		string getInstanceName();
		
	protected:	
		void setClassName(string n);
		void setClassName(const char* sz); 
		
	private:
		unsigned int	__useCount;
		string			__myClassName;
		string			__myInstanceName;
	};
	
}

