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

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

#include "MSAGuiIncludes.h"

namespace MSA {
		
		class Gui {
			
		public:
			int							guiFocus;
			
			GuiConfig			*config;	
			
			Gui();
			void						setup();
			
			
			void						loadFromXML();
			void						saveToXML();	
			void						setAutoSave(bool b);
			void						setAlignRight(bool b);
			void						setDefaultKeys(bool b);
			
			//	int		getValueI(string nameID);
			//	float	getValueF(string nameID);
			//	bool	getValueB(string nameID);
			
			void						drawFocus(float x, float y);
			
			
			void						setDraw(bool b);
			void						toggleDraw();
			void						show();		// simply calls setDraw(true);
			void						hide();		// simply calls setDraw(false);
			bool						isOn();
			
			void						nextPage();
			void						prevPage();
			void						setPage(int i);				// 1 based index of page
			void						setPage(string name);
			
			void						nextPageWithBlank();		// cycles through pages, and closes after last page
			
			GuiPage&			page(int i);				// 1 based index of page
			GuiPage&			page(string name);			// returns page by name
			GuiPage&			currentPage();				// returns current page
			vector <GuiPage*>&	getPages();
			
			GuiControl			&control(string name);		// returns control by name
			
			
			GuiPage			&addPage(string name = "");
			GuiControl			&addControl(GuiControl& control);
			GuiContent			&addContent(string name, ofBaseDraws &content, float fixwidth = -1);
			GuiButton			&addButton(string name, bool &value);
			GuiFPSCounter		&addFPSCounter();
			//	GuiMovieSlider		&addMovieSlider(string name, ofVideoPlayer& input);
			GuiQuadWarp		&addQuadWarper(string name, ofBaseDraws &baseDraw, ofPoint *pts);
			GuiSliderInt		&addSlider(string name, int &value, int min, int max);
			GuiSliderFloat		&addSlider(string name, float &value, float min, float max);
			GuiSlider2d		&addSlider2d(string name, ofPoint& value, float xmin, float xmax, float ymin, float ymax);
			GuiTitle			&addTitle(string name="", float height = 0);
			GuiToggle			&addToggle(string name, bool &value);
			GuiColorPicker		&addColorPicker(string name, float *values);
			GuiComboBox        &addComboBox(string name, int &value, int numChoices, string* choiceTitles=NULL);
			
			
			void						draw();
			
		protected:
			bool							doAutoSave;
			bool							alignRight;
			bool							doDefaultKeys;
			bool							doSave;//, doSaveBackup;
			bool							changePage;
			int								currentPageIndex;			// 1 based index of page (0 is for global controls)
			
			//	ofxXmlSettings					XML;
			//	string							xmlFilename;
			
			bool							doDraw;
			float							border;
			
			GuiPage				*headerPage;
			GuiButton				*titleButton;
			vector <GuiPage*>		pages;				// 0 is for headerPage
			
			void addListeners();
			void removeListeners();
			
			//	void setup(ofEventArgs &e);
			void update(ofEventArgs &e);
			//  void draw(ofEventArgs &e);
			//	void exit(ofEventArgs &e);
			
			void mouseMoved(ofMouseEventArgs &e);
			void mousePressed(ofMouseEventArgs &e);	
			void mouseDragged(ofMouseEventArgs &e);	
			void mouseReleased(ofMouseEventArgs &e);
			
			void keyPressed(ofKeyEventArgs &e);
			void keyReleased(ofKeyEventArgs &e);
		};

	extern Gui gui;
}




