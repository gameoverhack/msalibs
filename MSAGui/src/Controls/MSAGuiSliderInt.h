#pragma once

#include "MSAGuiSliderBase.h"


namespace MSA {
	class GuiSliderInt : public GuiSliderBase<int> {
	public:
		GuiSliderInt(string name, int &value, int min, int max) : GuiSliderBase<int>(name, value, min, max) {
			controlType = "SliderInt";
			setIncrement(1);
		}
	};
}