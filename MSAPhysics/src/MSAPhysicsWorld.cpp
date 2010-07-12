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

#include "MSAPhysics.h"


namespace MSA {
	
	namespace Physics {
		
		World::World() {
			verbose = false;
			setTimeStep(0.000010);
			setDrag();
			setNumIterations();
			disableCollision();
			setGravity();
			clearWorldSize();
			setSectorCount(0);
			
#ifdef MSAPHYSICS_USE_RECORDER
			_frameCounter = 0;
			setReplayMode(OFX_MSA_DATA_IDLE);
			setReplayFilename("recordedData/physics/physics");
#endif
		}
		
		World::~World() {
			clear();
		}
		
		
		
		Particle* World::makeParticle(Vec3f pos, float m, float d) {
			Particle *p = new Particle(pos, m, d);
			addParticle(p);
			p->release();	// cos addParticle(p) retains it
			return p;
		}
		
		Spring* World::makeSpring(Particle *a, Particle *b, float _strength, float _restLength) {
			if(a==b) return NULL;
			Spring* c = new Spring(a, b, _strength, _restLength);
			addConstraint(c);
			c->release();	// cos addConstraint(c) retains it
			return c;
		}
		
		Attraction* World::makeAttraction(Particle *a, Particle *b, float _strength) {
			if(a==b) return NULL;
			Attraction* c = new Attraction(a, b, _strength);
			addConstraint(c);
			c->release();	// cos addConstraint(c) retains it
			return c;
		}
		
		
		
		Particle* World::addParticle(Particle *p) {
			p->verbose = verbose;
			_particles.push_back(p);
			p->setInstanceName(string("particle "));// + ofToString(_particles.size(), 0));
			p->_params = &params;
			p->_world = this;
			
#ifdef MSAPHYSICS_USE_RECORDER
			if(_replayMode == OFX_MSA_DATA_SAVE)
				_recorder.setSize(numberOfParticles());
#endif
			p->retain();
			return p;		// so you can configure the particle or use for creating constraints
		}
		
		Constraint* World::addConstraint(Constraint *c) {
			c->verbose = verbose;
			_constraints[c->type()].push_back(c);
			c->_params = &params;
			
			c->retain();
			(c->_a)->retain();
			(c->_b)->retain();
			
			switch(c->type()) {
				case kConstraintTypeCustom:
					c->setInstanceName(string("constraint "));// + ofToString(_constraints[kConstraintTypeCustom].size(), 0));
					break;
					
				case kConstraintTypeSpring:
					c->setInstanceName(string("spring "));// + ofToString(_constraints[kConstraintTypeSpring].size(), 0));
					break;
					
				case kConstraintTypeAttraction:
					c->setInstanceName(string("attraction "));// + ofToString(_constraints[kConstraintTypeAttraction].size(), 0));
					break;
			}
			
			return c;
		}
		
		
		Particle*		World::getParticle(uint i) {
			return i < numberOfParticles() ? _particles[i] : NULL;
		}
		
		Constraint*	World::getConstraint(uint i) {
			return i < numberOfConstraints() ? _constraints[kConstraintTypeCustom][i] : NULL;
		}
		
		Spring*		World::getSpring(uint i) {
			return i < numberOfSprings() ? (Spring*)_constraints[kConstraintTypeSpring][i] : NULL;
		}
		
		Attraction*	World::getAttraction(uint i) {
			return i < numberOfAttractions() ? (Attraction*)_constraints[kConstraintTypeAttraction][i] : NULL;
		}
		
		
		uint World::numberOfParticles() {
			return _particles.size();
		}
		
		uint World::numberOfConstraints() {
			return _constraints[kConstraintTypeCustom].size();
		}
		
		uint World::numberOfSprings() {
			return _constraints[kConstraintTypeSpring].size();
		}
		
		uint World::numberOfAttractions() {
			return _constraints[kConstraintTypeAttraction].size();
		}
		
		
		World*  World::setDrag(float drag) {
			params.drag = drag;
			return this;
		}
		
		World*  World::setGravity(float gy) {
			setGravity(Vec3f(0, gy, 0));
			return this;
		}
		
		World*  World::setGravity(Vec3f g) {
			params.gravity= g;
			params.doGravity = params.gravity.lengthSquared() > 0;
			return this;
		}
		
		Vec3f& World::getGravity() {
			return params.gravity;
		}
		
		World*  World::setTimeStep(float timeStep) {
			params.timeStep = timeStep;
			params.timeStep2 = timeStep * timeStep;
			return this;
		}
		
