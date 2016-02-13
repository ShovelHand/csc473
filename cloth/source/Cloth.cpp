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
mModelOldPosition(1.0, 1.0, 0),
m_fVertexWeight(0.5f),
m_iClothWidth(12),
m_iClothHeight(8)
{
	USING_ATLAS_GL_NS;
	USING_ATLAS_MATH_NS;

	//original position
//	mRefMatrix = glm::translate(Matrix4(1.0f), mRefPosition);
	mModel = glm::translate(Matrix4(1.0f), mModelPosition);
//	mModel *= glm::scale(Matrix4(1.0f), Vector(0.1f,0.1f,0.1f));

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

	MakeVertices(m_iClothWidth, m_iClothHeight);

	//using the code below caused the geometry to be whacky, like not a grid. See BuildTriangleVec for how this was handled better.
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
	for (int i = 0; i < vertices.size(); ++i)
	{
		//compute force of gravity on cloth
		Vector gravity(0, -9.8, 0);
		Vector forceGrav = m_fVertexWeight * gravity;
		Vector summedForces = forceGrav;

		//compute spring dampening forces
		//--Structure dampeners
		if (i % m_iClothWidth != (m_iClothWidth - 1))
		{
			SpringDamper structDamp(vertices[i], vertices[i + 1], 100.0f, 0.2f, 1.0f);
			summedForces += structDamp.computeForce();

		}
		if (i % m_iClothWidth != 0)
		{
			SpringDamper structDamp(vertices[i], vertices[i - 1], 100.0f, 0.2f, 1.0f);
			summedForces += structDamp.computeForce();
		}
		if (i + m_iClothWidth < vertices.size())
		{
			SpringDamper structDamp(vertices[i], vertices[i + m_iClothWidth], 100.0f, 0.2f, 1.0f);
			summedForces += structDamp.computeForce();
		}

		//--Shear Dampeners
		if (i % (m_iClothWidth - 1) != 0 && (i + m_iClothWidth + 1) < vertices.size() || i == 0)//topleft
		{
			SpringDamper shearDamp(vertices[i], vertices[i + m_iClothWidth + 1], 1.0f, 0.2f, 1.414f);
			summedForces += shearDamp.computeForce();
		}
		if (i % (m_iClothWidth) > 0 && (i - m_iClothWidth - 1) > 0)//bottomright
		{
			SpringDamper shearDamp(vertices[i], vertices[i - m_iClothWidth - 1], 1.0f, 0.2f, 1.414f);
			summedForces += shearDamp.computeForce();
		}
		if (i % (m_iClothWidth - 1) != 0 && (i - m_iClothWidth + 1) > 0)//bottom left
		{
			SpringDamper shearDamp(vertices[i], vertices[i - m_iClothWidth + 1], 1.0f, 0.2f, 1.414f);
			summedForces += shearDamp.computeForce();
		}
		if (i % (m_iClothWidth - 1) != 0 && (i + m_iClothWidth - 1) < vertices.size())//top right
		{
			SpringDamper shearDamp(vertices[i], vertices[i + m_iClothWidth - 1], 1.0f, 0.2f, 1.414f);
			summedForces += shearDamp.computeForce();
		}

		//--bend dampers
		if ((i % m_iClothWidth) + 2 > m_iClothWidth  && i + 2 < vertices.size())//right + 1
		{
			SpringDamper bendDamp(vertices[i], vertices[i + 2], 0.1f, 0.02f, 2.0f);
			summedForces += bendDamp.computeForce();
		}
		if ((i % (m_iClothWidth)) - 2 >= 0)//left - 1
		{
			SpringDamper bendDamp(vertices[i], vertices[i - 2], 0.10f, 0.02f, 2.0f);
			summedForces += bendDamp.computeForce();
		}
		if (i + 2 * m_iClothWidth < vertices.size())//2 rows up
		{
			SpringDamper bendDamp(vertices[i], vertices[i + 2 * m_iClothWidth], 0.10f, 0.02f, 2.0f);
			summedForces += bendDamp.computeForce();
		}
		if (i - 2 * m_iClothWidth < vertices.size())//2 rows up
		{
			SpringDamper bendDamp(vertices[i], vertices[i - 2 * m_iClothWidth], 0.10f, 0.02f, 2.0f);
			summedForces += bendDamp.computeForce();
		}



		Vector acceleration = (1 / m_fVertexWeight)*summedForces;// *pow(t.totalTime, 2);
		Vector velocity(0, 0, 0);
		
		//use pair mappings to get the current velocity of the vertex
		for (std::vector<std::pair<Vector, Vector> >::iterator pairIt = vertexVelocity.begin(); pairIt != vertexVelocity.end(); ++pairIt)
		{
			if ((*pairIt).first == (vertices[i]) && ((*pairIt).first != vertices.front()))
				velocity = (*pairIt).second;
		}

		EulerIntegrator(vertices[i], velocity, acceleration, t);
	}

	BuildTriangleVec(m_iClothWidth, m_iClothHeight);
}

