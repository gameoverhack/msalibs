//
//  ofxAppDelegate.h
//  Cocoa Test
//
//  Created by Mehmet Akten on 16/08/2009.
//  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
//

// Shared between iPhone and Desktop 

#pragma once


#import "CocoaDefines.h"
#import "GLWindow.h"
#import "GLView.h"

@interface ofxAppDelegate : NSObject {
	GLWindow			*glWindow;
	GLView				*glView;
}


@property (readonly) GLWindow	*glWindow;

+(ofxAppDelegate*)instance;

-(void) setFrameRate:(float)rate;

@end

/*************************************************************/