		World*  World::setNumIterations(float numIterations) {
			params.numIterations = numIterations;
			return this;
		}
		
		
		World* World::setWorldMin(Vec3f worldMin) {
			params.worldMin		= worldMin;
			params.worldSize	= params.worldMax - params.worldMin;
			params.doWorldEdges	= true;
			return this;
		}
		
		World* World::setWorldMax(Vec3f worldMax) {
			params.worldMax		= worldMax;
			params.worldSize	= params.worldMax - params.worldMin;
			params.doWorldEdges = true;
			return this;
		}
		
		World* World::setWorldSize(Vec3f worldMin, Vec3f worldMax) {
			setWorldMin(worldMin);
			setWorldMax(worldMax);
			return this;
		}
		
		World* World::clearWorldSize() {
			params.doWorldEdges = false;
			disableCollision();
			return this;
		}
		
		
		
		World* World::enableCollision() {
			params.isCollisionEnabled = true;
			return this;
		}
		
		World* World::disableCollision() {
			params.isCollisionEnabled = false;
			return this;
		}
		
		bool World::isCollisionEnabled() {
			return params.isCollisionEnabled;
		}
		
		
		
		World* World::setSectorCount(int count) {
			setSectorCount(Vec3f(count, count, count));
			return this;
		}
		
		World* World::setSectorCount(Vec3f vCount) {
			if(vCount.x <= 0) vCount.x = 1;
			if(vCount.y <= 0) vCount.y = 1;
			if(vCount.z <= 0) vCount.z = 1;
			
			params.sectorCount = vCount;
			
			//	params.sectorCount.x = 1 << (int)vPow.x;
			//	params.sectorCount.y = 1 << (int)vPow.y;
			//	params.sectorCount.z = 1 << (int)vPow.z;
			
			//	Vec3f sectorSize = params.worldSize / sectorCount;
			
			_sectors.clear();
			int numSectors = params.sectorCount.x * params.sectorCount.y * params.sectorCount.z;
			for(int i=0; i<numSectors; i++) {
				_sectors.push_back(Sector());
			}
			//	_sectors.reserve(params.sectorCount.x * params.sectorCount.y * params.sectorCount.z);
			
			return this;
		}
		
		
		
		
		
		World*  World::setParticleCount(uint i) {
			_particles.reserve(i);
#ifdef MSAPHYSICS_USE_RECORDER
			//	if(_replayMode == OFX_MSA_DATA_SAVE)
			_recorder.setSize(i);
#endif
			return this;
		}
		
		
		World* World::setConstraintCount(uint i){
			_constraints[kConstraintTypeCustom].reserve(i);
			return this;
		}
		
		World* World::setSpringCount(uint i){
			_constraints[kConstraintTypeSpring].reserve(i);
			return this;
		}
		
		World* World::setAttractionCount(uint i){
			_constraints[kConstraintTypeAttraction].reserve(i);
			return this;
		}
		
		
		void World::clear() {
			for(vector<Particle*>::iterator it = _particles.begin(); it != _particles.end(); it++) {
				Particle* particle = *it;
				particle->release();
			}
			_particles.clear();
			
			
			for(int i=0; i<kConstraintTypeCount; i++) {
				for (vector<Constraint*>::iterator it = _constraints[i].begin(); it != _constraints[i].end(); it++) {
					Constraint* constraint = *it;
					constraint->release();
				}
				_constraints[i].clear();
			}
		}
		
		
		
		
		
		void World::update(int frameNum) {
#ifdef MSAPHYSICS_USE_RECORDER
			if(frameNum < 0) frameNum = _frameCounter;
			if(_replayMode == OFX_MSA_DATA_LOAD) {
				load(frameNum);
			} else {
				updateParticles();
				updateConstraints();
				if(isCollisionEnabled()) checkAllCollisions();
				if(_replayMode == OFX_MSA_DATA_SAVE) _recorder.save(frameNum);
			}
			_frameCounter++;
#else
			updateParticles();
			updateConstraints();
			if(isCollisionEnabled()) checkAllCollisions();
#endif
		}
		
		
		void World::draw() {
			for(int i=0; i<kConstraintTypeCount; i++) {
				for (vector<Constraint*>::iterator it = _constraints[i].begin(); it != _constraints[i].end(); it++) {
					Constraint* constraint = *it;
					constraint->draw();
				}
			}
			
			for(vector<Particle*>::iterator it = _particles.begin(); it != _particles.end(); it++) {
				Particle* particle = *it;
				particle->draw();
			}
		}
		
