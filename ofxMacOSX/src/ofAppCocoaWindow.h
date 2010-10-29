/*
 *  ofAppCocoaWindow.h
 *  Cocoa Test
 *
 *  Created by Mehmet Akten on 17/08/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#pragma once

#include "CocoaWindowBase.h"

@class GLView;

class ofAppCocoaWindow : public CocoaWindowBase {
public:
	ofAppCocoaWindow();
	
	void				setUseFSAA(int numberOfFSAASamples = 4);

	virtual void		setWindowPosition(int x, int y);
	virtual void		setWindowShape(int w, int h);
	virtual void		setWindowTitle(string windowString);
	virtual ofPoint		getScreenSize();
	virtual ofPoint		getWindowSize();

	int					numberOfFSAASamples;
};