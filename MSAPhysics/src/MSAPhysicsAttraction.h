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
#include "MSAPhysicsConstraint.h"


namespace MSA {
	
	namespace Physics {
		
		template <typename T>
		class AttractionT : public ConstraintT<T> {
		public:
			friend class WorldT<T>;
			
			AttractionT(ParticleT<T> *a, ParticleT<T> *b, float strength) {
				this->_a	= a;
				this->_b	= b;
				this->_type = kConstraintTypeAttraction;
				this->setClassName("AttractionT");

				setStrength(strength);
			}
			
			void setStrength(float s);
			float getStrength();
			
		protected:
			float _strength;
			
			void solve() {
				T delta = this->_b->getPosition() - this->_a->getPosition();
				float deltaLength2 = delta.lengthSquared();
				
				float force = _strength * (this->_b->getMass()) * (this->_a->getMass()) / (deltaLength2 + 0.001);		// to avoid divide by zero
				
				T deltaForce = delta * force;
				
				if (this->_a->isFree()) this->_a->moveBy(deltaForce * this->_a->getInvMass(), false);
				if (this->_b->isFree()) this->_b->moveBy(deltaForce * -this->_b->getInvMass(), false);
			}
			
			void debugDraw() {
				ConstraintT<T>::debugDraw();
			}
		};
		
		template <typename T>
		inline void AttractionT<T>::setStrength(float s) {
			_strength = s;
		}
		
		template <typename T>
		inline float AttractionT<T>::getStrength() {
			return _strength;
		}
		
	}
}