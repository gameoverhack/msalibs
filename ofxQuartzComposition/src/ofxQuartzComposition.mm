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
 - ofxCocoa
 ***************/ 


#include "ofxQuartzComposition.h"


namespace MSA {
	
	ofxQuartzComposition::ofxQuartzComposition():
	_qcComposition(nil),
	_qcRenderer(nil)
	{
	}
	
	ofxQuartzComposition::~ofxQuartzComposition() {
		[_qcComposition release];
		[_qcRenderer release];
	}
	
	
	void ofxQuartzComposition::load(string path) {
		[_qcComposition release];
		_qcComposition = [[QCComposition compositionWithFile:ofxCocoa::toNSString(ofToDataPath(path))] retain];

		
		[_qcRenderer release];
		CGLContextObj cglContext			= (CGLContextObj)[[ofxCocoa::glView() openGLContext] CGLContextObj];
		CGLPixelFormatObj cglPixelFormat	= (CGLPixelFormatObj)[[ofxCocoa::glView() pixelFormat] CGLPixelFormatObj];
		CGColorSpaceRef colorSpace			= CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
		_qcRenderer = [[QCRenderer alloc] initWithCGLContext:cglContext pixelFormat:cglPixelFormat colorSpace:colorSpace composition:_qcComposition];
		CGColorSpaceRelease(colorSpace);
	}
	
//	void ofxQuartzComposition::createOffscreenRenderer(float w, float h) {
//		[_qcRenderer release];
//		CGColorSpaceRef colorSpace			= CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
//		_qcRenderer = [[QCRenderer alloc] initOffScreenWithSize:NSMakeSize(w, h) colorSpace:colorSpace composition:_qcComposition];
//		CGColorSpaceRelease(colorSpace);
//	}
	
	
	void ofxQuartzComposition::draw() {
		drawAtTime(ofGetElapsedTimef());
	}
	
	void ofxQuartzComposition::drawAtTime(double t) {
		if(_qcRenderer) {
			
			// TODO: map this to coordinates within the QC Comp
			NSPoint mousePos;
			mousePos.x = ofGetAppPtr()->mouseX / ofGetWidth();
			mousePos.y = 1.0f - ofGetAppPtr()->mouseY / ofGetHeight();
			
			NSDictionary  *arguments = [NSDictionary dictionaryWithObject:[NSValue valueWithPoint:mousePos] forKey:QCRendererMouseLocationKey];
			
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			
			if(![_qcRenderer renderAtTime:t arguments:arguments]) NSLog(@"Rendering failed at time %.3fs", time);
			
			glPopMatrix();
			
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}
	}
	
	bool ofxQuartzComposition::setValueForKey(id value, string key) {
		return [_qcRenderer setValue:value forInputKey:ofxCocoa::toNSString(key)];
	}
	
	id ofxQuartzComposition::getValueForInputKey(string key) {
		return [_qcRenderer valueForInputKey:ofxCocoa::toNSString(key)];
	}

	id ofxQuartzComposition::getValueForOutputKey(string key) {
		return [_qcRenderer valueForOutputKey:ofxCocoa::toNSString(key)];
	}

	
	
	
	NSArray *ofxQuartzComposition::inputKeys() {
		return [_qcRenderer inputKeys];
	}
	
	NSArray *ofxQuartzComposition::outputKeys() {
		return [_qcRenderer outputKeys];
	}

}

