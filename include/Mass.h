/*this class defines the mass object that is attached to the end of the spring*/

#pragma once
#include <atlas/utils/Geometry.hpp>

class Mass : public atlas::utils::Geometry
{
public:
	Mass();
	~Mass();

	void renderGeometry(atlas::math::Matrix4 projection,
		atlas::math::Matrix4 view) override;

	void SetMass(float newmass){ m_fmass = newmass; }
	void SetPos(float x, float y);  //set the mass object to x-y coords of window.
	float GetMass() { return m_fmass; }

private:
	GLuint mVao;
	GLuint mBuffer;

	//parameters that will influence the simulation
	float m_fmass; //the mass on the end of the string;
	float xPos; float yPos;
};