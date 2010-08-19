/***********************************************************************
 
 Copyright (c) 2008, 2009, 2010, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
 
 based on Todd Vanderlin's Gui API
 http://toddvanderlin.com/
 
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ***********************************************************************/


#include "MSAGuiPage.h"

namespace MSA {
	
	GuiPage::GuiPage(string name) : GuiControl(name) {
		disableAllEvents();
		width = 0;
		height = ofGetHeight();
		eventStealingControl = NULL;
		setXMLName(name + "_settings.xml");
	}
	
	GuiPage::~GuiPage() {
		// delete all controls
	}
	
	
	GuiPage &GuiPage::setXMLName(string s) {
		xmlFilename = s;
		return *this;
	}
	
	
	void GuiPage::loadFromXML() {
		ofLog(OF_LOG_VERBOSE, "MSAGuiPage::loadFromXML: " + xmlFilename);
		
		if(xmlFilename.compare("") == 0) return;
		
		if(XML.loadFile(xmlFilename) == false) {
			ofLog(OF_LOG_ERROR, "Error loading xmlFilename: " + xmlFilename);
			return;
		}
		
		XML.pushTag("controls");
		for(int i=0; i < controls.size(); i++) {
			controls[i]->loadFromXML(XML);
		}
		XML.popTag();
	}
	
	
	void GuiPage::saveToXML() {
		if(controls.size() <= 1 || xmlFilename.compare("") == 0) return;	// if it has no controls (title counts as one control)
		
		XML.saveFile(xmlFilename + ".bak");
		
		XML.clear();	// clear cause we are building a new xml file
		
		XML.addTag("controls");
		XML.pushTag("controls");
		for(int i=0; i < controls.size(); i++) {
			controls[i]->saveToXML(XML);
		}
		XML.popTag();
		
		XML.saveFile(xmlFilename);
		//	if(doSaveBackup) 
		ofLog(OF_LOG_VERBOSE, "MSAGuiPage::saveToXML: " + xmlFilename + " " + ofToString(controls.size(), 0) + " items");
	}
	
	
	float GuiPage::getNextY(float y) {
		return y;
		int iy = (int)ceil(y/config->gridSize.y);
		return (iy) * config->gridSize.y;
	}
	
	
	void GuiPage::draw(float x, float y, bool alignRight) {
		setPos(x += config->offset.x, y += config->offset.y);
		if(alignRight) x = ofGetWidth() - x -  config->gridSize.x;
		
		float posX		= 0;
		float posY		= 0;
		float stealingX = 0;
		float stealingY = 0;
		
		ofSetRectMode(OF_RECTMODE_CORNER);
		
//		ofEnableAlphaBlending();
//		glColor4f(0, 0, 0, 0.5);
//		ofFill();
//		ofRect(posX, posY, width, height);
//		ofDisableAlphaBlending();
		
		width = 0;
		//height = 0;
		
		for(int i=0; i<controls.size(); i++) {
			GuiControl &control = *controls[i];
			
			if(control.newColumn) {
				if(alignRight) posX -= config->gridSize.x;
				else posX += config->gridSize.x;
				posY = 0;
			}
			
			float controlX = posX + x;
			float controlY = posY + y;
			
			//we don't draw the event stealing controls until the end because they can expand and overlap with other controls (e.g. combo box)
			if(eventStealingControl == &control) {
				stealingX = controlX;
				stealingY = controlY;
			} else {
				//			printf("drawing control: %s %s\n", control.controlType.c_str(), control.name.c_str());
				control.draw(controlX, controlY);
			}
			
			if(control.hasTitle) {
				ofNoFill();
				ofSetColor(config->borderColor);
				glLineWidth(0.5f);
				ofRect(controlX, controlY, control.width, control.height);
			}
			posY = getNextY(posY + control.height + config->padding.y);
			
			if(posY + y >= height - control.height - config->padding.y) {
				if(alignRight) posX -= config->gridSize.x;
				else posX += config->gridSize.x;
				posY = 0;
			}
			
			width	= MAX(width, posX + control.width);
//			height	= MAX(height, posY + control.height);
			
			//		if(guiFocus == controls[i]->guiID) controls[i]->focused = true;		// MEMO
			//		else							   controls[i]->focused = false;
		}
		
		
		//event stealing controls get drawn on top
		if(eventStealingControl) {
			eventStealingControl->draw(stealingX, stealingY);
			if(eventStealingControl->hasTitle) {
				ofNoFill();
				ofSetColor(config->borderColor);
				glLineWidth(0.5f);
				ofRect(stealingX, stealingY, eventStealingControl->width, eventStealingControl->height);
			}
		}
	}
	
	
	GuiControl &GuiPage::addControl(GuiControl& control) {
		controls.push_back(&control);
//		width += control.width + config->padding.x;
		return control;
	}
	
	GuiButton &GuiPage::addButton(string name, bool &value) {
		return (GuiButton &)addControl(* new GuiButton(name, value));
	}
	
