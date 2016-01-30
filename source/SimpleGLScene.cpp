#include "SimpleGLScene.hpp"
#include <atlas/core/GLFW.hpp>
#include <atlas/core/Log.hpp>
#include <atlas/core/Macros.hpp>
#include <atlas/core/Float.hpp>
SimpleGLScene::SimpleGLScene() :
mLastTime(0.0f),
mTick(0.1f),
mAnimTime(0.0f)
{
    // Initialize the matrices to identities.
    mProjection = atlas::math::Matrix4(1.0f);
    mView = atlas::math::Matrix4(1.0f);
}

SimpleGLScene::~SimpleGLScene()
{ }

void SimpleGLScene::updateScene(double time)
{
	mTime.currentTime = (float)time;
	mTime.totalTime += (float)time;

	if (atlas::core::geq(mTime.currentTime - mLastTime, mTick))
	{
		mLastTime += mTick;
		mTime.deltaTime = mTick;

		mAnimTime += mTick;
		m_linSpring.updateGeometry(mTime);
	}
}

void SimpleGLScene::renderScene()
{
	glClearColor(1.0f, 1.0f, 1.0f,0);
    glClear(GL_COLOR_BUFFER_BIT);

	m_linSpring.renderGeometry(mProjection, mView);
}