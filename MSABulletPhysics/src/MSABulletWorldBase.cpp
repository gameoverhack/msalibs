/*
 *  BulletWorldBase.cpp
 *  BulletWorldBase demo
 *
 *  Created by Mehmet Akten on 21/06/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#include "MSABulletWorldBase.h"


namespace MSA {
	
	BulletWorldBase::BulletWorldBase() {
		bulletWorld				= NULL;
		solver					= NULL;
		broadphase				= NULL;
		dispatcher				= NULL;
		collisionConfiguration	= NULL;
	}
	
	BulletWorldBase::~BulletWorldBase() {
		destroy();
	}
	
	
	void BulletWorldBase::setup() {
		setup(Vec3f(-10000,-10000,-10000), Vec3f(10000, 10000, 10000));
	}
	
	void BulletWorldBase::setup(Vec3f worldMin, Vec3f worldMax) {
		if(!collisionConfiguration) collisionConfiguration = new btDefaultCollisionConfiguration();
		
		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		if(!dispatcher) dispatcher = new btCollisionDispatcher(collisionConfiguration);
		
		///the maximum size of the collision bulletWorld. Make sure objects stay within these boundaries
		///Don't make the bulletWorld AABB size too large, it will harm simulation quality and performance
		if(!broadphase) {
			btVector3 worldAabbMin = Vec3f_To_btVector3(worldMin);
			btVector3 worldAabbMax = Vec3f_To_btVector3(worldMax);
			int	maxProxies = 1024;
			broadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies);
		}
		
		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		if(!solver) solver = new btSequentialImpulseConstraintSolver;
		if(!bulletWorld) bulletWorld = createBulletWorld();
		
		setGravity();
	}
	
	void BulletWorldBase::destroy() {
		//cleanup in the reverse order of creation/initialization	
		
		//remove the rigidbodies from the dynamics bulletWorld and delete them
		for (int i=bulletWorld->getNumCollisionObjects()-1; i>=0 ;i--) {
			btCollisionObject* obj = bulletWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState()) {
				delete body->getMotionState();
			}
			bulletWorld->removeCollisionObject( obj );
			delete obj;
		}	
		
		//delete objects
		for (int i=0; i<objects.size(); i++) {
			BulletRigidBody* o = objects[i];
			objects[i] = 0;
			delete o;		// this line results in "non-page-aligned, non-allocated pointer being freed". not sure why
		}
		objects.clear();
		
		DelPointer(bulletWorld);
		DelPointer(solver);
		DelPointer(broadphase);
		DelPointer(dispatcher);
		DelPointer(collisionConfiguration);
	}
	
	//
	//
	//BulletObjectBox *createBox(Vec3f &position) {
	//	BulletObjectBox *box = new BulletObjectBox;
	//}
	
}

