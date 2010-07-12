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

#include "MSAPhysics.h"


namespace MSA {
	
	namespace Physics {
		
		class World : public ParticleUpdatable {
			
		public:
			friend class Particle;
			
			bool				verbose;
			
			World();
			~World();
			
			Particle*		makeParticle(Vec3f pos, float m = 1.0f, float d = 1.0f);
			Spring*			makeSpring(Particle *a, Particle *b, float _strength, float _restLength);
			Attraction*		makeAttraction(Particle *a, Particle *b, float _strength);
			
			// this method retains the particle, so you should release() it after adding (obj-c style)
			Particle*		addParticle(Particle *p);
			
			// this method retains the constraint, so you should release it after adding (obj-c style)
			Constraint*		addConstraint(Constraint *c);
			
			Particle*		getParticle(uint i);
			Constraint*		getConstraint(uint i);			// generally you wouldn't use this but use the ones below
			Spring*			getSpring(uint i);
			Attraction*		getAttraction(uint i);
			
			uint			numberOfParticles();
			uint			numberOfConstraints();		// all constraints: springs, attractions and user created
			uint			numberOfSprings();			// only springs
			uint			numberOfAttractions();		// only attractions
			
			World*			setDrag(float drag = 0.99);					// set the drag. 1: no drag at all, 0.9: quite a lot of drag, 0: particles can't even move
			World*			setGravity(float gy = 0);					// set gravity (y component only)
			World*			setGravity(Vec3f g);						// set gravity (full vector)
			Vec3f&			getGravity();
			World*			setTimeStep(float timeStep);
			World*			setNumIterations(float numIterations = 20);	// default value
			
			// for optimized collision, set world dimensions first
			World*			setWorldMin(Vec3f worldMin);
			World*			setWorldMax(Vec3f worldMax);
			World*			setWorldSize(Vec3f worldMin, Vec3f worldMax);
			World*			clearWorldSize();
			
			// and then set sector size (or count)
			World*			enableCollision();
			World*			disableCollision();
			bool			isCollisionEnabled();
			World*			setSectorCount(int count);		// set the number of sectors (will be equal in each axis)
			World*			setSectorCount(Vec3f vCount);	// set the number of sectors in each axis
			
			// preallocate buffers if you know how big they need to be (they grow automatically if need be)
			World*			setParticleCount(uint i);
			World*			setConstraintCount(uint i);
			World*			setSpringCount(uint i);
			World*			setAttractionCount(uint i);
			
			
			void clear();
			void update(int frameNum = -1);
			void draw();
			void debugDraw();
			
#ifdef MSAPHYSICS_USE_RECORDER
			World*			setReplayMode(int i, float playbackScaler = 1.0f);		// when playing back recorded data, optionally scale positions up (so you can record in lores, playback at highres)
			World*			setReplayFilename(string f);
#endif
			
		protected:
			vector<Particle*>	_particles;
			vector<Constraint*>	_constraints[kConstraintTypeCount];
			vector<Sector>		_sectors;
			
			Params				params;
			
			void				updateParticles();
			void				updateConstraints();
			void				updateConstraintsByType(vector<Constraint*> constraints);
			//	void						updateWorldSize();
			
			
			void				checkAllCollisions();
			
			Constraint*			getConstraint(Particle *a, int constraintType);
			Constraint*			getConstraint(Particle *a, Particle *b, int constraintType);
			
			
#ifdef MSAPHYSICS_USE_RECORDER
			DataRecorder<Vec3f>	_recorder;
			uint				_frameCounter;
			uint				_replayMode;
			float				_playbackScaler;
			void load(uint frameNum);
#endif
		};
		
	}
}