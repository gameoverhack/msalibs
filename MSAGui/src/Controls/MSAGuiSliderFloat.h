#pragma once

#include "MSAGuiSliderBase.h"

namespace MSA {
	class GuiSliderFloat : public GuiSliderBase<float> {
		
	public:
		GuiSliderFloat(string name, float &value, float min, float max) : GuiSliderBase<float>(name, value, min, max) {
			controlType = "SliderFloat";
		}
	};
}