/*
 *  ofxCocoa.h
 *  Cocoa Test
 *
 *  Created by Mehmet Akten on 17/08/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#pragma once

@class ofxAppDelegateBase;
class CocoaWindowBase;


ofxAppDelegateBase*	ofxGetAppDelegate();
CocoaWindowBase*	ofxGetCocoaWindow();
void				ofxRunLoopInThread();