/*
 *  MSABPMManager.h
 *  UK Summer Games v2
 *
 *  Created by Memo Akten on 24/08/2010.
 *  Copyright 2010 MSA Visuals Ltd. All rights reserved.
 *
 */

#pragma once

#include "MSATimer.h"


namespace MSA {
	class BPMTapper {
	public:
		BPMTapper();
		
		// these are precalculated, and interpolated for when BPM changes
		float sin1;
		float sin2;
		float sin4;
		float sin8;
		float sin16;
		
		float cos1;
		float cos2;
		float cos4;
		float cos8;
		float cos16;
		
		
		// call this to tap
		void tap();
		
		// call this to cache all variables
		void update();
		
		// call this to start calculating fresh
		void startFresh();
		
		// returns number of beats per minute
		float bpm();
		
		// returns beat time since last tap
		// integer is the beat number
		// remainder is percentage where we are in the beat
		float beatTime();
		
		// 0...1 percentage where we are in the beat
		float beatPerc();
		
		
		// 0...1 or -1..1 smooth interpolation based on beat
		// loops over 'beats' beats
		float beatSinU(float beats);
		float beatSinS(float beats);
		float beatCosU(float beats);
		float beatCosS(float beats);
		
		
		void draw(int x, int y, int r, ofImage &image);
		
	protected:
		int		_tapCount;		// number of taps
		float	_bpm;			// beats per minute
		float	_lengthOfBeat;	// length of one beat in seconds
		
		float	_beatPerc;
		float	_beatTime;
		
		Timer	_timer;
		
	};
}