/***********************************************************************
 
 * this is a class for solving real-time fluid dynamics simulations based on Navier-Stokes equations 
 * and code from Jos Stam's paper "Real-Time Fluid Dynamics for Games" http://www.dgp.toronto.edu/people/stam/reality/Research/pdf/GDC03.pdf
 * Other useful resources and implementations I looked at while building this lib: 
 * Mike Ash (C) - http://mikeash.com/?page=pyblog/fluid-simulation-for-dummies.html
 * Alexander McKenzie (Java) - http://www.multires.caltech.edu/teaching/demos/java/stablefluids.htm
 * Pierluigi Pesenti (AS3 port of Alexander's) - http://blog.oaxoa.com/2008/01/21/actionscript-3-fluids-simulation/
 * Gustav Taxen (C) - http://www.nada.kth.se/~gustavt/fluids/
 * Dave Wallin (C++) - http://nuigroup.com/touchlib/ (uses portions from Gustav's)
 
 
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
//	April-Mai 2009 optimized and extended by Maa (http://www.lagraine.com/ - new content coming soon)

/* Portions Copyright (c) 2010, The Cinder Project, http://libcinder.org */

#pragma once

#include "MSACore.h"


// do not change these values, you can override them using the solver methods
#define		FLUID_DEFAULT_NX					100
#define		FLUID_DEFAULT_NY					100
#define     FLUID_DEFAULT_DT					0.04f	//Maa	25fps
#define		FLUID_DEFAULT_VISC					0.0001f
#define     FLUID_DEFAULT_COLOR_DIFFUSION		0.0f
#define     FLUID_DEFAULT_FADESPEED				0.03f
#define		FLUID_DEFAULT_SOLVER_ITERATIONS		10

#define		FLUID_IX(i, j)		((i) + (_NX + 2)  *(j))

namespace MSA {
	
	class FluidSolver {
	public:	
		FluidSolver();
		virtual ~FluidSolver();
		
		FluidSolver& setup(int NX = FLUID_DEFAULT_NX, int NY = FLUID_DEFAULT_NY);
		FluidSolver& setSize(int NX = FLUID_DEFAULT_NX, int NY = FLUID_DEFAULT_NY);
		
		// solve one step of the fluid solver
		void update();
		
		// clear all forces in fluid and reset
		void reset();	
		
		// get fluid cell index for cell coordinates or normalized position
		inline int getIndexForCell(int i, int j) const;
		inline int getIndexForPos(const Vec2f &pos) const;
		
		
		// get color and/or vel at any point in the fluid.
		// pass pointers to Vec2f (for velocity) and Color (for color) and they get filled with the info
		// leave any pointer NULL if you don't want that info
		inline void getInfoAtIndex(int index, Vec2f *vel, Color *color = NULL) const;
		inline void getInfoAtCell(int i, int j, Vec2f *vel, Color *color = NULL) const;
		inline void getInfoAtPos(const Vec2f &pos, Vec2f *vel, Color *color = NULL) const;
		
		
		// get just velocity
		inline Vec2f getVelocityAtIndex(int index) const;
		inline Vec2f getVelocityAtCell(int i, int j) const;
		inline Vec2f getVelocityAtPos(const Vec2f &pos) const;
		
		
		// get just color
		inline Color getColorAtIndex(int index) const;
		inline Color getColorAtCell(int i, int j) const;
		inline Color getColorAtPos(const Vec2f &pos) const;
		
		
		// add force (at cell index, cell coordinates, or normalized position)
		inline void addForceAtIndex(int index, const Vec2f &force);
		inline void addForceAtCell(int i, int j, const Vec2f &force);
		inline void addForceAtPos(const Vec2f &pos, const Vec2f &force);
		
		
		// add color (at cell index, cell coordinates, or normalized position)
		inline void addColorAtIndex(int index, const Color &color);
		inline void addColorAtCell(int i, int j, const Color &color);
		inline void addColorAtPos(const Vec2f &pos, const Color &color);
		
		
		// fill with random color at every cell
		void randomizeColor();
		
		// return number of cells and dimensions
		int getNumCells() const;
		int getWidth() const;
		int getHeight() const;
		float getInvWidth() const;
		float getInvHeight() const;
		Vec2f getSize();
		Vec2f getInvSize();
		
		bool isInited() const;
		
		// accessors for  viscocity, it will lerp to the target at lerpspeed
		FluidSolver& setVisc(float newVisc); 
		float getVisc() const;
		
		// accessors for  color diffusion
		// if diff == 0, color diffusion is not performed
		// ** COLOR DIFFUSION IS SLOW!
		FluidSolver& setColorDiffusion( float diff );
		float				getColorDiffusion();
		
		FluidSolver& enableRGB(bool isRGB);
		FluidSolver& setDeltaT(float deltaT = FLUID_DEFAULT_DT);
		FluidSolver& setFadeSpeed(float fadeSpeed = FLUID_DEFAULT_FADESPEED);
		FluidSolver& setSolverIterations(int solverIterations = FLUID_DEFAULT_SOLVER_ITERATIONS);
		FluidSolver& enableVorticityConfinement(bool b);
		bool getVorticityConfinement();
		FluidSolver& setWrap( bool bx, bool by );
		
		// returns average density of fluid 
		float getAvgDensity() const;
		
		// returns average _uniformity
		float getUniformity() const;
		
		// returns average speed of fluid
		float getAvgSpeed() const;
		
