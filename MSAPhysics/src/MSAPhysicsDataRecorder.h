/***********************************************************************
 
 Copyright (c) 2008, 2009, 2010, Memo Akten, www.memo.tv
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

#define OF_ADDON_USING_OFXMSADATARECORDER

namespace MSA {
	
#define OFX_MSA_DATA_IDLE		0					// do nothing
#define OFX_MSA_DATA_SAVE		1					// save
#define OFX_MSA_DATA_LOAD		2					// load
	
	
	// always tries to read _numItems, no matter how big the file is (quicker to not reallocate the buffer when reading)
	template <typename Type> class DataRecorder {
	public:
		DataRecorder() {
			_buffer = 0;
			_curItem = 0;
		}
		
		virtual ~DataRecorder() {
			delete []_buffer;
		}
		
		
		void setSize(int n) {
			if(n<1) return;
			
			if(_buffer) delete []_buffer;
			_numItems = n;
			_buffer = new Type[_numItems];
			_curItem = 0;
		}
		
		void setFilename(string f) {
			_fileName = ofToDataPath(f);
		}
		
		void add(Type &t) {
			_buffer[_curItem++] = t;
		}
		
		Type &get() {
			return _buffer[_curItem++];
		}
		
		bool save(int i) {
			_curItem = 0;
			FILE *fileOut;											// output BIN file
			string fullFileName = _fileName + "_" + ofToString(i) + ".bin";
			fileOut = fopen(fullFileName.c_str(), "wb");					// open output bin file for writing
			if(fileOut == NULL) {
				printf("DataRecorder::save() - could not save %s\n", fullFileName.c_str());
				exit(1);
			}
			int numWritten = fwrite(_buffer, sizeof(Type), _numItems, fileOut);
			fclose(fileOut);
			return numWritten == _numItems;
		}
		
		bool load(int i) {
			_curItem = 0;
			FILE *fileIn;											// output BIN file
			string fullFileName = _fileName + "_" + ofToString(i) + ".bin";
			fileIn = fopen(fullFileName.c_str(), "rb");					// open output bin file for writing
			if(fileIn == NULL) {
				printf("DataRecorder::load() - could not load %s\n", fullFileName.c_str());
				exit(1);
			}
			
			int numRead = fread(_buffer, sizeof(Type), _numItems, fileIn);
			fclose(fileIn);
			return true;
		}
		
		
	protected:
		Type *_buffer;
		int _numItems;
		int _curItem;
		string _fileName;
	};
	
}