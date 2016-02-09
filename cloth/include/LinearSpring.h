

#pragma once
#include "Mass.h"
#include <atlas/utils/Geometry.hpp>

class LinearSpring : public atlas::utils::Geometry
{
public:
	LinearSpring();
	~LinearSpring();

	void renderGeometry();

	void updateGeometry(atlas::utils::Time const& t) override;

	void SetConstant(float newk){ m_fk = newk; }

	void EulerIntegrator(atlas::utils::Time const& t);

private:
	GLuint mVao;
	GLuint mBuffer;

	//parameters that will influence the simulation
	float m_fk;  //spring constant
	float m_fRestLength;
	float m_fLength;
	float m_fDamping;
	Mass mass;
	
	atlas::math::Vector mRefPosition;
	atlas::math::Vector mModelVelocity;
	atlas::math::Vector mModelPosition;
	atlas::math::Vector mModelOldPosition;
	atlas::math::Vector mSpringScalar;
	atlas::math::Vector mForce;

	atlas::math::Matrix4 mRefMatrix;
};