		void World::debugDraw() {
			for(int i=0; i<kConstraintTypeCount; i++) {
				for (vector<Constraint*>::iterator it = _constraints[i].begin(); it != _constraints[i].end(); it++) {
					Constraint* constraint = *it;
					constraint->debugDraw();
				}
			}
			
			for(vector<Particle*>::iterator it = _particles.begin(); it != _particles.end(); it++) {
				Particle* particle = *it;
				particle->debugDraw();
			}
		}
		
#ifdef MSAPHYSICS_USE_RECORDER
		void World::load(uint frameNum) {
			_recorder.load(frameNum);
			for(vector<Particle*>::iterator it = _particles.begin(); it != _particles.end(); it++) {
				Particle* particle = *it;
				particle->set(_recorder.get());// * _playbackScaler);
			}
		}
#endif
		
		void World::updateParticles() {
			int num = 0;
			vector<Particle*>::iterator it = _particles.begin();
			while(it != _particles.end()) {
				Particle* particle = *it;
				if(particle->_isDead) {							// if particle is dead
					it = _particles.erase(it);
					particle->release();
				} else {
					num++;
					particle->doVerlet();
					particle->update();
					applyUpdaters(particle);
					if(params.doWorldEdges) {
						//				if(particle->isFree()) 
						particle->checkWorldEdges();
					}
					
					// find which sector particle is in
					int i = mapRange(particle->getX(), params.worldMin.x, params.worldMax.x, 0.0f, params.sectorCount.x, true);
					int j = mapRange(particle->getY(), params.worldMin.y, params.worldMax.y, 0.0f, params.sectorCount.y, true);
					int k = mapRange(particle->getZ(), params.worldMin.z, params.worldMax.z, 0.0f, params.sectorCount.z, true);
					
					_sectors[i * params.sectorCount.y * params.sectorCount.x + j * params.sectorCount.x + k].addParticle(particle);
					
					//			printf("sector for particle at %f, %f, %f is %i %i %i\n", particle->getX(), particle->getY(), particle->getZ(), i, j, k);
					//			for(int s=0; s<_sectors.size(); s++) _sectors[s].checkParticle(particle);
					
#ifdef MSAPHYSICS_USE_RECORDER
					if(_replayMode == OFX_MSA_DATA_SAVE) _recorder.add(*particle);
#endif
					it++;
				}
			}
		}
		
		
		void World::updateConstraintsByType(vector<Constraint*> constraints) {
		}
		
		void World::updateConstraints() {
			// iterate all constraints and update
			for (int i = 0; i < params.numIterations; i++) {
				for(int i=0; i<kConstraintTypeCount; i++) {
					vector<Constraint*>::iterator it = _constraints[i].begin();
					while(it != _constraints[i].end()) {
						Constraint* constraint = *it;
						if(constraint->_isDead || constraint->_a->_isDead || constraint->_b->_isDead) {
							constraint->kill();
							it = _constraints[i].erase(it);
							constraint->release();
						} else {
							if(constraint->shouldSolve()) constraint->solve();
							it++;
						}
					}
					
				}
			}
		}
		
		
#ifdef MSAPHYSICS_USE_RECORDER
		World*  World::setReplayMode(uint i, float playbackScaler) {
			_replayMode = i;
			_playbackScaler = playbackScaler;
			//	if(_replayMode == OFX_MSA_DATA_SAVE)		// NEW
			_recorder.setSize(i);
			return this;
		}
		
		
		World*  World::setReplayFilename(string f) {
			_recorder.setFilename(f);
			return this;
		}
#endif
		
		
		void World::checkAllCollisions() {
			int s = _sectors.size();
			for(int i=0; i<s; i++) {
				_sectors[i].checkSectorCollisions();
				_sectors[i].clear();
			}
		}
		
		
		Constraint* World::getConstraint(Particle *a, Particle *b, int constraintType) {
			for (vector<Constraint*>::iterator it = _constraints[constraintType].begin(); it != _constraints[constraintType].end(); it++) {
				Constraint* s = *it;
				if(((s->_a == a && s->_b == b) || (s->_a == b && s->_b == a)) && !s->_isDead) {
					return s;
				}
			}
			return NULL;
		}
		
		
		Constraint* World::getConstraint(Particle *a, int constraintType) {
			for (vector<Constraint*>::iterator it = _constraints[constraintType].begin(); it != _constraints[constraintType].end(); it++) {
				Constraint* s = *it;
				if (((s->_a == a ) || (s->_b == a)) && !s->_isDead) {
					return s;
				}
			}
			return NULL;
		}
		
	}
}