#include "testApp.h"
#include "ofxCocoa.h"

void testApp::setup() {
	printf("Window dimensions: %i %i\n", ofGetWidth(), ofGetHeight());
	
	ofBackground(0, 0, 0);
	
	doVSync			= true;
	doDisplayLink	= true;
	
	ofSetVerticalSync(doVSync);	
	MSA::ofxCocoa::setSyncToDisplayLink(doDisplayLink);
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
//	ofCircle(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()), ofRandom(10, 100));

	for(int i=0; i<20; i++ ) {
		ofRect((x + i * ofGetWidth()/20) % ofGetWidth(), 0, lineWidth, ofGetHeight());
	}
	x = (x + 10) % ofGetWidth();

	ofDrawBitmapString(ofToString(ofGetFrameRate(), 2) + " | doDisplayLink: " + ofToString(doDisplayLink) + " | doVSync: " + ofToString(doVSync) , 20, 20);
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
			doDisplayLink ^= true;
			MSA::ofxCocoa::setSyncToDisplayLink(doDisplayLink);
			break;
			
		case 'v':
			doVSync ^= true;
			ofSetVerticalSync(doVSync);	
			break;
			
			
	}
}