/*
 *  BulletPhysicsUtils.h
 *  BulletPhysics demo
 *
 *  Created by Mehmet Akten on 21/06/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#pragma once

#include "MSACore.h"
#include "btBulletDynamicsCommon.h"

namespace MSA {
	
	inline btVector3 Vec3f_To_btVector3(Vec3f &p) {
		return btVector3(p.x, p.y, p.z);
	}
	
	
	inline Vec3f btVector3_To_Vec3f(btVector3 &p) {
		return Vec3f(p.m_floats[0], p.m_floats[1], p.m_floats[2]);
	}
	
}