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
#include "MSAObjCPointer.h"
#include "MSAPhysicsParams.h"


namespace MSA {
	
	namespace Physics {
		
		class World;
		
		class Particle : public ObjCPointer {
		public:
			friend class World;
			
			Particle();
			Particle(Vec3f pos, float m = 1.0f, float d = 1.0f);
			Particle(Particle &p);
			
			virtual void	init(Vec3f pos, float m = 1.0f, float d = 1.0f);
			
			Particle* setMass(float t = 1);
			float			getMass();
			float			getInvMass();
			
			Particle* setDrag(float t = 1);
			float			getDrag();
			
			Particle* setBounce(float t = 1);
			float			getBounce();
			
			Particle* setRadius(float t = 15);
			float			getRadius();
			
			// collision methods
			Particle* enableCollision();
			Particle* disableCollision();
			bool			hasCollision();
			
			bool			isFixed();
			bool			isFree();
			Particle* makeFixed();
			Particle* makeFree();
			
			// move the particle
			// if preserveVelocity == true, the particle will move to new position and keep it's old velocity
			// if preserveVelocity == false, the particle will move to new position but gain the velocity of the displacement
			Particle* moveTo(Vec3f targetPos, bool preserveVelocity = true);
			Particle* moveBy(Vec3f offset, bool preserveVelocity = true);
			
			float			getX();
			float			getY();
			float			getZ();
			Vec3f			getPosition();
			
			Particle* setVelocity(Vec3f vel);
			Particle* addVelocity(Vec3f vel);
			Vec3f			getVelocity();
			
			// override these functions if you create your own particle type with custom behaviour and/or drawing
			virtual void	update() {}
			virtual void	draw() {}
			
			void			kill();
			bool			isDead();
			
			// custom void* which you can use to store any kind of custom data
			void			*data;
			
			Params *getParams();
			
		protected:
			Params			*_params;
			World			*_world;
			
			Vec3f			_pos;
			Vec3f			_oldPos;
			float			_mass, _invMass;
			float			_drag;
			float			_bounce;
			float			_radius;
			float			_age;
			bool			_isDead;
			bool			_isFixed;
			bool			_collisionEnabled;
			
			void			doVerlet();
			void			checkWorldEdges();
			
			virtual void debugDraw();
		};
		
		
		
		inline Particle* Particle::setMass(float t) {
			if(t==0) t=0.00001f;
			_mass = t;
			_invMass = 1.0f/t;
			return this;
		}
		
		inline float Particle::getMass() {
			return _mass;
		}
		
		inline float Particle::getInvMass() {
			return _invMass;
		}
		
		
		inline Particle* Particle::setDrag(float t) {
			_drag = t;
			return this;
		}
		
		inline float Particle::getDrag() {
			return _drag;
		}
		
		inline Particle* Particle::setBounce(float t) {
			_bounce = t;
			return this;
		}
		
		inline float Particle::getBounce() {
			return _bounce;
		}
		
		
		inline Particle* Particle::setRadius(float t) {
			_radius = t;
			return this;
		}
		
		inline float Particle::getRadius() {
			return _radius;
		}
		
		inline bool Particle::isFixed() {
			return (_isFixed == true);
		}
		
		inline bool Particle::isFree() {
			return (_isFixed == false);
		}
		
		inline Particle* Particle::makeFixed() {
			_isFixed = true;
			return this;
		}
		
		inline Particle* Particle::makeFree() {
			_oldPos = _pos;
			_isFixed = false;
			return this;
		}
		
		inline Particle* Particle::moveTo(Vec3f targetPos, bool preserveVelocity) {
			Vec3f diff = targetPos - _pos;
			moveBy(diff, preserveVelocity);
			return this;
		}
		
		inline Particle* Particle::moveBy(Vec3f offset, bool preserveVelocity) {
			_pos += offset;
			if(preserveVelocity) _oldPos += offset;
			return this;
		}
		
		inline float Particle::getX() {
			return _pos.x;
		}
		
		inline float Particle::getY() {
			return _pos.y;
		}
		
		inline float Particle::getZ() {
			return _pos.z;
		}
		
		inline Vec3f Particle::getPosition() {
			return _pos;
		}
		
		inline Particle* Particle::setVelocity(Vec3f vel) {
			_oldPos = _pos - vel;
			return this;
		}
		
		inline Particle* Particle::addVelocity(Vec3f vel) {
			_oldPos -= vel;
			return this;
		}
		
		inline Vec3f Particle::getVelocity() {
			return _pos - _oldPos;
		}
		
		inline void Particle::kill() {
			_isDead = true;
		}
		
		
		inline bool Particle::isDead() {
			return _isDead;
		}
		
	}
}
