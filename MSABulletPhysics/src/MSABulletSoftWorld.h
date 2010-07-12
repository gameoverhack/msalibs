/*
 *  BulletRigidWorld.h
 *  BulletPhysics demo
 *
 *  Created by Mehmet Akten on 21/06/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

// container for soft body and rigid body objects

#pragma once

#include "MSABulletWorldBase.h"

namespace MSA {
	
	class BulletSoftWorld : public BulletWorldBase {
	public:
		
		
	protected:
		btDynamicsWorld* createBulletWorld() {
			//return new btSoftRigidDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
		}
	};
}