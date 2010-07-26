//
//  ofxAppDelegateBase.h
//  Cocoa Test
//
//  Created by Mehmet Akten on 16/08/2009.
//  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
//

// Shared between iPhone and Desktop 

#pragma once


#import "CocoaDefines.h"
#import "ofxCocoa.h"

@interface ofxAppDelegateBase : NSObject {
	NSTimer				*timer;
	
	float				targetFrameRate;
	
	uint64_t			frameLength;			// nanoseconds between successive frames at targetFrameRate
	bool				killThread;
	bool				loopInThreadIsEnabled;
}

-(void) setFrameRate:(float)frameRate;
-(void) runLoopInThread;
-(void) applicationDidFinishLaunching;

@end

/*************************************************************/


