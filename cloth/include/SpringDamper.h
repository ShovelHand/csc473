//class for spring dampener between two vertices in the cloth mesh
#pragma once
#include <atlas/utils/Geometry.hpp>
#include "atlas/Atlas.hpp"

class SpringDamper
{

public:
	SpringDamper(atlas::math::Vector &p1, atlas::math::Vector &p2, float springConstant, float dampFactor, float restLength);
	~SpringDamper();

	atlas::math::Vector computeForce();

private:
	atlas::math::Vector &mP1, &mP2;
	float m_fK, m_fRestLength, m_fDamp, m_fCurLength;
};