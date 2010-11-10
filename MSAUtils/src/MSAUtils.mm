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

#include "MSAUtils.h"

#ifdef MSA_TARGET_OSX
#import <Cocoa/Cocoa.h>
#endif

namespace MSA {
	
	void msaClear() {
		glClearColor(ofBgColorPtr()[0], ofBgColorPtr()[1], ofBgColorPtr()[2], ofBgColorPtr()[3]);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	void drawFPS() {
		drawFPS(10, ofGetHeight() - 10);
	}
	
	void drawFPS(int x, int y) {
		ofDisableAlphaBlending();
		ofFill();
		glColor4f(0, 0, 0, 1);
		ofRect(x - 10, y - 20, x + 100, y + 10);
		glColor4f(0.9, 0.9, 0.9, 1);
		ofDrawBitmapString("FPS: "+ofToString(ofGetFrameRate(), 2), x, y);
	}
	
	void drawFPS(int color) {
		ofSetRectMode(OF_RECTMODE_CORNER);
		ofFill();
		ofSetHexColor(color);
		string fpsStr = "FPS: "+ofToString(ofGetFrameRate(), 2);
		ofDrawBitmapString(fpsStr, 20, ofGetHeight()-20);
	}
	
	
	
	void drawFPSBar(int fpsMult) {
		static float lastTime = 0;
		float nowTime = ofGetElapsedTimef();
		float fps = 1.0f/(nowTime - lastTime);
		lastTime = nowTime;
		
		// draw a green bar width of fps
		ofEnableAlphaBlending();
		glColor4f(0, 1, 0, 0.5);
		ofRect(0, 0, fps * fpsMult, 20);
		glColor4f(0, 0, 0, 1);
		// draw a line every 10 pixels
		for(int x=(fpsMult * 10); x<ofGetWidth(); x+=(fpsMult * 10)) {
			ofLine(x, 0, x, x % (fpsMult * 30) == 0 ? 30 : 20);		// draw a taller line every 30fps
		}	
	}
	
	void dumpFPS(float seconds) {
		static float lastTimeDumped = 0;
		float nowTime = ofGetElapsedTimef();
		if(nowTime - lastTimeDumped > seconds) {
			printf("FPS: %f\n", ofGetFrameRate());
			lastTimeDumped = nowTime;
		}
		
	}
	
	
	void hideCursor() {
		[NSCursor hide];
	}
	void showCursor() {
		[NSCursor unhide];
	}

	
	void setMouseCursor(bool forceOn) {
		if(forceOn || ofGetWindowMode() == OF_WINDOW) ofShowCursor();
		else ofHideCursor();
	}
	
	
	
	static string savedDataPath;
	
	void setDataPathToBundle() {
//		savedDataPath = ofToDataPath(".", false);
//#ifdef MSA_TARGET_OSX
//		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
//		string newDataPath = string([[[NSBundle mainBundle] resourcePath] UTF8String]) + "/";
//		printf("newDataPath : %s\n", newDataPath.c_str());
//		ofSetDataPathRoot( newDataPath );
//		
//		printf("testing : %s\n", ofToDataPath("myfile.bmp").c_str());
//		
//		[pool release];
//#endif						   
//		ofDisableDataPath();
		
	}
	
	void restoreDataPath() {
//		ofEnableDataPath();
//		if(savedDataPath.empty()) ofSetDataPathRoot(savedDataPath);
	}

	string padWithZero(float num, float precision) {
		string str = ofToString(num, precision);
		if(num<10) str = "0" + str;
		return str;
	}

	
	string secondsToString(float secs) {
		int mins = floor(secs / 60);
		int hours = floor(mins/24);

		secs -= mins * 60;
		mins %= 60;
		hours %= 24;
		
		return padWithZero(hours) + ":" + padWithZero(mins) + ":" + padWithZero(secs, 2);
	}

	
	// pass a string e.g. "output/images/capture/
	void createDir(string fullpath) {
#ifdef MSA_TARGET_OSX
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
		
		NSString *nsPath = [NSString stringWithUTF8String:fullpath.c_str()];		// convert stl-string to NSString
		
		if(fullpath[0] != '/') {		// if path is relative, prefix with data path
			NSString *dataPath = [NSString stringWithFormat:@"%@/data", [[[NSBundle mainBundle] bundlePath] stringByDeletingLastPathComponent]];
			nsPath = [dataPath stringByAppendingPathComponent:nsPath];
		}
		
		[[NSFileManager defaultManager] createDirectoryAtPath:nsPath withIntermediateDirectories:YES attributes:nil error:nil];
		//	NSArray *folders = [[NSString stringWithUTF8String:fullpath.c_str()] pathComponents];	// convert stl::String into NSString and explode into components
		////	NSLog(@"folders to create: %@", folders);
		//	
		//	NSString *startPath;
		//	
		//	if(fullpath[0] == '/') {
		//		startPath = @"/";
		//	} else {
		//		startPath = [NSString stringWithFormat:@"%@/data", [[[NSBundle mainBundle] bundlePath] stringByDeletingLastPathComponent]];
		//	}
		//	
		//	for(NSString *path in folders) {
		//		startPath = [startPath stringByAppendingPathComponent:path];
		//		ofLog(OF_LOG_VERBOSE, string("Creating folder: ") + [startPath UTF8String]);
		//		[[NSFileManager defaultManager] createDirectoryAtPath:startPath attributes:nil];
		//	}
		//	
		////	success = [[NSFileManager defaultManager] createDirectoryAtPath:[NSString stringWithUTF8String:path.c_str()] attributes:nil];
		
		
		[pool release];
#endif		
	}
	
	
	int showDialog(string message, string info, int style) {
		int ret;
#ifdef MSA_TARGET_OSX

		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
		NSAlert *alert = [[NSAlert alloc] init];
//		[alert addButtonWithTitle:@"OK"];
//		[alert addButtonWithTitle:@"Cancel"];
		[alert setMessageText:[NSString stringWithUTF8String:message.c_str()]];
		[alert setInformativeText:[NSString stringWithUTF8String:info.c_str()]];
		[alert setAlertStyle:style];
		
		ret = [alert runModal];
		ret -= NSAlertFirstButtonReturn;
		[alert release];
		[pool release];
#endif		
		return ret;
	}
	
	
	
}
