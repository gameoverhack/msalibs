#include "ofMain.h"
#include "testApp.h"
#include "ofxCocoa.h"


//========================================================================
int main( ){

	MSA::ofxCocoa::InitSettings			initSettings;
	initSettings.isOpaque				= false;
	initSettings.windowLevel			= NSMainMenuWindowLevel + 1;
	initSettings.hasWindowShadow		= false;
	initSettings.numFSAASamples			= 4;
	initSettings.initRect.size.width	= 1280;
	initSettings.initRect.size.height	= 720;
	initSettings.windowMode				= OF_WINDOW;
	
	// to go fullscreen across all windows:
//	initSettings.windowStyle			= NSBorderlessWindowMask;
//	initSettings.initRect				= MSA::ofxCocoa::rectForMainScreen();
	initSettings.initRect				= MSA::ofxCocoa::rectForAllScreens();
	
	MSA::ofxCocoa::AppWindow		cocoaWindow(initSettings);

	
	ofAppGlutWindow glutWindow;		// use this instead of cocoaWindow in ofSetupOpenGL below to use glut instead of cocoa
	
	ofSetupOpenGL(&cocoaWindow, 0, 0, 0);			// all other parameters are ignored, use initSettings above

	ofRunApp( new testApp());
}