void Cloth::renderGeometry(atlas::math::Matrix4 projection,
	atlas::math::Matrix4 view)
{
	USING_ATLAS_GL_NS;
	USING_ATLAS_MATH_NS;

	// Enable the shaders.
	mShaders[0]->enableShaders();

	glBindVertexArray(mVao);
	

	auto mMat = projection * view * mModel;
//	auto mMat = mModel;
	glUniformMatrix4fv(mUniforms["Mat"], 1, GL_FALSE, &mMat[0][0]);
	//---draw
	glDrawArrays(GL_TRIANGLES, 0, triangle_vec.size());  //uncomment to see the mesh drawn as triangle strips
	
//	glPointSize(5.0);
//	glDrawArrays(GL_POINTS, 0, triangle_vec.size());

	// Disable them.
	mShaders[0]->disableShaders();
}

void Cloth::EulerIntegrator(atlas::math::Vector &pos, atlas::math::Vector velocity, atlas::math::Vector accel, atlas::utils::Time const& t)
{
	USING_ATLAS_MATH_NS;
	//pin some vertices so the whole thing doesn't fall to ground
	/*if (pos != Vector(3,float(m_iClothHeight-1),-0.5) 
		&& pos != Vector(float(m_iClothWidth-3),float(m_iClothHeight-1), ))*/
	if ((pos.x == 1 && pos.y == m_iClothHeight - 1) || (pos.x == m_iClothWidth - 3 && pos.y == m_iClothHeight - 1))
		return;
	{
		//update velocity
		Vector newVelocity = velocity + (accel * t.deltaTime);
	
		//update the vertex's stored velocity value
		for (std::vector<std::pair<Vector, Vector> >::iterator pairIt = vertexVelocity.begin(); pairIt != vertexVelocity.end(); ++pairIt)
		{
			if ((*pairIt).first == (pos))
				(*pairIt).second = newVelocity;
		}

		//update the vertex position
		Vector newPos = pos + newVelocity*t.deltaTime;
		pos = newPos;
	}
}

void Cloth::MakeVertices(int width, int height)
{
	USING_ATLAS_MATH_NS;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			vertices.push_back(Vector(float(j), float(i), 2*(float)rand() / (float)(RAND_MAX)));
			vertexVelocity.push_back(std::make_pair(vertices.back(), Vector(0, 0, 0)));  //every vertex starts with velocity of zero.
		}
	}

	BuildTriangleVec(m_iClothWidth, m_iClothHeight);

	//make the pixel coordinates for the height map texture
	//for (std::vector<Vector>::iterator itr = triangle_vec.begin(); itr != triangle_vec.end(); ++itr)
	//	vtexcoord.push_back(Vector((*itr).x() / float(width), (*itr).z() / float(height),0.0f));

}

void Cloth::BuildTriangleVec(int width, int height)
{
	USING_ATLAS_MATH_NS;
	//triangle strip
	if (!triangle_vec.empty())
		triangle_vec.clear();
	for (int j = 0; j < (height - 1); ++j)
	{
		//sqaures inside a triangle strip
		for (int i = 0; i < (width - 1); ++i)
		{
			int topleft = (j + 1) * width + i;
			int topright = topleft + 1;
			int bottomleft = (j)* width + i;
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
	//update OpenGL memory buffer, or nothing happens on screen.
	//--buffer
	glGenBuffers(1, &mBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(Vector))*triangle_vec.size(), &triangle_vec[0], GL_STATIC_DRAW);

	GLuint vpoint_id = glGetAttribLocation(mShaders[0]->getShaderProgram(), "vpoint");
	glEnableVertexAttribArray(vpoint_id);
	glVertexAttribPointer(vpoint_id, 3, GL_FLOAT, false, 0, 0);
}

void Cloth::reset()
{
	vertices.clear();
	vertexVelocity.clear();
	MakeVertices(m_iClothWidth, m_iClothHeight);
}