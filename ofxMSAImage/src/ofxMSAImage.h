/*
 *  ofxMSAImage.h
 *  UK Summer Games v2
 *
 *  Created by Memo Akten on 27/08/2010.
 *  Copyright 2010 MSA Visuals Ltd. All rights reserved.
 *
 */

#pragma once


#include "MSACore.h"
#include "ofMain.h"

namespace MSA {
	class Image : public ofImage {
	public:
		ofPixels& getMyPixels() {
			return myPixels;
		}
	};
}