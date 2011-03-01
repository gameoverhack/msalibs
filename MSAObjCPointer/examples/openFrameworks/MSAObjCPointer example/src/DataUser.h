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


/********** Example data user *************/
class DataUser {
public:
	MyObject *myData;
	int	index;
	
	DataUser() {
		printf("User::User()\n");
		myData = NULL;
	}
	
	~DataUser() {
		printf("User::~User()\n");
		releaseData();
	}
	
	
	// point myData pointer to the passed data, and retain it
	void setData(MyObject *d) {
		printf("User::setData(%p)\n", d);		
		myData = d;
		myData->retain();
	}
	
	
	// just some arbitrary function to do something with the data
	void displayData() {
		if(myData) {
			glColor4f(0, 0, 0, 0);
			ofRect(myData->a, myData->b, myData->c, myData->d);
			glColor4f(1, 0, 0, 1);
			ofDrawBitmapString( " Data for " + ofToString(index) + ": \n   " + ofToString(myData->a, 2) + "\n   " + ofToString(myData->b, 2) + "\n   " + ofToString(myData->c, 2) + "\n   "  + ofToString(myData->d, 2) + "\n   ", myData->a, myData->b + 30);
		}
	}
	
	
	// release the data and set pointer to NULL - GOOD
	void releaseData() {
		printf("User::releaseData() - GOOD\n");
		if(myData) myData->release();
		myData = NULL;
	}
	
	
	// delete the data and set pointer to NULL - BAD
	void deleteData() {
		printf("User::deleteData() - BAD\n");
		if(myData) delete myData;
		myData = NULL;
	}
	
};
