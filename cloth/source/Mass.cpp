#include "Mass.h"
#pragma once
#include "ShaderPaths.hpp"
#include <atlas/core/Macros.hpp>

/*ctor*/
Mass::Mass() :
m_fmass(2.0),  //TODO: should have gui functionality to make this user set
mModelPosition(-10, 0, 0)
{
	USING_ATLAS_GL_NS;

	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	GLfloat vertices[] =
	{//hexagon shape
		-0.1f, 0.0f,
		0.0f, 0.0f,
		0.1f,-0.1f,
		0.0f, -0.2f,
		-0.1f, -0.2f,
		-0.2f,-0.1f
		
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

void Mass::updateGeometry(float q, atlas::math::Vector refMatrix)
{
	USING_ATLAS_GL_NS;
	USING_ATLAS_MATH_NS;
	//mModel = refMatrix;
	mModel *= glm::rotate(Matrix4(1.0f), q, refMatrix);
}

void Mass::renderGeometry()
{

	// Enable the shaders.
	mShaders[0]->enableShaders();

	glBindVertexArray(mVao);

	//load transformations
	auto mMat = mModel;
	glUniformMatrix4fv(mUniforms["Mat"], 1, GL_FALSE, &mMat[0][0]);

	glDrawArrays(GL_LINE_LOOP, 0, 6);

	// Disable them.
	mShaders[0]->disableShaders();
}

void Mass::SetPos(atlas::math::Vector xyz)
{
	USING_ATLAS_GL_NS;
	USING_ATLAS_MATH_NS;

	mModelPosition.x = xyz.x; mModelPosition.y = xyz.y ;
	mModel = glm::translate(Matrix4(1.0f), mModelPosition) * glm::scale(Matrix4(1.0f), Vector(m_fmass/2.0f, m_fmass/2.0f,0));
}