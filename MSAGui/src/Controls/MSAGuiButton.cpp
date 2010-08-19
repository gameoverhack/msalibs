
#include "MSAGuiButton.h"

namespace MSA {
	
	GuiButton::GuiButton(string name, bool &value) : GuiValueControl<bool>(name, value) {
		beToggle	= false;
		beenPressed = false;
		controlType = "Button";
		setup();
	}
	
	void GuiButton::setup() {
		setSize(config->gridSize.x - config->padding.x, config->buttonHeight);
	}
	
	void GuiButton::loadFromXML(ofxXmlSettings &XML) {
		setValue(XML.getValue(controlType + "_" + key + ":value", 0));
	}
	
	void GuiButton::saveToXML(ofxXmlSettings &XML) {
		XML.addTag(controlType + "_" + key);
		XML.pushTag(controlType + "_" + key);
		XML.addValue("name", name);
		XML.addValue("value", getValue());
		XML.popTag();
	}
	
	void GuiButton::keyPressed( int key ) {
		if(key==keyboardShortcut) toggle();
	}
	
	
	
	bool GuiButton::getValue() {
		return (*value);
	}
	
	void GuiButton::setValue(bool b) {
		(*value) = b;
	}
	
	void GuiButton::toggle() {
		(*value) = !(*value); 
	}
	
	void GuiButton::setToggleMode(bool b) {
		beToggle = b;
	}
	
	void GuiButton::onPress(int x, int y, int button) {
		beenPressed = true;	
		if(beToggle) (*value) = !(*value); 
		else (*value) = true;
	}
	
	void GuiButton::onRelease(int x, int y, int button) {
		if(!beToggle) (*value) = false;
	}
	
	void GuiButton::draw(float x, float y) {
		setPos(x, y);
		
		glPushMatrix();
		glTranslatef(x, y, 0);
		
		ofEnableAlphaBlending();
		ofFill();
		setTextBGColor();
		ofRect(0, 0, width, height);
		
		// if a toggle
		if((*value) && beToggle) {
			setTextColor();
			//ofLine(0, 0, box.width, box.height);
			//ofLine(box.width, 0, 0, box.height);
		}
		
		setTextColor();
		ofDrawBitmapString(name, 3, 15);
		
		ofDisableAlphaBlending();
		
		glPopMatrix();
	}
}