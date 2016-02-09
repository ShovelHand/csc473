

#pragma once
#include "Mass.h"
#include <atlas/utils/Geometry.hpp>

class AngularSpring : public atlas::utils::Geometry
{
public:
	AngularSpring();
	~AngularSpring();

	void renderGeometry();

	void updateGeometry(atlas::utils::Time const& t) override;

	void SetConstant(float newk){ m_fk = newk; }

	void EulerIntegrator(atlas::utils::Time const& t);

private:
	GLuint mVao;
	GLuint mBuffer;

	//parameters that will influence the simulation
	float m_fk;  //spring constant
	Mass mass;
	float m_fRestLength;
	float m_fLength;
	float m_fDamping;

	atlas::math::Vector mRefPosition;
	atlas::math::Vector mModelVelocity;
	atlas::math::Vector mModelPosition;
	atlas::math::Vector mModelOldPosition;
	atlas::math::Vector mForce;
	atlas::math::Vector mRotate;

	atlas::math::Matrix4 mRefMatrix;
};
