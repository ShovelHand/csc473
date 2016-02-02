#include "AngularSpring.h"
#pragma once
#include "ShaderPaths.hpp"
#include <atlas/core/Macros.hpp>

#include <atlas/core/Float.hpp>
#include <atlas/core/Log.hpp>

/*ctor*/
AngularSpring::AngularSpring() :
m_fk(1.0), //TODO: make spring constant gui settable by user.
mRefPosition(6, 1, 0),
mModelVelocity(0),
mModelPosition(-6, 1, 0),
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

	//CODE GOES HERE
	EulerIntegrator(t);
	mModel = glm::translate(Matrix4(1.0f), mModelPosition);
}

void AngularSpring::renderGeometry(atlas::math::Matrix4 projection,
	atlas::math::Matrix4 view)
{
	// To avoid warnings from unused variables, you can use the 
	// UNUSED macro.
	UNUSED(projection);
	UNUSED(view);

	// Enable the shaders.
	mShaders[0]->enableShaders();

	glBindVertexArray(mVao);
	glDrawArrays(GL_LINE_STRIP, 0, 11);

	// Disable them.
	mShaders[0]->disableShaders();

	//draw the weight at the end of the spring
	mass.renderGeometry(projection, view);
}

void AngularSpring::EulerIntegrator(atlas::utils::Time const& t)
{
	mModelPosition += cos(t.deltaTime);
}