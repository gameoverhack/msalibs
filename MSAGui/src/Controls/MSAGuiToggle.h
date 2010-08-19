#pragma once

#include "MSAGuiControl.h"

namespace MSA {
	class GuiToggle : public GuiControl {
		
	public:
		bool	*value;
		bool	momentary;
		
		GuiToggle(string name, bool &value);
		GuiToggle& setMomentary(bool m);
		void setup();
		void loadFromXML(ofxXmlSettings &XML);
		void saveToXML(ofxXmlSettings &XML);
		bool getValue();
		void setValue(bool b);
		void toggle();
		void onPress(int x, int y, int button);
		void onRelease(int x, int y, int button);
		void keyPressed( int key );
		void keyReleased( int key );
		void onKeyEnter();
		void update();
		void draw(float x, float y);
	};
}