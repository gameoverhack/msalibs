/*
 *  ofxCocoa.h
 *  Cocoa Test
 *
 *  Created by Mehmet Akten on 17/08/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

//@class ofxAppDelegate;
//@class CocoaWindowBase;
//@class GLWindow;
//@class GLView;

#import "ofxAppDelegate.h"
#import "CocoaWindowBase.h"
#import "GLWindow.h"
#import "GLView.h"

ofxAppDelegate*		ofxGetAppDelegate();
GLWindow*			ofxGetGLWindow(int index=0);
GLView*				ofxGetGLView(int index=0);
CocoaWindowBase*	ofxGetAppCocoaWindow();

//void				ofxRunLoopInThread();