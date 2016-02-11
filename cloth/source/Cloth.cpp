#include "Cloth.h"

#pragma once
#include "ShaderPaths.hpp"
#include <atlas/core/Macros.hpp>

#include <atlas/core/Float.hpp>
#include <atlas/core/Log.hpp>
#include <math.h>
#include <algorithm>
#include <iostream>

/*Ctor*/
Cloth::Cloth() :
mRefPosition(0, 0, 0),
mModelVelocity(0),
mModelPosition(-0.5f, -0.25f, 0),
mModelOldPosition(1.0, 1.0, 0)
{
	USING_ATLAS_GL_NS;
	USING_ATLAS_MATH_NS;

	//original position
//	mRefMatrix = glm::translate(Matrix4(1.0f), mRefPosition);
	mModel = glm::translate(Matrix4(1.0f), mModelPosition);
	mModel *= glm::scale(Matrix4(1.0f), Vector(0.1f,0.1f,0.1f));

	// Get the path where our shaders are stored.
	std::string shaderDir = generated::ShaderPaths::getShaderDirectory();

	// Now set up the information for our shaders.
	std::vector<ShaderInfo> shaders
	{
		ShaderInfo{ GL_VERTEX_SHADER, shaderDir + "cloth.vs.glsl" },
		ShaderInfo{ GL_FRAGMENT_SHADER, shaderDir + "cloth.fs.glsl" }
	};

	// Create a new shader and add it to our list.
	mShaders.push_back(ShaderPointer(new Shader));

	// Compile the shaders.
	mShaders[0]->compileShaders(shaders);

	// And link them.
	mShaders[0]->linkShaders();

	GLuint mMat = mShaders[0]->getUniformVariable("Mat");
	mUniforms.insert(UniformKey("Mat", mMat));

	//--vertex array
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	MakeVertices(12, 8);

	//--buffer
	glGenBuffers(1, &mBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(Vector))*triangle_vec.size(), &triangle_vec[0], GL_STATIC_DRAW);

	//using the commented out code below caused the geometry to be whacky, like not a grid
	GLuint vpoint_id = glGetAttribLocation(mShaders[0]->getShaderProgram(), "vpoint");
	glEnableVertexAttribArray(vpoint_id);
	glVertexAttribPointer(vpoint_id, 3, GL_FLOAT, false, 0, 0);

	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//glEnableVertexAttribArray(0);

	// Disable at the end to avoid mixing shaders.
	glBindVertexArray(0);
	mShaders[0]->disableShaders();
}

/*Dtor*/
Cloth::~Cloth()
{
	glDeleteVertexArrays(1, &mVao);
	glDeleteBuffers(1, &mBuffer);
}

void Cloth::updateGeometry(atlas::utils::Time const& t)
{
	USING_ATLAS_MATH_NS;
}

void Cloth::renderGeometry()
{
	// Enable the shaders.
	mShaders[0]->enableShaders();

	glBindVertexArray(mVao);
	
	auto mMat = mModel;
	glUniformMatrix4fv(mUniforms["Mat"], 1, GL_FALSE, &mMat[0][0]);
	//---draw
	glDrawArrays(GL_LINES, 0, triangle_vec.size());  //uncomment to see the mesh drawn as triangle strips

	// Disable them.
	mShaders[0]->disableShaders();
}

void Cloth::EulerIntegrator(atlas::utils::Time const& t)
{

}

void Cloth::MakeVertices(int width, int height)
{
	USING_ATLAS_MATH_NS;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			vertices.push_back(Vector(float(j), float(i), 0.0));
	}

	//triangle strip
	for (int j = 0; j < (height - 1); ++j)
	{
		//sqaures inside a triangle strip
		for (int i = 0; i < (width - 1); ++i)
		{
			int topleft = (j+1) * width + i;
			int topright = topleft + 1;
			int bottomleft = (j) * width + i;
			int bottomright = bottomleft + 1;
			//the upper left triangle
			triangle_vec.push_back(vertices[topleft]);
			triangle_vec.push_back(vertices[topright]);
			triangle_vec.push_back(vertices[bottomleft]);
			//the lower right triangle
			triangle_vec.push_back(vertices[topright]);
			triangle_vec.push_back(vertices[bottomright]);
			triangle_vec.push_back(vertices[bottomleft]);
		}
	}

	//make the pixel coordinates for the height map texture
	//for (std::vector<Vector>::iterator itr = triangle_vec.begin(); itr != triangle_vec.end(); ++itr)
	//	vtexcoord.push_back(Vector((*itr).x() / float(width), (*itr).z() / float(height),0.0f));

}