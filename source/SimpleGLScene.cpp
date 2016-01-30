#include "SimpleGLScene.hpp"
#include <atlas/core/GLFW.hpp>
#include <atlas/core/Log.hpp>
#include <atlas/core/Macros.hpp>
#include <atlas/core/Float.hpp>
SimpleGLScene::SimpleGLScene()
{
    // Initialize the matrices to identities.
    mProjection = atlas::math::Matrix4(1.0f);
    mView = atlas::math::Matrix4(1.0f);
}

SimpleGLScene::~SimpleGLScene()
{ }

void SimpleGLScene::renderScene()
{
	glClearColor(1.0f, 1.0f, 1.0f,0);
    glClear(GL_COLOR_BUFFER_BIT);

	m_linSpring.renderGeometry(mProjection, mView);
}