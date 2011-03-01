#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxCocoa.h"
#include "ofxQuartzComposition.h"


class testApp : public ofBaseApp{
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	
	float	lineWidth;

	MSA::ofxQuartzComposition qcBox;
	MSA::ofxQuartzComposition qcGrid;
};

#endif
