/*
 *  BulletRigidBody.cpp
 *  BulletPhysics demo
 *
 *  Created by Mehmet Akten on 21/06/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#include "MSABulletRigidBody.h"
#include "MSABulletPhysicsUtils.h"

namespace MSA {
	
	BulletRigidBody::BulletRigidBody() {
		bulletBody			= NULL;
		collisionShape		= NULL;
	}
	
	BulletRigidBody::~BulletRigidBody() {
		DelPointer(bulletBody);					// object owns bulletBody, so delete when done
		if(ownsCollisionShape) DelPointer(collisionShape);
	}
	
	
	
	void BulletRigidBody::setup(Vec3f &startPosition, btCollisionShape *collisionShape, bool ownsCollisionShape, float mass) {
		this->ownsCollisionShape = ownsCollisionShape;
		this->collisionShape = collisionShape;
		
		btTransform startTransform;
		startTransform.setIdentity();
		bool isDynamic = (mass != 0.f);
		
		btVector3 localInertia(0,0,0);
		if (isDynamic) collisionShape->calculateLocalInertia(mass,localInertia);
		
		startTransform.setOrigin(Vec3f_To_btVector3(startPosition));	
		
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collisionShape, localInertia);
		bulletBody = new btRigidBody(rbInfo);
		//body->setContactProcessingThreshold(collisionShape->getContactBreakingThreshold());
		bulletBody->setActivationState(ISLAND_SLEEPING);
		
	}
}