		// allocate an array large enough to hold information for u, v, r, g, OR b
		float* alloc()	{ return new float[_numCells];	}
		
		
		float	*density, *densityOld;		// used if not RGB
		Vec3f	*color, *colorOld;			// used for RGB
		Vec2f	*uv, *uvOld;
		
		float	*curl;
		
		bool	doRGB;				// for monochrome, update only density
		bool	doVorticityConfinement;
		int		solverIterations;
		
		float	colorDiffusion;
		float	viscocity;
		float	fadeSpeed;
		float	deltaT;
		bool	wrap_x;
		bool	wrap_y;
		
	protected:
		
		float width;
		float height;
		float invWidth;
		float invHeight;
		
		int		_NX, _NY, _numCells;
		float	_invNX, _invNY, _invNumCells;
		bool	_isInited;
		float	*_tmp;
		
		float	_avgDensity;			// this will hold the average color of the last frame (how full it is)
		float	_uniformity;			// this will hold the _uniformity of the last frame (how uniform the color is);
		float	_avgSpeed;
		
		void	destroy();
		
		inline	float	calcCurl(int i, int j);
		void	vorticityConfinement(Vec2f *Fvc_xy);
		
		template<typename T>
		void	addSource(T *x, T *x0);
		
		void	advect(int b, float *d, const float *d0, const Vec2f *duv);
		void	advect2d( Vec2f *uv, const Vec2f *duv );
		void	advectRGB(int b, const Vec2f *duv);
		
		void	diffuse(int b, float *c, float *c0, float diff);
		void	diffuseRGB(int b, float diff);
		void	diffuseUV(float diff);
		
		void	project(Vec2f *xy, Vec2f *pDiv);
		void	linearSolver(int b, float *x, const float *x0, float a, float c);
		void	linearSolverProject( Vec2f *pdiv );
		void	linearSolverRGB( float a, float c);
		void	linearSolverUV(float a, float c);
		
		void	setBoundary(int b, float *x);
		void	setBoundary02d(Vec2f* x);
		void	setBoundary2d(int b, Vec2f *xy );
		void	setBoundaryRGB();
		
		void	fadeDensity();
		void	fadeRGB();
	};
	
	
	//-------- get index
	inline int FluidSolver::getIndexForCell(int i, int j) const {
		i = constrain(i, 1, _NX);
		j = constrain(j, 1, _NY);
		return FLUID_IX(i, j);
	}
	
	inline int FluidSolver::getIndexForPos(const Vec2f &pos) const {
		return getIndexForCell((int)floor(pos.x * width), (int)floor(pos.y * height));
	}
	
	
	
	//-------- get info
	inline	void FluidSolver::getInfoAtIndex(int index, Vec2f *vel, Color *color) const {
		if(vel) *vel = getVelocityAtIndex(index);
		if(color) *color = getColorAtIndex(index);
	}
	
	inline void FluidSolver::getInfoAtCell(int i, int j, Vec2f *vel, Color *color) const {
		getInfoAtIndex(getIndexForCell(i, j), vel, color);
	}
	
	
	inline void FluidSolver::getInfoAtPos(const Vec2f &pos, Vec2f *vel, Color *color) const {
		getInfoAtIndex(getIndexForPos(pos), vel, color);
	}
	
	
	//-------- get velocity
	inline Vec2f FluidSolver::getVelocityAtIndex(int index) const {
		return uv[index];	
	}
	
	inline Vec2f FluidSolver::getVelocityAtCell(int i, int j) const {
		return getVelocityAtIndex(getIndexForCell(i, j));	
	}
	
	inline Vec2f FluidSolver::getVelocityAtPos(const Vec2f &pos) const {
		return getVelocityAtIndex(getIndexForPos(pos));	
	}
	
	
	//-------- get color
	inline Color FluidSolver::getColorAtIndex(int index) const {
		if(doRGB) {
			return Color(this->color[index].x, this->color[index].y, this->color[index].z);
		} else {
			return Color(density[index], density[index], density[index]);
		}
	}
	
	inline Color FluidSolver::getColorAtCell(int i, int j) const {
		return getColorAtIndex(getIndexForCell(i, j));	
	}
	
	inline Color FluidSolver::getColorAtPos(const Vec2f &pos) const {
		return getColorAtIndex(getIndexForPos(pos));
	}
	
	
	//-------- add force
	inline void FluidSolver::addForceAtIndex(int index, const Vec2f &force) {
		uv[index] += force;
	}
	
	inline void FluidSolver::addForceAtCell(int i, int j, const Vec2f &force) {
		addForceAtIndex(getIndexForCell(i, j), force);
	}
	
	inline void FluidSolver::addForceAtPos(const Vec2f &pos, const Vec2f &force) {
		addForceAtIndex(getIndexForPos(pos), force);
	}
	
	
	//-------- add color
	inline void FluidSolver::addColorAtIndex(int index, const Color &color) {
		if(doRGB) {
			colorOld[index] += Vec3f(color.r, color.g, color.b);
		} else {
			density[index] += color.r;
		}
	}
	
	inline void FluidSolver::addColorAtCell(int i, int j, const Color &color) {
		addColorAtIndex(getIndexForCell(i, j), color);
	}
	
	inline void FluidSolver::addColorAtPos(const Vec2f &pos, const Color &color) {
		addColorAtIndex(getIndexForPos(pos), color);
	}
	
	template<typename T>
	void FluidSolver::addSource(T *x, T *x0) {
		for(int i = _numCells-1; i >=0; --i) {
			x[i] += x0[i] * deltaT;
		}
	}

}