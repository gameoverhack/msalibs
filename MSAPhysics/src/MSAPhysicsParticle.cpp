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


#include "MSAPhysicsParticle.h"
#include "MSAPhysics.h"


namespace MSA {
	
	namespace Physics {
		
		Particle::Particle() {
			init(Vec3f());
		}
		
		
		Particle::Particle(Vec3f pos, float m, float d) {
			init(pos, m, d);
		}
		
		Particle::Particle(Particle &p) {
			init(p.getPosition(), p._mass, p._drag);
			_isFixed = p._isFixed;
			setBounce(p._bounce);
			setRadius(p._radius);
		}
		
		
		
		void Particle::init(Vec3f pos, float m, float d) {
			_params = NULL;
			_world = NULL;
			
			_pos = _oldPos = pos;
			setMass(m);
			setDrag(d);
			setBounce();
			setRadius();
			enableCollision();
			makeFree();
			_isDead = false;
			_age = 0;
			verbose = true;
			data = NULL;
			setClassName("Particle");
		}
		
		Particle* Particle::enableCollision(){
			_collisionEnabled = true;
			return this;
		}
		
		Particle* Particle::disableCollision() {
			_collisionEnabled = false;
			return this;
		}
		
		bool Particle::hasCollision() {
			return _collisionEnabled;
		}
		
		
		
		
		void Particle::doVerlet() {
			if (!_isFixed) {
				if(_params->doGravity) {
					Vec3f gravityForce = _params->gravity;
					addVelocity(gravityForce);
				}
				
				Vec3f curPos = _pos;
				Vec3f vel = _pos - _oldPos;
				//		_pos += vel * _params->drag * _drag + _params->timeStep2;
				_pos += (_pos - _oldPos) + _params->timeStep2;
				_oldPos = curPos;
			}
		}
		
		
		
		void Particle::checkWorldEdges() {
			//	printf("%.3f, %.3f, %.3f\n", _params->worldMin.x, _params->worldMax.y, _params->worldMax.z);
			
			if(_pos.x < _params->worldMin.x + _radius) {
				float vel = _pos.x - _oldPos.x;
				_pos.x = _params->worldMin.x + _radius;
				_oldPos.x = _pos.x + vel * _bounce;
			} else if(_pos.x > _params->worldMax.x - _radius) {
				float vel = _pos.x - _oldPos.x;
				_pos.x = _params->worldMax.x - _radius;
				_oldPos.x = _pos.x + vel * _bounce;
			}
			
			if( _pos.y < _params->worldMin.y + _radius) {
				float vel = _pos.y - _oldPos.y;
				_pos.y = _params->worldMin.y + _radius;
				_oldPos.y = _pos.y + vel * _bounce;
			} else if(_pos.y > _params->worldMax.y - _radius) {
				float vel = _pos.y - _oldPos.y;
				_pos.y = _params->worldMax.y - _radius;
				_oldPos.y = _pos.y + vel * _bounce;
			}
			
			if(_pos.z < _params->worldMin.z + _radius) {
				float vel = _pos.z - _oldPos.z;
				_pos.z = _params->worldMin.z + _radius;
				_oldPos.z = _pos.z + vel * _bounce;
			} else if(_pos.z > _params->worldMax.z - _radius) {
				float vel = _pos.z - _oldPos.z;
				_pos.z = _params->worldMax.z - _radius;
				_oldPos.z = _pos.z + vel * _bounce;
			}
		}
		
		
		Params *Particle::getParams() {
			return _params;
		}
		
		
		void Particle::debugDraw() {
//			glPushMatrix();
//			glTranslatef(_pos.x, _pos.y, _pos.z);
//#ifndef TARGET_OS_IPHONE
//			glutSolidSphere(_radius, 12, 12);
//#else
//			ofCircle(0, 0, _radius);
//#endif
//			glPopMatrix();
		}
		
	}
}