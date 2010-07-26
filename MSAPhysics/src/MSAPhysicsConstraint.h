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
#include "MSAPhysicsParticle.h"


namespace MSA {
	
	namespace Physics {
		
		typedef enum ConstraintType {
			kConstraintTypeCustom,
			kConstraintTypeSpring,
			kConstraintTypeAttraction,
			kConstraintTypeCount,
		} ConstraintType;
		
		
		
		template <typename T>
		class ConstraintT : public ObjCPointer {
		public:
			friend class WorldT<T>;
			
			ConstraintT() {
				_isOn	= true;
				_type	= kConstraintTypeCustom;
				_isDead = false;
				verbose = true;
				_params = NULL;
				
				setMinDistance(0);
				setMaxDistance(0);
				
				setClassName("ConstraintT");
			}
			
			virtual ~ConstraintT() {
				_a->release();
				_b->release();
			}
			
			int type();
			
			// getOneEnd is a same as getA and getTheOtherEnd is same as getB
			// just have both methods so you can choose whichever you please
			ParticleT<T>* getOneEnd();
			ParticleT<T>* getTheOtherEnd();			
			
			ParticleT<T>* getA();
			ParticleT<T>* getB();
			
			void turnOff();
			void turnOn();
			
			bool isOn();
			bool isOff();
			
			void kill();
			bool isDead();
			
			// set minimum distance before constraint takes affect
			void setMinDistance(float d);
			
			// get minimum distance
			float getMinDistance();
			
			// set maximum distance before constraint takes affect
			void setMaxDistance(float d);
			
			// get maximum distance
			float getMaxDistance();
			
			
			// only worth solving the constraint if its on, and at least one end is free
			bool shouldSolve();
			
			virtual void update() {}
			virtual void draw() {}
			
			
			
		protected:
			ConstraintType	_type;
			bool			_isOn;
			bool			_isDead;
			float			_minDist;
			float			_minDist2;
			float			_maxDist;
			float			_maxDist2;
			
			ParticleT<T>	*_a, *_b;
			ParamsT<T>		*_params;
			virtual void solve() = 0;
			
			virtual void debugDraw() {
				//ofLine(_a->x, _a->y, _b->x, _b->y);
				/*
				 T vec = (*_b - *_a);
				 float dist = msaLength(vec);
				 float angle = acos( vec.z / dist ) * RAD_TO_DEG;
				 if(vec.z <= 0 ) angle = -angle;
				 float rx = -vec.y * vec.z;
				 float ry =  vec.x * vec.z;
				 
				 glPushMatrix();
				 glTranslatef(_a->x, _a->y, _a->z);
				 glRotatef(angle, rx, ry, 0.0);
				 glScalef(1, 1, dist);
				 glTranslatef(0, 0, 0.5);
				 #ifndef TARGET_OF_IPHONE
				 glutSolidCube(1);
				 #endif
				 glPopMatrix();
				 */
			}
		};
		
		
		
		template <typename T>
		inline int ConstraintT<T>::type() {
			return _type;
		}
		
		template <typename T>
		inline ParticleT<T>* ConstraintT<T>::getOneEnd() {
			return _a;
		}
		
		template <typename T>
		inline ParticleT<T>* ConstraintT<T>::getTheOtherEnd() {
			return _b;
		}
		
		template <typename T>
		inline ParticleT<T>* ConstraintT<T>::getA() {
			return _a;
		}
		
		template <typename T>
		inline ParticleT<T>* ConstraintT<T>::getB() {
			return _b;
		}
		
		template <typename T>
		inline void ConstraintT<T>::turnOff() {
			_isOn = false;
		}
		
		template <typename T>
		inline void ConstraintT<T>::turnOn() {
			_isOn = true;
		}
		
		template <typename T>
		inline bool ConstraintT<T>::isOn() {
			return (_isOn == true);
		}
		
		template <typename T>
		inline bool ConstraintT<T>::isOff(){
			return (_isOn == false);
		}
		
		template <typename T>
		inline void ConstraintT<T>::kill() {
			_isDead = true;
		}
		
		template <typename T>
		inline bool ConstraintT<T>::isDead() {
			return _isDead;
		}
		
		
		template <typename T>
		inline void ConstraintT<T>::setMinDistance(float d) {
			_minDist = d;
			_minDist2 = d*d;
		}
		
		template <typename T>
		inline float ConstraintT<T>::getMinDistance() {
			return _minDist;
		}
		
		template <typename T>
		inline void ConstraintT<T>::setMaxDistance(float d) {
			_maxDist = d;
			_maxDist2 = d*d;
		}
		
		template <typename T>
		inline float ConstraintT<T>::getMaxDistance() {
			return _maxDist;
		}
		
		// only worth solving the constraint if its on, and at least one end is free
		template <typename T>
		inline bool ConstraintT<T>::shouldSolve() {
			
			// if the constraint is off or both sides are fixed then return false
			if(isOff() || (_a->isFixed() && _b->isFixed())) return false;	
			
			// if no length restrictions then return true (by this point we know above condition is false)
			if(_minDist == 0 && _maxDist == 0) return true;
			
			T delta = _b->getPosition() - _a->getPosition();
			float deltaLength2 = delta.lengthSquared();
			
			bool minDistSatisfied;
			if(_minDist) minDistSatisfied = deltaLength2 > _minDist2;
			else minDistSatisfied = true;
			
			bool maxDistSatisfied;
			if(_maxDist) maxDistSatisfied = deltaLength2 < _maxDist2;
			else maxDistSatisfied = true;
			
			return minDistSatisfied && maxDistSatisfied;
		}
		
	}
}
