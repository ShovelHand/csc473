#include "Ground.h"

#pragma once
#include "ShaderPaths.hpp"
#include <atlas/core/Macros.hpp>
#include <atlas/core/GLFW.hpp>
#include "TextureLoader.h"

typedef unsigned char BYTE;
/*ctor*/
Ground::Ground() :
mModelPosition(1, -3, 1),
bMirrorTexSet(false)
{
	USING_ATLAS_GL_NS;
	USING_ATLAS_MATH_NS;

	mModel *= glm::scale(Matrix4(1.0f), Vector(1, 1, 1));
	//	mModel *= glm::translate(Matrix4(1.0f), Vector(1, 10, 1));
	// Get the path where our shaders are stored.
	std::string shaderDir = generated::ShaderPaths::getShaderDirectory();

	// Now set up the information for our shaders.
	std::vector<ShaderInfo> shaders
	{
		ShaderInfo{ GL_VERTEX_SHADER, shaderDir + "Ground.vs.glsl" },  //not sure if sharing shaders is bad news or not.
		ShaderInfo{ GL_FRAGMENT_SHADER, shaderDir + "Ground.fs.glsl" }
	};


	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	// Create a new shader and add it to our list.
	mShaders.push_back(ShaderPointer(new Shader));
	// Compile the shaders.
	mShaders[0]->compileShaders(shaders);
	// And link them.
	mShaders[0]->linkShaders();
	GLfloat vertices[] =
	{//hexagon shape
		/*V1*/ -10.0f, 0.0f, -10.0f,
		/*V2*/ +10.0f, 0.0f, -10.0f,
		/*V3*/ -10.0f, 0.0f, +10.0f,
		/*V4*/ +10.0f, 0.0f, +10.0f
	};

	//--buffer
	glGenBuffers(1, &mBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//attribute
	GLuint vpoint_id = glGetAttribLocation(mShaders[0]->getShaderProgram(), "vpoint");
	glEnableVertexAttribArray(vpoint_id);
	glVertexAttribPointer(vpoint_id, 3, GL_FLOAT, false, 0, 0);
}
void Ground::init(GLuint texture = -1)
{

	GLuint mMat = mShaders[0]->getUniformVariable("Mat");
	mUniforms.insert(UniformKey("Mat", mMat));

	///--- Texture coordinates
	{
		const GLfloat vtexcoord[] = { 
			/*V1*/ 0.0f, 0.0f,
			/*V2*/ 1.0f, 0.0f,
			/*V3*/ 0.0f, 1.0f,
			/*V4*/ 1.0f, 1.0f };

		///--- Buffer
		glGenBuffers(1, &mBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vtexcoord), vtexcoord, GL_STATIC_DRAW);

		///--- Attribute
		GLuint vtexcoord_id = glGetAttribLocation(mShaders[0]->getShaderProgram(), "vtexcoord");
		glEnableVertexAttribArray(vtexcoord_id);
		glVertexAttribPointer(vtexcoord_id, 2, GL_FLOAT, false, 0, 0);
		

		///--- Load texture
		glGenTextures(1, &_tex);
		glBindTexture(GL_TEXTURE_2D, _tex);
		const char * filename = "C:\\Users\\Alexander\\Documents\\csc_assignments\\csc473\\assgts/termProj/shaders/textures/lena512.bmp";
		_tex = loadBMP_custom(filename);
		
		_tex_mirror = texture;
		
		///--- Texture uniforms
		GLuint tex_id = glGetUniformLocation(mShaders[0]->getShaderProgram(), "tex");
		glUniform1i(tex_id, 0/*GL_TEXTURE0*/);
		GLuint tex_mirror_id = glGetUniformLocation(mShaders[0]->getShaderProgram(), "tex_mirror");
		glUniform1i(tex_mirror_id, 1 /*GL_TEXTURE1*/);
	}
	
	// Disable at the end to avoid mixing shaders.
	glBindVertexArray(0);
	glUseProgram(0);
}

/*dtor*/
Ground::~Ground()
{
	glDeleteVertexArrays(1, &mVao);
	glDeleteBuffers(1, &mBuffer);
}

void Ground::updateGeometry()
{//not sure if much will happen here
	USING_ATLAS_GL_NS;
	USING_ATLAS_MATH_NS;
//	mModel = refMatrix;

}

void Ground::renderGeometry(atlas::math::Matrix4 projection,
	atlas::math::Matrix4 view)
{
	// Enable the shaders.
	mShaders[0]->enableShaders();

	glBindVertexArray(mVao);
	///--- Bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _tex);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _tex_mirror);

	atlas::math::Matrix4 mvp = projection * view * mModel;
	//load transformations
	glUniformMatrix4fv(mUniforms["Mat"], 1, GL_FALSE, &mvp[0][0]);

//	glEnable(GL_CLIP_PLANE0);
	glEnable(GL_BLEND);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisable(GL_BLEND);
//	glDisable(GL_CLIP_PLANE0);
	glBindVertexArray(0);
	glUseProgram(0);
}

