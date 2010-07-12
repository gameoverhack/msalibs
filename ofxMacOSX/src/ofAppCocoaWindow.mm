/*
 *  ofAppCocoaWindow.cpp
 *  Cocoa Test
 *
 *  Created by Mehmet Akten on 17/08/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#include "ofAppCocoaWindow.h"
#include "ofMain.h"

#import <AppKit/AppKit.h>

ofAppCocoaWindow::ofAppCocoaWindow() {
	NSLog(@"ofAppCocoaWindow::ofAppCocoaWindow()");
	numberOfFSAASamples		= 0;
}


void ofAppCocoaWindow::setUseFSAA(int numberOfFSAASamples) {
	this->numberOfFSAASamples = numberOfFSAASamples;
}


void ofAppCocoaWindow::setWindowPosition(int requestedX, int requestedY) {
	NSRect viewFrame = [glView frame];
	NSRect screenRect = [[NSScreen mainScreen] frame];
	
	NSPoint point;
	point.x = requestedX;
	point.y = screenRect.size.height - requestedY + viewFrame.origin.y; 
	
	[[glView window] setFrameTopLeftPoint:point];
}



void ofAppCocoaWindow::setWindowShape(int requestedWidth, int requestedHeight) {
	NSRect windowFrame  = [[glView window] frame];
	NSRect viewFrame = [glView frame];
	NSLog(@"ofAppCocoaWindow::setWindowShape requested:(%i %i) window:%@ view:%@", requestedWidth, requestedHeight, NSStringFromRect(windowFrame), NSStringFromRect(viewFrame));
	
	windowFrame.origin.y -= requestedHeight -  viewFrame.size.height;
	windowFrame.size = NSMakeSize(requestedWidth + windowFrame.size.width - viewFrame.size.width, requestedHeight + windowFrame.size.height - viewFrame.size.height);
	
	[[glView window] setFrame:windowFrame display:YES];
}

ofPoint	ofAppCocoaWindow::getScreenSize() {
	NSRect screenRect = [[[glView window] screen] frame];
	screenSize.set(screenRect.size.width, screenRect.size.height);
	return screenSize;
}

ofPoint	ofAppCocoaWindow::getWindowSize() {
	NSRect viewFrame = [glView frame];
	windowSize.set(viewFrame.size.width, viewFrame.size.height);
	return windowSize;
}


void ofAppCocoaWindow::setWindowTitle(string windowString) {
	NSString *stringFromUTFString = [[NSString alloc] initWithUTF8String:windowString.c_str() ];
	[ [glView window] setTitle: stringFromUTFString];
}



void ofAppCocoaWindow::keyPressed(int key){
	if(key == OF_KEY_ESC){
		OF_EXIT_APP(0);
	}
	ofGetAppPtr()->keyPressed(key);
}

void ofAppCocoaWindow::mousePressed(float x, float y, int button){
	ofGetAppPtr()->mousePressed(x, windowSize.y-y, button);
	ofGetAppPtr()->mouseX = x;
	ofGetAppPtr()->mouseY = windowSize.y-y;
}

void ofAppCocoaWindow::mouseDragged(float x, float y, int button){
	ofGetAppPtr()->mouseDragged(x, windowSize.y-y, button);
	ofGetAppPtr()->mouseX = x;
	ofGetAppPtr()->mouseY = windowSize.y-y;
}

void ofAppCocoaWindow::mouseReleased(int button){
	ofGetAppPtr()->mouseReleased(ofGetAppPtr()->mouseX, ofGetAppPtr()->mouseY, button);
	ofGetAppPtr()->mouseReleased();
}


void ofAppCocoaWindow::mouseMoved(float x, float y){
	ofGetAppPtr()->mouseMoved(x, windowSize.y-y);
	ofGetAppPtr()->mouseX = x;
	ofGetAppPtr()->mouseY = windowSize.y-y;
}





