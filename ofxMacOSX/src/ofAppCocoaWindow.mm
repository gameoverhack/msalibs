/*
 *  ofAppCocoaWindow.cpp
 *  Cocoa Test
 *
 *  Created by Mehmet Akten on 17/08/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#include "ofAppCocoaWindow.h"
#import "ofxCocoa.h"

#import <AppKit/AppKit.h>

ofAppCocoaWindow::ofAppCocoaWindow() {
	NSLog(@"ofAppCocoaWindow::ofAppCocoaWindow()");
	numberOfFSAASamples		= 0;
}


void ofAppCocoaWindow::setUseFSAA(int numberOfFSAASamples) {
	this->numberOfFSAASamples = numberOfFSAASamples;
}


void ofAppCocoaWindow::setWindowPosition(int requestedX, int requestedY) {
	NSRect viewFrame = [ofxGetGLView() frame];
	NSRect screenRect = [[NSScreen mainScreen] frame];
	
	NSPoint point;
	point.x = requestedX;
	point.y = screenRect.size.height - requestedY + viewFrame.origin.y; 
	
	[ofxGetGLWindow() setFrameTopLeftPoint:point];
}



void ofAppCocoaWindow::setWindowShape(int requestedWidth, int requestedHeight) {
	NSRect windowFrame  = [ofxGetGLWindow() frame];
	NSRect viewFrame = [ofxGetGLView() frame];
	NSLog(@"ofAppCocoaWindow::setWindowShape requested:(%i %i) window:%@ view:%@", requestedWidth, requestedHeight, NSStringFromRect(windowFrame), NSStringFromRect(viewFrame));
	
	windowFrame.origin.y -= requestedHeight -  viewFrame.size.height;
	windowFrame.size = NSMakeSize(requestedWidth + windowFrame.size.width - viewFrame.size.width, requestedHeight + windowFrame.size.height - viewFrame.size.height);
	
	[ofxGetGLWindow() setFrame:windowFrame display:YES];
	
	[ofxGetGLWindow() windowDidResize:NULL];
}

ofPoint	ofAppCocoaWindow::getScreenSize() {
	NSRect screenRect = [[ofxGetGLWindow() screen] frame];
	screenSize.set(screenRect.size.width, screenRect.size.height);
	return screenSize;
}

ofPoint	ofAppCocoaWindow::getWindowSize() {
	NSRect viewFrame = [ofxGetGLView() frame];
	windowSize.set(viewFrame.size.width, viewFrame.size.height);
	return windowSize;
}


void ofAppCocoaWindow::setWindowTitle(string windowString) {
	NSString *stringFromUTFString = [[NSString alloc] initWithUTF8String:windowString.c_str() ];
	[ ofxGetGLWindow() setTitle: stringFromUTFString];
}
