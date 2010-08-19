#pragma once

#include "MSAGuiControl.h"

namespace MSA {
	class GuiFPSCounter : public GuiControl {
	public:
		GuiFPSCounter();	
		void setup();
		void draw(float x, float y);
	};
}