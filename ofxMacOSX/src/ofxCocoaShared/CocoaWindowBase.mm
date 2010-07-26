//
//  CocoaWindowBase.cpp
//  Cocoa Test
//
//  Created by Mehmet Akten on 16/08/2009.
//  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
//

// Shared between iPhone and Desktop 

#import "CocoaWindowBase.h"
#import "ofxAppDelegateBase.h"

#import "ofMain.h"

static CocoaWindowBase * ofWindowPtr = NULL;
CocoaWindowBase* ofxGetCocoaWindow() {
	return ofWindowPtr;
}



/******** Constructor ************/

CocoaWindowBase::CocoaWindowBase() {
	NSLog(@"CocoaWindowBase::CocoaWindowBase()");
	nFrameCount				= 0;
	bEnableSetupScreen		= true;
	
	windowPos.set(0, 0);
	windowSize.set(0, 0);
	screenSize.set(0, 0);
	
	nFrameCount				= 0;
	windowMode				= 0;
	timeNow, timeThen, fps	= 0.0f;
	
	frameRate				= 0;
	glView					= nil;
}


/******** Initialization methods ************/

void CocoaWindowBase::setupOpenGL(int w, int h, int screenMode) {
	NSLog(@"CocoaWindowBase::setupOpenGL(%i, %i, %i)", w, h, screenMode);
	
	windowMode = screenMode;
	windowSize.set(w, h);
}


void CocoaWindowBase::initializeWindow() {
	
}


void  CocoaWindowBase::runAppViaInfiniteLoop(ofBaseApp * appPtr) {
	NSLog(@"CocoaWindowBase::runAppViaInfiniteLoop()");
	
	ofWindowPtr = this;
	ofGetAppPtr()->mouseX = 0;
	ofGetAppPtr()->mouseY = 0;
	
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
#ifdef TARGET_OF_IPHONE
	UIApplicationMain(nil, nil, nil, @"iPhoneAppDelegate");		// this will run the infinite loop checking all events
#else	
	NSApplicationMain(0,  NULL);
#endif
	[pool release];	
}


void CocoaWindowBase::update(){	
	ofGetAppPtr()->update();
}

void CocoaWindowBase::render(int width, int height){

	// set viewport, clear the screen
	glViewport( 0, 0, width, height );
	if(bEnableSetupScreen) ofSetupScreen();
	
	if(ofbClearBg()){
		float * bgPtr = ofBgColorPtr();
		glClearColor(bgPtr[0],bgPtr[1],bgPtr[2], bgPtr[3]);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	ofGetAppPtr()->draw();
	
	// -------------- fps calculation:
	timeNow = ofGetElapsedTimef();
	if( ( timeNow - timeThen ) > 0 ) {
		fps = 1.0 / (timeNow-timeThen);
		frameRate *= 0.9f;
		frameRate += 0.1f*fps;
	}
	timeThen = timeNow;
	// --------------
	
	nFrameCount++;		// increase the overall frame count
	
}



void CocoaWindowBase::timerLoop() {
	update();
	
	[glView setCurrentContext];
	
	render(ofGetWidth(), ofGetHeight());
	
	[glView flush];
}



/******** Get Window/Screen properties ************/

// return cached pos, read if nessecary
ofPoint	CocoaWindowBase::getWindowPosition() {
	return windowPos;
}


int	CocoaWindowBase::getWindowMode() {
	return windowMode;
}

float CocoaWindowBase::getFrameRate() {
	return frameRate;
}

/******** Other stuff ************/
void CocoaWindowBase::setFrameRate(float targetRate) {
	[ofxGetAppDelegate() setFrameRate:targetRate];
	
}

int	CocoaWindowBase::getFrameNum() {
	return nFrameCount;
}



void CocoaWindowBase::setFullscreen(bool fullscreen) {
	
	if(fullscreen) {
		windowMode		= OF_FULLSCREEN;
		[glView goFullscreen];
	} else {
		windowMode		= OF_WINDOW;
		[glView goWindow];
	}
}

void CocoaWindowBase::toggleFullscreen() {
	if(windowMode == OF_FULLSCREEN) setFullscreen(false);
	else setFullscreen(true);
}


void CocoaWindowBase::enableSetupScreen(){
	bEnableSetupScreen = true;
};

void CocoaWindowBase::disableSetupScreen(){
	bEnableSetupScreen = false;
};


void CocoaWindowBase::initWindowSize() {
	if(windowSize.x && windowSize.y) setWindowShape(windowSize.x, windowSize.y);

	if(windowMode == OF_FULLSCREEN) setFullscreen(true);
}

