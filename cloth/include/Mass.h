/*this class defines the mass object that is attached to the end of the spring*/

#pragma once
#include <atlas/utils/Geometry.hpp>
#include <atlas/core/Macros.hpp>
#include <atlas/core/Float.hpp>
#include <atlas/core/Log.hpp>

class Mass : public atlas::utils::Geometry
{
public:
	Mass();
	~Mass();

	void renderGeometry();
	void updateGeometry(float q, atlas::math::Vector refMatrix);

	void SetMass(float newmass){ m_fmass = newmass; }
	void SetPos(atlas::math::Vector xyz);  //set the mass object to x-y coords of window.
	float GetMass() { return m_fmass; }

private:
	GLuint mVao;
	GLuint mBuffer;

	atlas::math::Matrix4 mRefMatrix;
	atlas::math::Vector mModelPosition;
	//parameters that will influence the simulation
	float m_fmass; //the mass on the end of the string;
	float xPos; float yPos;
};