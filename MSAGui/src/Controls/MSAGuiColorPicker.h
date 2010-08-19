#pragma once

#include "MSAGuiControl.h"

namespace MSA {
	
	class GuiColorPicker : public GuiControl {
	public:
		
		float		*value;
		float		min, max;
		
		float		barwidth[4];
		float		pct[4];
		
		GuiColorPicker(string name, float *value, float max = 1);
		void setup();
		void loadFromXML(ofxXmlSettings &XML);
		void saveToXML(ofxXmlSettings &XML);
		float getValue(int i);
		void setValue(float f, int i);
		void updateSlider();	
		void onPress(int x, int y, int button);
		void onDragOver(int x, int y, int button);
		void onDragOutside(int x, int y, int button);
		void update();
		void draw(float x, float y);
	};
}