#pragma once

#include "MSAGuiControl.h"

namespace MSA {
	class GuiQuadWarp : public GuiControl {
	public:
		ofPoint			*pts;
		ofPoint			*curPoint;
		ofBaseDraws		*baseDraw;
		
		// for doubleclick
		ofPoint			lastPressPlace;
		float			lastPressTime;
		
		GuiQuadWarp(string name, ofBaseDraws &baseDraw, ofPoint *pts);
		void setup();
		void loadFromXML(ofxXmlSettings &XML);
		void saveToXML(ofxXmlSettings &XML);
		
		void onPress(int x, int y, int button);
		void onPressOutside(int x, int y, int button);
		void onDragOver(int x, int y, int button);
		void onDragOutside(int x, int y, int button);
		void draw(float x, float y);
	};
	
}