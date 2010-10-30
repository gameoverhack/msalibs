#include "testApp.h"
#include "ofxCocoa.h"

void testApp::setup() {
	printf("Window dimensions: %i %i\n", ofGetWidth(), ofGetHeight());
	
	ofSetVerticalSync(true);	// limit to vsync 
	MSA::ofxCocoa::setSyncToDisplayLink(false);
	ofSetFrameRate(0);			// run as fast as you can
	
	MSA::ofxCocoa::showSystemUI(kUIModeNormal);
	MSA::ofxCocoa::setTransparent(false);
//	MSA::ofxCocoa::setWindowLevel(NSScreenSaverWindowLevel);
	
	
	lineWidth = 10;
}


void testApp::update(){
}


void testApp::draw(){
	static int x = 0;
	
//	glColor3f(ofRandomuf(), ofRandomuf(), ofRandomuf());
	ofCircle(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()), ofRandom(10, 100));

	ofRect(x, 0, lineWidth, ofGetHeight());
	x = (x + 10) % ofGetWidth();

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