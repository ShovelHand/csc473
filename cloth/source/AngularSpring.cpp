#include "AngularSpring.h"
#pragma once
#include "ShaderPaths.hpp"
#include <atlas/core/Macros.hpp>

#include <atlas/core/Float.hpp>
#include <atlas/core/Log.hpp>

/*ctor*/
AngularSpring::AngularSpring() :
m_fk(1.0), //TODO: make spring constant gui settable by user.
m_fRestLength(2.0f), //spring's resting length
m_fLength(5.0f), //spring's current length given forces
m_fDamping(0.1f),
mRefPosition(6, 1, 0),
mRotate(0,0,1),
mModelVelocity(0),
mModelPosition(0.3, 0.0, 0),
mModelOldPosition(-6, 1, 0)
{
	USING_ATLAS_GL_NS;
	USING_ATLAS_MATH_NS;

	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	//original position
	mRefMatrix = glm::translate(Matrix4(1.0f), mRefPosition);
	mModel = glm::translate(Matrix4(1.0f), mModelPosition);

	GLfloat vertices[] =
	{
		0.0,0.0,  //just two points!
		0.0, 0.50
	};

//	mass.SetPos(Vector(-0.8f, 0.0f,0.0f));
	glGenBuffers(1, &mBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Get the path where our shaders are stored.
	std::string shaderDir = generated::ShaderPaths::getShaderDirectory();

	// Now set up the information for our shaders.
	std::vector<ShaderInfo> shaders
	{
		ShaderInfo{ GL_VERTEX_SHADER, shaderDir + "AngularSpring.vs.glsl" },
		ShaderInfo{ GL_FRAGMENT_SHADER, shaderDir + "AngularSpring.fs.glsl" }
	};

	// Create a new shader and add it to our list.
	mShaders.push_back(ShaderPointer(new Shader));

	// Compile the shaders.
	mShaders[0]->compileShaders(shaders);

	// And link them.
	mShaders[0]->linkShaders();

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// Disable at the end to avoid mixing shaders.
	mShaders[0]->disableShaders();
	mass.SetMass(2.0f);

}

/*dtor*/
AngularSpring::~AngularSpring()
{
	glDeleteVertexArrays(1, &mVao);
	glDeleteBuffers(1, &mBuffer);
}

void AngularSpring::updateGeometry(atlas::utils::Time const& t)
{
	USING_ATLAS_MATH_NS;
	USING_ATLAS_GL_NS;

	//CODE GOES HERE
	EulerIntegrator(t);
	mModel = glm::translate(Matrix4(1.0f), mModelPosition);
	mModel *= glm::rotate(Matrix4(1.0f), mForce.x, mRotate);
	Vector placeAtEnd(0.35, 0.7, 0);
	mass.SetPos(placeAtEnd); //put it at the end of the rod, and rotate it
	mass.updateGeometry(m_fLength, -mRotate);
}

void AngularSpring::renderGeometry()
{
	// Enable the shaders.
	mShaders[0]->enableShaders();

	glBindVertexArray(mVao);

	//load transformations
	auto mMat = mModel;
	glUniformMatrix4fv(mUniforms["Mat"], 1, GL_FALSE, &mMat[0][0]);

	glLineWidth((GLfloat)2.0);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
	glLineWidth((GLfloat)1.0);

	// Disable them.
	mShaders[0]->disableShaders();

	//draw the weight at the end of the spring
	mass.renderGeometry();
}

void AngularSpring::EulerIntegrator(atlas::utils::Time const& t)
{
	float LoadForce = mass.GetMass() * (9.81f)*t.deltaTime;
	float SpringForce = (m_fk * (m_fLength - m_fRestLength) - m_fDamping)*t.deltaTime;
	m_fLength = (SpringForce-LoadForce)*(t.deltaTime / 10.0f);
	mForce = atlas::math::Vector(LoadForce - SpringForce, LoadForce - SpringForce, LoadForce - SpringForce)*t.deltaTime;

}