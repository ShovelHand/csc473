

#pragma once
#include "Mass.h"
#include <atlas/utils/Geometry.hpp>

class AngularSpring : public atlas::utils::Geometry
{
public:
	AngularSpring();
	~AngularSpring();

	void renderGeometry(atlas::math::Matrix4 projection,
		atlas::math::Matrix4 view) override;

	void updateGeometry(atlas::utils::Time const& t) override;

	void SetConstant(float newk){ m_fk = newk; }

	void EulerIntegrator(atlas::utils::Time const& t);

private:
	GLuint mVao;
	GLuint mBuffer;

	//parameters that will influence the simulation
	float m_fk;  //spring constant
	Mass mass;

	atlas::math::Vector mRefPosition;
	atlas::math::Vector mModelVelocity;
	atlas::math::Vector mModelPosition;
	atlas::math::Vector mModelOldPosition;
	atlas::math::Vector mForce;

	atlas::math::Matrix4 mRefMatrix;
};
