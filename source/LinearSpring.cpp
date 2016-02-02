#include "LinearSpring.h"
#pragma once
#include "ShaderPaths.hpp"
#include <atlas/core/Macros.hpp>

#include <atlas/core/Float.hpp>
#include <atlas/core/Log.hpp>
#include <math.h>

/*ctor*/
LinearSpring::LinearSpring() :
m_fk(1.0),
m_fRestLength(1.0f), //spring's resting length
m_fLength(1.0f), //spring's current length given forces
m_fDamping(0.1f),
mRefPosition(1, 1, 0),
mModelVelocity(0),
mSpringScalar(1.0, 1.0, 1.0),
mModelPosition(.5, -0.1, 0),
mModelOldPosition(1.0, 1.0, 0)
{
	USING_ATLAS_GL_NS;
	USING_ATLAS_MATH_NS;

	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	//original position
	mRefMatrix = glm::translate(Matrix4(1.0f), mRefPosition);
	mModel = glm::translate(Matrix4(1.0f), mModelPosition);

	// Get the path where our shaders are stored.
	std::string shaderDir = generated::ShaderPaths::getShaderDirectory();

	// Now set up the information for our shaders.
	std::vector<ShaderInfo> shaders
	{
		ShaderInfo{ GL_VERTEX_SHADER, shaderDir + "linearSpring.vs.glsl" },
		ShaderInfo{ GL_FRAGMENT_SHADER, shaderDir + "linearSpring.fs.glsl" }
	};

	// Create a new shader and add it to our list.
	mShaders.push_back(ShaderPointer(new Shader));

	// Compile the shaders.
	mShaders[0]->compileShaders(shaders);

	// And link them.
	mShaders[0]->linkShaders();

	GLuint mMat = mShaders[0]->getUniformVariable("Mat");
	mUniforms.insert(UniformKey("Mat", mMat));


	GLfloat vertices[] =
	{
		-0.7f, 1.0f,
		-0.9f, 1.0f,
		-0.7f, 0.9f,
		-0.9f, 0.8f,
		-0.7f, 0.7f,
		-0.9f, 0.6f,
		-0.7f, 0.5f,
		-0.9f, 0.4f,
		-0.7f, 0.3f,
		-0.9f, 0.2f,
		-0.7f, 0.1f,
		-0.9f, 0.0f
		- 0.8f, 0.0f
	};
	glGenBuffers(1, &mBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// Disable at the end to avoid mixing shaders.
	mShaders[0]->disableShaders();

	mass.SetPos(Vector(-0.3, -0.1,0.0)*mModelPosition); //make mass appear to hang from end of spring

}

/*dtor*/
LinearSpring::~LinearSpring()
{
	glDeleteVertexArrays(1, &mVao);
	glDeleteBuffers(1, &mBuffer);
}

void LinearSpring::updateGeometry(atlas::utils::Time const& t)
{
	USING_ATLAS_MATH_NS;

	//CODE GOES HERE
	EulerIntegrator(t);
	mModel = glm::scale(Matrix4(1.0f), mSpringScalar);
}

void LinearSpring::renderGeometry()
{
	// To avoid warnings from unused variables, you can use the 
	// UNUSED macro.

	// Enable the shaders.
	mShaders[0]->enableShaders();

	glBindVertexArray(mVao);
	
	auto mMat =  mModel;
	glUniformMatrix4fv(mUniforms["Mat"], 1, GL_FALSE, &mMat[0][0]);
	//---draw
	glDrawArrays(GL_LINE_STRIP, 0, 11);

	// Disable them.
	mShaders[0]->disableShaders();

	//draw the weight at the end of the spring
	mass.renderGeometry();
}

void LinearSpring::EulerIntegrator(atlas::utils::Time const& t)
{
	float LoadForce = mass.GetMass() * 9.81f;
	float SpringForce = m_fk * (m_fLength - m_fRestLength) - m_fDamping;
	mSpringScalar.y -= ((LoadForce - SpringForce) + t.deltaTime)/100.f;
}