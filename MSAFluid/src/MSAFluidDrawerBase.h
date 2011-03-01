/***********************************************************************
 
 This class is for drawing a fluidsolver using the OpenFrameworks texture
 
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

#include "MSACore.h"
#include "MSAFluidSolver.h"

namespace MSA {
	
	typedef enum {
		kFluidDrawColor,
		kFluidDrawMotion,
		kFluidDrawSpeed,
		kFluidDrawVectors,
		kFluidDrawCount
	} FluidDrawMode;
	
	
	class FluidDrawerBase 
#ifdef MSA_HOST_OPENFRAMEWORKS
	: public ofBaseDraws
#endif
	{
	public:
		bool	enabled;
		bool	doInvert;
		bool	useAdditiveBlending;
		float	brightness;
		float	velDrawThreshold;
		float	velDrawMult;
		int		vectorSkipCount;
		
		FluidDrawMode		drawMode;

		FluidDrawerBase();
		virtual ~FluidDrawerBase();
		
		FluidSolver* setup(int NX = FLUID_DEFAULT_NX, int NY = FLUID_DEFAULT_NY);
		FluidSolver* setup(FluidSolver* f);
		FluidSolver* getFluidSolver();
		
		void enableAlpha(bool b);
		
		void update();
		
		void draw(float x = 0, float y = 0);
		void draw(float x, float y, float renderWidth, float renderHeight);				// this one does chooses one of the below based on drawmode
		void drawColor(float x, float y, float renderWidth, float renderHeight, bool withAlpha = false);
		void drawMotion(float x, float y, float renderWidth, float renderHeight, bool withAlpha = false);
		void drawSpeed(float x, float y, float renderWidth, float renderHeight, bool withAlpha = false);
		void drawVectors(float x, float y, float renderWidth, float renderHeight);
		void reset();
		
		virtual float getWidth() = 0;	
		virtual float getHeight() = 0;
		
		void setDrawMode(FluidDrawMode newDrawMode);
		void incDrawMode();
		void decDrawMode();
		FluidDrawMode getDrawMode();
		string getDrawModeName();
		
		const static string	drawOptionTitles[];
		
	protected:	
		unsigned char		*_pixels;						// pixels array to be drawn
		
		int					_glType;						// GL_RGB or GL_RGBA
		bool				_alphaEnabled;
		int					_bpp;							// 3 or 4
		
		FluidSolver			*_fluidSolver;
		bool				_didICreateTheFluid;
		
		void				allocatePixels();
		
		virtual void		createTexture() = 0;									// override to create a texture
		virtual void		updateTexture() = 0;									// override to update the texture from the pixels array
		virtual void		deleteTexture() = 0;									// override to delete the texture
		virtual void		drawTexture(float x, float y, float w, float h) = 0;	// override to draw texture
		
		void				deleteFluidSolver();
		bool				isFluidReady();
		
	};
	
}
