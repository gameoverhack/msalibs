#include "testApp.h"


void testApp::setup() {
	printf("Window dimensions: %i %i\n", ofGetWidth(), ofGetHeight());
	
	ofSetVerticalSync(true);	// limit to vsync 
	MSA::ofxCocoa::setSyncToDisplayLink(false);
	ofSetFrameRate(0);			// run as fast as you can
	
	MSA::ofxCocoa::showSystemUI(kUIModeNormal);
	MSA::ofxCocoa::setTransparent(false);
//	MSA::ofxCocoa::setWindowLevel(NSScreenSaverWindowLevel);
	
	lineWidth = 10;
	
	qcGrid.load("msaSphereGrid.qtz");
	
	qcBox.load("box.qtz");
	
	ofBackground(60, 20, 10);
}


void testApp::update(){
}


void testApp::draw(){
	static int x = 0;
	
	ofRect(x, 0, lineWidth, ofGetHeight());
	x = (x + 10) % ofGetWidth();
	
	qcGrid.draw();
	qcBox.draw();

	ofDrawBitmapString(ofToString(ofGetFrameRate(), 2), 20, 20);
}


void testApp::keyPressed(int key){
	switch(key) {
		case 'f':
			ofToggleFullscreen();
			break;
			
		case 'm':
		{
			static bool showCursor = true;
			showCursor ^= true;
			if(showCursor) ofShowCursor();
			else ofHideCursor();
		}
			break;
			
		case 't':
			MSA::ofxCocoa::setTransparent(!MSA::ofxCocoa::getTransparent());
			break;
			
			
		case 'd':
			MSA::ofxCocoa::setSyncToDisplayLink(!MSA::ofxCocoa::getSyncToDisplayLink());
			break;
	}
}