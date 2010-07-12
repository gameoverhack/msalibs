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

#include "testApp.h"

// try commenting out the line below to see what it'll be like without using smart pointers
// as soon as you delete one instance, the data is lost
// and all other users of the data screwup

#define USE_SMART_POINTER


//--------------------------------------------------------------
void testApp::setup(){	
	ofBackground(255, 255, 255);
	
	MyObject *tempData = new MyObject(20, 70, 100, 500);
	tempData->verbose = true;
	
	for(int i=0; i < NUM_USERS; i++) {
		user[i].setData(tempData);			// users' internal pointer points to the same data, and retains it
		user[i].index = i+1;				// just arbitrary unique data per user for display
	}
	tempData->release();			// data is released from tempData, but is still in memory because its being used by the users
}


//--------------------------------------------------------------
void testApp::draw(){
	glColor4f(0, 0, 0, 1);
	
#ifdef USE_SMART_POINTER	
	ofDrawBitmapString( " Press number keys 1-6 to destroy the objects. \n Even though they share data, the data remains until all objects have been destroyed.\n Check the console to see when the data is actually deleted.", 20, 20);
#else
	ofDrawBitmapString( " Press number keys 1-6 to destroy the objects. \n They share data, so when one is destroyed, all others go bonkers.", 20, 20);
#endif
	
	for(int i=0; i<NUM_USERS; i++) {
		user[i].displayData();
		glTranslatef(150, 0, 0);
	}
}


//--------------------------------------------------------------
void testApp::keyPressed (int key){ 
	int i = key - '0';
	if(i<1 || i>NUM_USERS) return;
	printf("Pressed key: %i\n", i);
	
#ifdef USE_SMART_POINTER
	user[i-1].releaseData();
#else 
	user[i-1].deleteData();
#endif	
}
