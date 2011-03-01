/***********************************************************************
 
 Copyright (c) 2008, 2009, 2010 Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
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

#include "ofxDirList.h"
#include "MSADataProtector.h"
#include "ofxMSAImage.h"

namespace MSA {
	
	template <typename T> class DirManager {
	public:
		ofxDirList DIR;

		void setup(string path, const char *ext = NULL, string* md5 = NULL) {
			currentIndex = 0;
			if(ext) DIR.allowExt(ext);
			int numFiles = DIR.listDir(path);
			
			ofLog(OF_LOG_VERBOSE, "DirManager::setup( " + path + " ) | " + ofToString(numFiles) + " files loaded");
			for(int i = 0; i < numFiles; i++) {
				if(md5) {
					checkFileMD5(DIR.getPath(i), md5[i], true);
				} else {
					checkFileMD5(DIR.getPath(i), "", false);
				}
				string filename = DIR.getPath(i);
				ofLog(OF_LOG_VERBOSE, "   loading " + filename);
				resources.push_back(T());
				loadResource(filename);
			}
		}
		
		virtual void loadResource(string filename) {
		}
		
		
		T &getCurrentFast() {
			return resources[currentIndex];
		}
		
		
		T &getCurrent() {
			if(resources.size() > 0) {
				currentIndex = (currentIndex + resources.size()) % resources.size();
				return resources[currentIndex];
			} else {
				printf("*** No resources yet ***\n");
			}
		}
		
		T &getRandom() {
			currentIndex = rand() % resources.size();
			return getCurrent();
		}
		
		T &getNext() {
			currentIndex++;
			return getCurrent();
		}
		
		T &getPrev() {
			currentIndex--;
			return getCurrent();
		}
		
		T &getAt(int i) {
			currentIndex = i;
			return getCurrent();
		}
		
		int size() {
			return resources.size();
		}
		
	protected:
		vector<T>		resources;
		int				currentIndex;
	};
	
	
	
	//---------------------------
	class ImageDirManager : public DirManager<Image> {
	public:
		void loadResource(string filename) {
			resources.back().loadImage(filename);
		}
	};
	
	//---------------------------
	class SoundDirManager : public DirManager<ofSoundPlayer*> {
	public:
		void loadResource(string filename) {
			ofSoundPlayer *s = resources.back() = new ofSoundPlayer();
			s->loadSound(filename);
		}
	};
	
	class MovieDirManager : public DirManager<ofVideoPlayer*> {
	public:
		void loadResource(string filename) {
			ofVideoPlayer *s = resources.back() = new ofVideoPlayer();
			s->loadMovie(filename);
		}
	};
	
}
