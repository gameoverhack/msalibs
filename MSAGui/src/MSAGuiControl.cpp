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


#include "MSAGuiControl.h"

namespace MSA {
	
	GuiControl::GuiControl(string name) {
		controlType = "";
		this->config = &defaultSimpleGuiConfig;
		setName(name);
		setKey(key);
		setPos(0, 0);
		lock	  = false;
		focused	  = false;
		newColumn = false;
		setKeyboardShortcut(0);
		
		setup();
		
		disableAllEvents();		// just for safety to make sure nothing is registered twice
		//	enableAppEvents();
		//	enableMouseEvents();
		//	disableKeyEvents();
	}
	
	GuiControl &GuiControl::setConfig(GuiConfig *config) {
		this->config = config;
		setup();
		return *this;
	}
	
	GuiControl &GuiControl::setNewColumn(bool b) {
		newColumn = b;
		return *this;
	}
	
	GuiControl &GuiControl::setName(string newName) {
		name = newName;
		origName = name;
		if(key.compare("") == 0) setKey("");	// if key has not been set yet, set name as key too
		
		hasTitle = (name.compare("") != 0);
		return *this;
	}
	
	
	GuiControl &GuiControl::setKey(string newKey) {
		if(newKey.compare("") == 0) key = name;
		else key = newKey;
		for(int i=0; i<key.size(); i++) {
			if(key[i] == ' ') key[i] = '_';
		}
		return *this;
	}
	
	GuiControl &GuiControl::setTextColor(bool clickable) {
		if(isMouseOver() && clickable) ofSetColor(config->textOverColor);
		else ofSetColor(config->textColor);
		return *this;
	}
	
	GuiControl &GuiControl::setTextBGColor(bool clickable) {
		if(isMouseOver() && clickable) ofSetColor(config->textBGOverColor);
		else ofSetColor(config->textBGColor);
		return *this;
	}
	
	GuiControl &GuiControl::setFullColor(bool forceActive) {
		if(isMouseDown() || forceActive) ofSetColor(config->fullActiveColor);
		else if(isMouseOver()) ofSetColor(config->fullOverColor);
		else ofSetColor(config->fullColor);
		return *this;
	}
	
	GuiControl &GuiControl::setEmptyColor() {
		ofSetColor(config->emptyColor);
		//		if(isMouseOver()) ofSetColor(config->overColor.r, config->overColor.g, config->overColor.b);
		//		if(focused && !isMouseOver()) ofSetColor(config->focusColor.r, config->focusColor.g, config->focusColor.b);
		return *this;
	}
	
	GuiControl &GuiControl::setKeyboardShortcut(char c) {
		keyboardShortcut = c;
		if(c) {
			//	printf("MSAGuiControl::setKeyboardShortcut %s %c\n", name.c_str(), c);
			name = origName + " (" + c + ")";
		} else {
			name = origName;
		}
		return *this;
	}
	
	
}


