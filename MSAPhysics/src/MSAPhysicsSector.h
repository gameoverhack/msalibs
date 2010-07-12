/*
 *  Sector.h
 *  Physics demo
 *
 *  Created by Mehmet Akten on 09/06/2010.
 *  Copyright 2010 MSA Visuals Ltd. All rights reserved.
 *
 */

#pragma once

#include "MSAPhysicsParticle.h"

namespace MSA {
	
	namespace Physics {
		
		class Sector {
		public:
			void	checkSectorCollisions();
			
			void	addParticle(Particle *p) {
				_particles.push_back(p);
			}
			
			void	clear() {
				_particles.clear();
			}
			
			//	void	checkParticle(Particle *p);
			
		protected:
			bool	checkCollisionBetween(Particle *a, Particle* b);
			vector<Particle*>	_particles;
		};
		
	}
}