//
//  ofxAppDelegate.m
//  Cocoa Test
//
//  Created by Mehmet Akten on 16/08/2009.
//  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
//

// Shared between iPhone and Desktop 



#import "GLee.h"
#import "ofxAppDelegate.h"
#import "ofxCocoa.h"


@implementation ofxAppDelegate

@synthesize glWindow;

static ofxAppDelegate* appDelegate = NULL;

+(ofxAppDelegate*)instance {
	return appDelegate;
}


- (void) startAnimation {
	[glView startAnimation];
}

- (void) stopAnimation {
	[glView stopAnimation];
}

- (void) toggleAnimation {
	[glView toggleAnimation];
}




-(void)createGLWindowAndView:(NSRect)windowRect {
	NSLog(@"createGLWindowAndView: ");
	glWindow	= [[GLWindow alloc] initWithContentRect:windowRect];		// release this?
	glView		= [[[GLView alloc] initWithFrame:NSMakeRect(0, 0, windowRect.size.width, windowRect.size.height)] autorelease];
	[glWindow setContentView:glView];
	[glWindow makeKeyAndOrderFront:self];
	[glWindow makeFirstResponder:glView];
}


- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication {
	return YES;
}


- (void)applicationDidFinishLaunching:(NSNotification*)n {
	NSLog(@"applicationDidFinishLaunching");
	
	appDelegate	= self;
	
	SetSystemUIMode(kUIModeAllHidden, NULL);
	NSRect rect = NSZeroRect;
	for(NSScreen *s in [NSScreen screens]) rect = NSUnionRect(rect, s.frame);
		
	[self createGLWindowAndView:rect];	
	
//	ofxGetAppCocoaWindow()->initWindowSize();
	
	ofGetAppPtr()->setup();
	
	[self startAnimation];
	
	// clear background
	glClearColor(ofBgColorPtr()[0], ofBgColorPtr()[1], ofBgColorPtr()[2], ofBgColorPtr()[3]);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

- (BOOL)applicationShouldTerminate:(NSNotification*)n {
	NSLog(@"applicationShouldTerminate");
	[self stopAnimation];
	return NSTerminateNow;
}



-(void) setFrameRate:(float)rate {
	[glView setFrameRate:rate];
}

-(void) dealloc {
	NSLog(@"ofxAppDelegate::dealloc");
	[glWindow release];
    [super dealloc];
}

@end
