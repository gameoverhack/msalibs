
#include "MSAGuiToggle.h"

namespace MSA {
	GuiToggle::GuiToggle(string name, bool &value) : GuiControl(name) {
		this->value	= &value;
		setMomentary(false);
		controlType = "Toggle";
		setup();
	}
	
	GuiToggle& GuiToggle::setMomentary(bool m) {
		momentary = m;
		return *this;
	}
	
	
	void GuiToggle::setup() {
		setSize(config->gridSize.x - config->padding.x, config->toggleHeight);
	}
	
	void GuiToggle::loadFromXML(ofxXmlSettings &XML) {
		setValue(XML.getValue(controlType + "_" + key + ":value", 0));
	}
	
	void GuiToggle::saveToXML(ofxXmlSettings &XML) {
		XML.addTag(controlType + "_" + key);
		XML.pushTag(controlType + "_" + key);
		XML.addValue("name", name);
		XML.addValue("value", getValue());
		XML.popTag();
	}
	
	
	bool GuiToggle::getValue() {
		return (*value);
	}
	
	void GuiToggle::setValue(bool b) {
		(*value) = b;
	}
	
	void GuiToggle::toggle() {
		(*value) = !(*value); 
	}
	
	void GuiToggle::onPress(int x, int y, int button) {
		if(momentary) setValue(true);
		else toggle();
	}
	
	void GuiToggle::onRelease(int x, int y, int button) {
		if(momentary) setValue(false);
	}
	
	void GuiToggle::keyPressed( int key ) {
		if(key==keyboardShortcut) onPress(0, 0, 0);
	}
	
	void GuiToggle::keyReleased( int key ) {
		if(key==keyboardShortcut) onRelease(0, 0, 0);
	}
	
	void GuiToggle::onKeyEnter() {
		toggle();
	}
	
	void GuiToggle::update() {
		//	if(!enabled) return;
		//	enabled = false;
	}
	
	void GuiToggle::draw(float x, float y) {
		//	enabled = true;
		setPos(x, y);
		
		glPushMatrix();
		glTranslatef(x, y, 0);
		
		ofEnableAlphaBlending();
		ofFill();
		setFullColor(*value);
		ofRect(0, 0, height, height);
		
		if((*value)) {
			setTextColor();
			ofLine(0, 0, height, height);
			ofLine(height, 0, 0, height);
		}
		
		setTextBGColor();
		ofRect(height, 0, width - height, height);
		
		setTextColor();
		ofDrawBitmapString(name, height + 15, 15);
		ofDisableAlphaBlending();
		
		glPopMatrix();
	}
}
