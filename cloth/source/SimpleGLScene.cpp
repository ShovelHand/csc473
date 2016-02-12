#include "SimpleGLScene.hpp"
#include <atlas/core/GLFW.hpp>
#include <atlas/core/Log.hpp>
#include <atlas/core/Macros.hpp>
#include <atlas/core/Float.hpp>

USING_ATLAS_GL_NS;
USING_ATLAS_MATH_NS;

SimpleGLScene::SimpleGLScene() :
mLastTime(0.0f),
mTick(10.0f/60.0f),
mAnimTime(0.0f),
mIsDragging(false)
{
    // Initialize the matrices to identities.
    mProjection = atlas::math::Matrix4(1.0f);
	mProjection = glm::perspective(glm::radians(45.0),
		1.0, 1.0, 1000.0);
    mView = atlas::math::Matrix4(1.0f);
}

SimpleGLScene::~SimpleGLScene()
{ }

void SimpleGLScene::updateScene(double time)
{
	mTime.deltaTime = (float)time - mTime.currentTime;
	mTime.totalTime += (float)time;
	mTime.currentTime = (float)time;

	if (atlas::core::geq(mTime.currentTime - mLastTime, mTick))
	{
		mLastTime += mTick;
		mTime.deltaTime = mTick;

		mAnimTime += mTick;
	}
	cloth.updateGeometry(mTime);
}

void SimpleGLScene::renderScene()
{
	glClearColor(1.0f, 1.0f, 1.0f,0);
    glClear(GL_COLOR_BUFFER_BIT);
	
	mView = mCamera.getCameraMatrix();
	cloth.renderGeometry(mProjection, mView);
	
}

void SimpleGLScene::mousePressEvent(int button, int action, int modifiers,
	double xPos, double yPos)
{
	USING_ATLAS_MATH_NS;

	if (button == GLFW_MOUSE_BUTTON_LEFT && modifiers == GLFW_MOD_ALT)
	{
		if (action == GLFW_PRESS)
		{
			mIsDragging = true;
			mCamera.mouseDown(Point2(xPos, yPos),
				MayaCamera::CameraMovements::TUMBLE);
		}
		else
		{
			mIsDragging = false;
			mCamera.mouseUp();
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && modifiers == GLFW_MOD_ALT)
	{
		if (action == GLFW_PRESS)
		{
			mIsDragging = true;
			mCamera.mouseDown(Point2(xPos, yPos),
				MayaCamera::CameraMovements::TRACK);
		}
		else
		{
			mIsDragging = false;
			mCamera.mouseUp();
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && modifiers == GLFW_MOD_ALT)
	{
		if (action == GLFW_PRESS)
		{
			// first click.
			mIsDragging = true;
			mCamera.mouseDown(Point2(xPos, yPos),
				MayaCamera::CameraMovements::DOLLY);
		}
		else
		{
			mIsDragging = false;
			mCamera.mouseUp();
		}
	}
	else if (action != GLFW_PRESS)
	{
		mIsDragging = false;
		mCamera.mouseUp();
	}
}

void SimpleGLScene::mouseMoveEvent(double xPos, double yPos)
{
	if (mIsDragging)
	{
		mCamera.mouseDrag(atlas::math::Point2(xPos, yPos));
	}
}

void SimpleGLScene::keyPressEvent(int key, int scancode, int action, int mods)
{
	UNUSED(scancode);
	UNUSED(mods);

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_R:
			mCamera.resetCamera();
			break;

	/*	case GLFW_KEY_S:
			mSpline.showSpline();
			break;

		case GLFW_KEY_C:
			mSpline.showControlPoints();
			break;

		case GLFW_KEY_G:
			mSpline.showCage();
			break;

		case GLFW_KEY_P:
			mSpline.showSplinePoints();
			break;

		case GLFW_KEY_SPACE:
			mIsPlaying = !mIsPlaying;*/


		default:
			break;
		}
	}
}