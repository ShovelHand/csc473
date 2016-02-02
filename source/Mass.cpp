#include "Mass.h"
#pragma once
#include "ShaderPaths.hpp"
#include <atlas/core/Macros.hpp>

/*ctor*/
Mass::Mass() :
m_fmass(1.0)  //TODO: should have gui functionality to make this user set
{
	USING_ATLAS_GL_NS;

	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	GLfloat vertices[] =  //TODO: figure out how to handle vertices.
	{//hexagon shape
		-0.5f, 0.0f,
		0.0f, 0.0f,
		0.2f,-0.1f,
		0.0f, -0.2f,
		-0.5f, -0.2f,
		-0.7f,-0.1f
		
	};

	glGenBuffers(1, &mBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Get the path where our shaders are stored.
	std::string shaderDir = generated::ShaderPaths::getShaderDirectory();

	// Now set up the information for our shaders.
	std::vector<ShaderInfo> shaders
	{
		ShaderInfo{ GL_VERTEX_SHADER, shaderDir + "mass.vs.glsl" },  //not sure if sharing shaders is bad news or not.
		ShaderInfo{ GL_FRAGMENT_SHADER, shaderDir + "mass.fs.glsl" }
	};

	// Create a new shader and add it to our list.
	mShaders.push_back(ShaderPointer(new Shader));
	// Compile the shaders.
	mShaders[0]->compileShaders(shaders);
	// And link them.
	mShaders[0]->linkShaders();

	/*mUniforms.insert(UniformKey("mvpMat",
		mShaders[0]->getUniformVariable("mvpMat")));*/

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// Disable at the end to avoid mixing shaders.
	mShaders[0]->disableShaders();

}

/*dtor*/
Mass::~Mass()
{
	glDeleteVertexArrays(1, &mVao);
	glDeleteBuffers(1, &mBuffer);
}

void Mass::renderGeometry(atlas::math::Matrix4 projection,
	atlas::math::Matrix4 view)
{
	// To avoid warnings from unused variables, you can use the 
	// UNUSED macro.
	UNUSED(projection);
	UNUSED(view);

	// Enable the shaders.
	mShaders[0]->enableShaders();

	glBindVertexArray(mVao);
	glDrawArrays(GL_LINE_LOOP, 0, 6);

	// Disable them.
	mShaders[0]->disableShaders();
}
