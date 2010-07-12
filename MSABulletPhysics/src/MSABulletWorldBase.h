/*
 *  BulletWorldBase.h
 *  BulletWorldBase demo
 *
 *  Created by Mehmet Akten on 21/06/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */


#pragma once

#include "MSACore.h"
#include "btBulletDynamicsCommon.h"

#include "MSABulletPhysicsUtils.h"
#include "MSABulletRigidBody.h"


namespace MSA {
	
	class BulletWorldBase {
	public:
		BulletWorldBase();
		virtual ~BulletWorldBase();
		
		void setup();
		void setup(Vec3f worldMin, Vec3f worldMax);
		void destroy();
		
		void setGravity(Vec3f &p);
		void setGravity(btScalar gx = 0, btScalar gy = 9.8, btScalar gz = 0);
		
		
		// creates a rigid body and adds to the world (and returns pointer to newly created object)
		// object will be released automatically when world is destroyed
		BulletRigidBody *createRigidBox(Vec3f &startPosition, Vec3f &size, float mass = 1);
		BulletRigidBody *createRigidSphere(Vec3f &startPosition, float radius, float mass = 1);
		
		// create a rigid body out of given collisionShape and add to the world
		// will be released automatically when world is destroyed
		// if you want to create a rigid body with an existing collision shape use this method
		// (e.g. if you want to use the collisionShape from a previously created box, sphere etc.)
		// (reusing collisionShapes is very much advised if possible)
		// ownsCollisionShape:	if the body owns (and should delete) the collision shape upon destruction set this true
		//						if the body is sharing this collision shape set to false
		BulletRigidBody *createRigidBody(Vec3f &startPosition, btCollisionShape *collisionShape, bool ownsCollisionShape, float mass = 1);
		
		// add an existing rigid body to the world
		// will be released automatically when world is destroyed
		BulletRigidBody *addRigidBody(BulletRigidBody *body);
		
		// add an existing bullet rigid body to the world (the original bullet class)
		// FOR ADVANCED USE: will NOT be released automatically when world is destroyed
		void addBulletRigidBody(btRigidBody *bulletBody);	
		
		
		
		void update();
		void debugDraw();
		
		// return reference to the bullet world
		// FOR ADVANCED USE
		btDynamicsWorld &getBulletWorld();
		
		
		// FOR ADVANCED USE
		// not making these protected for now so you can create your own versions of these before calling setup()
		btDynamicsWorld								*bulletWorld;	
		btCollisionConfiguration					*collisionConfiguration;
		btCollisionDispatcher						*dispatcher;
		btBroadphaseInterface						*broadphase;
		btConstraintSolver							*solver;
		btAlignedObjectArray<BulletRigidBody*>	objects;
		
	protected:
		virtual btDynamicsWorld* createBulletWorld() = 0;
	};
	
	
	inline BulletRigidBody *BulletWorldBase::addRigidBody(BulletRigidBody *body) {
		objects.push_back(body);
		addBulletRigidBody(body->getBulletBody());
		return body;
	}
	
	
	inline 	BulletRigidBody *BulletWorldBase::createRigidBody(Vec3f &startPosition, btCollisionShape *collisionShape, bool ownsCollisionShape, float mass) {
		BulletRigidBody *body = new BulletRigidBody();
		body->setup(startPosition, collisionShape, ownsCollisionShape, mass);
		addRigidBody(body);
		
		return body;
	}
	
	
	inline BulletRigidBody *BulletWorldBase::createRigidSphere(Vec3f &startPosition, float radius, float mass) {
		btCollisionShape *collisionShape = new btSphereShape(btScalar(radius));
		return createRigidBody(startPosition, collisionShape, true, mass); // body will own this collision shape
	}	
	
	
	inline void BulletWorldBase::addBulletRigidBody(btRigidBody *bulletBody) {
		bulletWorld->addRigidBody(bulletBody);
	}
	
	
	inline 	btDynamicsWorld& BulletWorldBase::getBulletWorld() {
		return *bulletWorld;
	}
	
	inline void BulletWorldBase::setGravity(Vec3f &p) {
		bulletWorld->setGravity(Vec3f_To_btVector3(p));
	}
	
	inline void BulletWorldBase::setGravity(btScalar gx, btScalar gy, btScalar gz) {
		bulletWorld->setGravity(btVector3(gx, gy, gz));
	}
	
	
	
	inline void BulletWorldBase::update() {
		bulletWorld->stepSimulation(1.0f/60.0f, 10);
	}
	
	inline void BulletWorldBase::debugDraw() {
		bulletWorld->debugDrawWorld();
	}
}