	GuiContent &GuiPage::addContent(string name, ofBaseDraws &content, float fixwidth) {
		if(fixwidth == -1) fixwidth = config->gridSize.x - config->padding.x;
		return (GuiContent &)addControl(* new GuiContent(name, content, fixwidth));
	}
	
	GuiFPSCounter &GuiPage::addFPSCounter() {
		return (GuiFPSCounter &)addControl(* new GuiFPSCounter());
	}
	
	GuiQuadWarp &GuiPage::addQuadWarper(string name, ofBaseDraws &baseDraw, ofPoint *pts) {
		return (GuiQuadWarp &)addControl(* new GuiQuadWarp(name, baseDraw, pts));
	}
	//
	//GuiMovieSlider &GuiPage::addMovieSlider(string name, ofVideoPlayer& input) {
	//	return (GuiMovieSlider &)addControl(* new GuiMovieSlider(name, input));
	//}
	
	GuiSliderInt &GuiPage::addSlider(string name, int &value, int min, int max) {
		return (GuiSliderInt &)addControl(* new GuiSliderInt(name, value, min, max));
	}
	
	GuiSliderFloat &GuiPage::addSlider(string name, float &value, float min, float max) {
		return (GuiSliderFloat &)addControl(* new GuiSliderFloat(name, value, min, max));
	}
	
	GuiSlider2d &GuiPage::addSlider2d(string name, ofPoint& value, float xmin, float xmax, float ymin, float ymax) {
		return (GuiSlider2d &)addControl(* new GuiSlider2d(name, value, xmin, xmax, ymin, ymax));
	}
	
	GuiTitle &GuiPage::addTitle(string name, float height) {
		return (GuiTitle &)addControl(* new GuiTitle(name, height));
	}
	
	GuiToggle &GuiPage::addToggle(string name, bool &value) {
		return (GuiToggle &)addControl(* new GuiToggle(name, value));
	}
	
	GuiColorPicker &GuiPage::addColorPicker(string name, float *values) {
		return (GuiColorPicker &)addControl(* new GuiColorPicker(name, values));
	}
	
	
	GuiComboBox &GuiPage::addComboBox(string name, int &choice_out, int numChoices, string* choiceTitles) {
		return (GuiComboBox &)addControl(* new GuiComboBox(name, choice_out, numChoices, this, choiceTitles));
	}
	
	
	
	
	void GuiPage::update(ofEventArgs &e) {
		for(int i=0; i<controls.size(); i++) controls[i]->update();
	}
	
	void GuiPage::SetEventStealingControl(GuiControl &control) {
		eventStealingControl = &control;
	}
	void GuiPage::ReleaseEventStealingControl() {
		eventStealingControl = NULL;
	}
	
	void GuiPage::mouseMoved(ofMouseEventArgs &e) {
		if(eventStealingControl)
			eventStealingControl->_mouseMoved(e);
		else
			for(int i=0; i<controls.size(); i++) controls[i]->_mouseMoved(e);
	}
	
	void GuiPage::mousePressed(ofMouseEventArgs &e) {
		if(eventStealingControl)
			eventStealingControl->_mousePressed(e);
		else
			for(int i=0; i<controls.size(); i++) controls[i]->_mousePressed(e);
	}
	
	void GuiPage::mouseDragged(ofMouseEventArgs &e) {
		if(eventStealingControl)
			eventStealingControl->_mouseDragged(e);
		else
			for(int i=0; i<controls.size(); i++) controls[i]->_mouseDragged(e);
	}
	
	void GuiPage::mouseReleased(ofMouseEventArgs &e) {
		if(eventStealingControl)
			eventStealingControl->_mouseReleased(e);
		else
			for(int i=0; i<controls.size(); i++) controls[i]->_mouseReleased(e);
	}
	
	void GuiPage::keyPressed(ofKeyEventArgs &e) {
		bool keyUp		= e.key == OF_KEY_UP;
		bool keyDown	= e.key == OF_KEY_DOWN;
		bool keyLeft	= e.key == OF_KEY_LEFT;
		bool keyRight	= e.key == OF_KEY_RIGHT;
		bool keyEnter	= e.key == OF_KEY_RETURN;
		
		for(int i=0; i<controls.size(); i++) {
			GuiControl *c = controls[i];
			if(c->isMouseOver()) {
				if(keyUp)		c->onKeyUp();
				if(keyDown)		c->onKeyDown();
				if(keyLeft)		c->onKeyLeft();
				if(keyRight)	c->onKeyRight();
				if(keyEnter)	c->onKeyEnter();
				c->_keyPressed(e);
			}
		}
	}
	
	void GuiPage::keyReleased(ofKeyEventArgs &e) {
		for(int i=0; i<controls.size(); i++) if(controls[i]->isMouseOver()) controls[i]->_keyReleased(e);
	}
	
	
	vector <GuiControl*>&	GuiPage::getControls() {
		return controls;
	}
	
}
