//
//  CocoaWindowBase.h
//  Cocoa Test
//
//  Created by Mehmet Akten on 16/08/2009.
//  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
//

// Shared between iPhone and Desktop 


#pragma once

#import "ofAppBaseWindow.h"
#import "GLViewProtocol.h"
#import "ofxCocoa.h"

class CocoaWindowBase : public ofAppBaseWindow {
public:
	
	CocoaWindowBase();
	virtual ~CocoaWindowBase(){};
	
	virtual void		setupOpenGL(int w, int h, int screenMode);
	virtual void		initializeWindow();
	virtual void		runAppViaInfiniteLoop(ofBaseApp * appPtr);
	
	virtual void		hideCursor() {};
	virtual void		showCursor() {};
	
	virtual void		setWindowPosition(int x, int y) {}
	virtual void		setWindowShape(int w, int h) {}
	
	virtual int			getFrameNum();
	virtual float		getFrameRate();
	
	virtual ofPoint		getWindowPosition();
	virtual ofPoint		getWindowSize() { return windowSize; };
	virtual ofPoint		getScreenSize() { return screenSize; };
	
	virtual void		setFrameRate(float targetRate);
	virtual void		setWindowTitle(string title) {}
	
	virtual int			getWindowMode();
	
	virtual void		setFullscreen(bool fullscreen);
	virtual void		toggleFullscreen();
	
	virtual void		enableSetupScreen();
	virtual void		disableSetupScreen();
	
	virtual void		update();
	virtual void		render(int width, int height);
	virtual void		timerLoop();
	
	virtual void		initWindowSize();
	
	id<GLViewProtocol>	glView;
	
	int					windowMode;
	ofPoint				screenSize;
	ofPoint				windowSize;
	ofPoint				windowPos;
	
	float				timeNow, timeThen, fps;
	int					nFrameCount;
	bool				bEnableSetupScreen;
	float				frameRate;
};

