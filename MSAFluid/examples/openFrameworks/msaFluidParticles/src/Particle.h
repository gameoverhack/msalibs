/*
 *  Particle.h
 *  ofxMSAFluid Demo
 *
 *  Created by Mehmet Akten on 02/05/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#pragma once

#include "MSACore.h"
#include "MSAFluidSolver.h"

using namespace MSA;

class Particle {
public:	
    Vec2f	pos, vel;
    float	radius;
    float	alpha;
    float	mass;
	
    void init(float x, float y);
    void update( const FluidSolver &solver, const Vec2f &windowSize, const Vec2f &invWindowSize );
	void updateVertexArrays( bool drawingFluid, const Vec2f &invWindowSize, int i, float* posBuffer, float* colBuffer);
};

