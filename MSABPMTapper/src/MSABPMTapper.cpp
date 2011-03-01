/*
 *  MSABPMManager.cpp
 *  UK Summer Games v2
 *
 *  Created by Memo Akten on 24/08/2010.
 *  Copyright 2010 MSA Visuals Ltd. All rights reserved.
 *
 */

#include "MSABPMTapper.h"

namespace MSA {
	BPMTapper::BPMTapper():_tapCount(0), _bpm(60), _lengthOfBeat(1), _beatTime(0) {startFresh();}
	
#define kBeatLerpSeconds	1		// it takes 5 seconds to fully lerp to beat (to smoothen transition)
	
	void BPMTapper::startFresh() {
		printf("BPM::startFresh\n");
		_tapCount		= 1;
		_timer.start();
		
//		float iBeatTime;
//		float fBeatTime;
//		fBeatTime = modf(_beatTime, &fBeatTime);
//		_beatTime = (int)iBeatTime % 16 + fBeatTime;
	}
	
	void BPMTapper::update() {
		float beatStartTime = _timer.getSeconds();

		_beatTime			= beatStartTime / _lengthOfBeat;
		_beatPerc			= _beatTime - static_cast<int>(_beatTime);
		
		float lerpSpeed;
		if(beatStartTime < kBeatLerpSeconds) {
			lerpSpeed = ofNormalize(beatStartTime, 0, kBeatLerpSeconds);
			printf("lerpSpeed: %f\n", lerpSpeed);
		} else {
			lerpSpeed = 1;
		}
		
		sin1	+= (beatSinS(1) - sin1) * lerpSpeed;
		sin2	+= (beatSinS(2) - sin2) * lerpSpeed;
		sin4	+= (beatSinS(4) - sin4) * lerpSpeed;
		sin8	+= (beatSinS(8) - sin8) * lerpSpeed;
		sin16	+= (beatSinS(16) - sin16) * lerpSpeed;
		
		cos1	+= (beatCosS(1) - cos1) * lerpSpeed;
		cos2	+= (beatCosS(2) - cos2) * lerpSpeed;
		cos4	+= (beatCosS(4) - cos4) * lerpSpeed;
		cos8	+= (beatCosS(8) - cos8) * lerpSpeed;
		cos16	+= (beatCosS(16) - cos16) * lerpSpeed;
	}
	
	
	void BPMTapper::tap() {
		float timeSinceLastCall = _timer.getSecondsSinceLastCall();
		
		if(timeSinceLastCall > 2) {
			startFresh();
			
		} else {
			_lengthOfBeat = _timer.getSeconds() / _tapCount;
			_bpm = 60.0f / _lengthOfBeat;
			_tapCount++;
		}
		
		printf("BPM %i %f\n", _tapCount, _bpm);
		
	}
	
	float BPMTapper::bpm() {
		return _bpm;
	}
	
	
	float BPMTapper::beatTime() {
		return _beatTime;
	}
	
	
	float BPMTapper::beatPerc() {
		return _beatPerc;
	}
	
	float BPMTapper::beatSinU(float beats) {
		return 0.5 + 0.5 * beatSinS(beats);
	}
	
	float BPMTapper::beatSinS(float beats) {
		return sin(beatTime() * TWO_PI/beats);
	}
	
	float BPMTapper::beatCosU(float beats) {
		return 0.5 + 0.5 * beatCosS(beats);
	}
	
	float BPMTapper::beatCosS(float beats) {
		return cos(beatTime() * TWO_PI/beats);
	}
	
	
	
	
	
	
	void BPMTapper::draw(int x, int y, int r, ofImage &image) {
		int d = r * 2;
		glColor3f(1, 0, 0);
		ofEnableAlphaBlending();
		image.draw(x, y, d * sin1, d * sin1);		x += d;
		image.draw(x, y, d * sin2, d * sin2);		x += d;
		image.draw(x, y, d * sin4, d * sin4);		x += d;
		image.draw(x, y, d * sin8, d * sin8);		x += d;
		image.draw(x, y, d * sin16, d * sin16);	x += d;
	}

}