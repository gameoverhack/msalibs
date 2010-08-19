#pragma once

#include "MSAGuiControl.h"

namespace MSA {
	
	class GuiContent  : public GuiControl {
	public:
		float			fixwidth;
		float			fixheight;
		ofBaseDraws		*content;
		
		GuiContent(string name, ofBaseDraws& content, float fixwidth=250.0);
		void setup();
		void draw(float x, float y);
	};
}