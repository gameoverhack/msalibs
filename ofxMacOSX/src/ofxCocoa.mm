/*
 *  ofxCocoaUtils.cpp
 *  Cosmic Sensation Cocoa
 *
 *  Created by Memo on 9/30/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxCocoa.h"
//#import "ofxAppDelegate.h"
//#import "GLView.h"
//#import "GLWindow.h"


ofxAppDelegate* ofxGetAppDelegate() {
	return [ofxAppDelegate instance];
}

GLWindow* ofxGetGLWindow(int index) {
	return [ofxGetAppDelegate() glWindow];
}

GLView*	ofxGetGLView(int index) {
	return [ofxGetGLWindow(index) contentView];
}




//void ofxRunLoopInThread() {
//	[ofxGetAppDelegate() runLoopInThread];
//}
