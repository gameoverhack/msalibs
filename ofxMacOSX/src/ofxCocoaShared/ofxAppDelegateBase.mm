//
//  ofxAppDelegateBase.m
//  Cocoa Test
//
//  Created by Mehmet Akten on 16/08/2009.
//  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
//

// Shared between iPhone and Desktop 


#import "ofxAppDelegateBase.h"
#import "CocoaWindowBase.h"

#include "ofMain.h"
#include <mach/mach_time.h>

static ofxAppDelegateBase *appDelegate = NULL;
ofxAppDelegateBase* ofxGetAppDelegate() {
	return appDelegate;
}


void ofxRunLoopInThread() {
	[appDelegate runLoopInThread];
}


@implementation ofxAppDelegateBase


-(void) timerLoop {
	//	NSLog(@"ofxAppDelegateBase::timerLoop");
	
	// create autorelease pool in case anything needs it
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
	ofxGetCocoaWindow()->timerLoop();
	
	// release pool
	[pool release];
}



-(void) timerLoopThreaded:(id*)sender {
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
	mach_timebase_info_data_t info;
	mach_timebase_info(&info); 
	while(!killThread) {
		//		NSLog(@"ofxAppDelegateBase::timerLoopThreaded");
		
		uint64_t startTime = mach_absolute_time() * info.numer / info.denom;
		ofxGetCocoaWindow()->timerLoop();
		
		uint64_t endTime = mach_absolute_time() * info.numer / info.denom;
		//		uint64_t diffTime = endTime - startTime;
		
		uint64_t targetStart = startTime + frameLength;
		if(targetStart > endTime) [NSThread sleepForTimeInterval:(targetStart - endTime) * 1e-9];
	}
	
	[pool release];
}


-(void) runLoopInThread {
	NSLog(@"ofxAppDelegateBase::runLoopInThread");
	loopInThreadIsEnabled = YES;
}


- (void)applicationDidFinishLaunching {
	NSLog(@"ofxAppDelegateBase::applicationDidFinishLaunching");
	
	appDelegate = self;
	
	ofxGetCocoaWindow()->initWindowSize();
	
	// zero targetFrameRate (to see if user setes it in setup()
	targetFrameRate = -1;
	
	ofGetAppPtr()->setup();
	
	if(targetFrameRate == -1) [self setFrameRate:60];
	
	// clear background
	glClearColor(ofBgColorPtr()[0], ofBgColorPtr()[1], ofBgColorPtr()[2], ofBgColorPtr()[3]);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// From Robert Carlsen
	if(loopInThreadIsEnabled) [NSThread detachNewThreadSelector:@selector(timerLoopThreaded:) toTarget:self withObject:nil];
}


-(void) setFrameRate:(float)rate {
	targetFrameRate = rate;
	frameLength = targetFrameRate ? 1e9/targetFrameRate : 0;
	
	if(!loopInThreadIsEnabled) {
		NSLog(@"setFrameRate %.3f using NSTimer", rate);
		
		[timer invalidate];
		if(rate) {
//			timer = [[NSTimer scheduledTimerWithTimeInterval:(1.0f / rate) target:self selector:@selector(timerLoop) userInfo:nil repeats:YES] retain];
			timer = [[NSTimer timerWithTimeInterval:(1.0f / rate) target:self selector:@selector(timerLoop) userInfo:nil repeats:YES] retain];
			[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
			[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSModalPanelRunLoopMode];
			[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSEventTrackingRunLoopMode];
			
		}
	} else {
		NSLog(@"setFrameRate %.3f using NSThread", rate);
		
	}
	
}

-(void) dealloc {
	NSLog(@"ofxAppDelegateBase::dealloc");
	if(!loopInThreadIsEnabled) {
		[timer release];
	}
    [super dealloc];
}

@end
