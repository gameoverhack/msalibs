/*
 *  BulletObject.h
 *  BulletPhysics demo
 *
 *  Created by Mehmet Akten on 21/06/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

/* TODO
 add ownership for collision shape, so it can delete or not
 */

#include "ofTypes.h"
#include "btBulletDynamicsCommon.h"

namespace MSA {
	
	class BulletRigidBody {
	public:
		BulletRigidBody();
		virtual	~BulletRigidBody();
		
		// initialize the rigidbody with the given collisionShape and startPosition
		// ownsCollisionShape is for advanced (and internal use) only
		void					setup(Vec3f &startPosition, btCollisionShape *collisionShape, bool ownsCollisionShape, float mass = 1);
		btCollisionShape		*getCollisionShape();	
		btRigidBody				*getBulletBody();
		
		//	void					setMass(float m = 1);
		//	float					getMass();	
		
		bool					isFixed();
		bool					isFree();
		void					makeFixed();
		void					makeFree();	
		
	protected:
		//	float					mass;
		bool					ownsCollisionShape;		// whether the object owns (and can delete) the collision shape
		btRigidBody				*bulletBody;
		btCollisionShape		*collisionShape;
	};
	
	
	inline btCollisionShape		*BulletRigidBody::getCollisionShape() {
		return collisionShape;
	}
	
	inline btRigidBody			*BulletRigidBody::getBulletBody() {
		return bulletBody;
	}
	//
	//inline void					BulletRigidBody::setMass(float m) {
	//	mass = m;
	//}
	//
	//inline float				BulletRigidBody::getMass() {
	//	return mass;
	//}
	//
	
	inline bool					BulletRigidBody::isFixed() {
		//	return (mass == 0);
	}
	
	inline bool					BulletRigidBody::isFree() {
	}
	
	inline void					BulletRigidBody::makeFixed() {
	}
	
	inline void					BulletRigidBody::makeFree() {
	}
}