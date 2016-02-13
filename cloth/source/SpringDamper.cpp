
#include "SpringDamper.h"

#pragma once
#include <math.h>

/*Ctor*/
SpringDamper::SpringDamper(atlas::math::Vector &p1, atlas::math::Vector &p2, float springConstant, float dampFactor, float restLength) :
mP1(p1),
mP2(p2),
m_fK(springConstant),
m_fDamp(dampFactor),
m_fRestLength(restLength)
{

}

/*Dtor*/
SpringDamper::~SpringDamper()
{

}

atlas::math::Vector SpringDamper::computeForce()
{
	USING_ATLAS_GL_NS;
	USING_ATLAS_MATH_NS;

	//get current length between vertices
	float curLength = sqrtf(pow(mP1.x - mP2.x, 2) + pow(mP1.y - mP2.y, 2) + pow(mP1.z - mP2.z, 2));
	Vector dirVec = (mP2 - mP1);
	dirVec /= dirVec.length(); //unit vector between two vertices
	float forceScalar = (m_fK * (curLength - m_fRestLength) * m_fDamp);
	
	return dirVec * forceScalar;
}