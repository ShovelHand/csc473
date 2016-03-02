#include "SimpleGLScene.hpp"
#include <atlas/core/GLFW.hpp>
#include <atlas/core/Log.hpp>
#include <atlas/core/Macros.hpp>
#include <atlas/core/Float.hpp>
#include <iostream>

USING_ATLAS_GL_NS;
USING_ATLAS_MATH_NS;

SimpleGLScene::SimpleGLScene() :
mLastTime(0.0f),
mTick(10.0f / 60.0f),
mAnimTime(0.0f),
mIsDragging(false),
mAnimLength(10.0f),
mSpline(int(mAnimLength * 60.0f)),
mFlyby(false),
fb(800, 800)

{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	// Initialize the matrices to identities.
	mProjection = atlas::math::Matrix4(1.0f);
	mProjection = glm::perspective(glm::radians(0.0),
		1.0, 1.0, 10.0);
	mView = atlas::math::Matrix4(1.0f);
//	mView *= glm::translate(atlas::math::Matrix4(1.0f), Vector(1, 1, 1));
	fb_tex = fb.init();
	ground.init(fb_tex);
	Vector eye(10, -10, -1);
	mCamera.setTrackVector(eye);
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
}

void SimpleGLScene::renderScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	mView = mCamera.getCameraMatrix();
	//mView = mCamera.lookAt(eye, Vector(0, 0, 0), Vector(0, 1, 0)); //calls like this work, but don't change the camera classes matrices.

	//TODO: reflective frame buffer stuff goes here
	Vector cam_pos = Vector(3,-1,5);
	Vector cam_mirror_pos = cam_pos;
	cam_mirror_pos.y *= -1;
	//Matrix4 view_mirror = mCamera.getCameraMatrix();
	//view_mirror *= glm::translate(Matrix4(1.0f), Vector(1, 1, 5));
	//view_mirror = mCamera.lookAt(cam_mirror_pos, Vector(1,1,1), Vector(0, 1, 0));
	
	fb.bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mSpline.renderGeometry(mProjection, mView);
	fb.unbind();

	ground.renderGeometry(mProjection, mView);
}

void SimpleGLScene::mousePressEvent(int button, int action, int modifiers,
	double xPos, double yPos)
{
	USING_ATLAS_MATH_NS;

	if (button == GLFW_MOUSE_BUTTON_LEFT)// && modifiers == GLFW_MOD_ALT)
	{
		if (action == GLFW_PRESS)
		{
			mIsDragging = !mIsDragging;
			mCamera.mouseDown(Point2(xPos, yPos),
				MayaCamera::CameraMovements::TUMBLE);
		}
		else
		{
			mIsDragging = false;
			mCamera.mouseUp();
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE)// && modifiers == GLFW_MOD_ALT)
	{
		if (action == GLFW_PRESS)
		{
			mIsDragging = true;
			mCamera.mouseDown(Point2(xPos, -yPos),
				MayaCamera::CameraMovements::DOLLY);
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

void SimpleGLScene::mouseScrollEvent(double xOffset, double yOffset)
{
	mCamera.inOutDolly(yOffset);
}

void SimpleGLScene::keyPressEvent(int key, int scancode, int action, int mods)
{
	UNUSED(scancode);
	UNUSED(mods);
	if (action == GLFW_RELEASE)
		return; ///< only act on PRESS

	float delta = 2.0f;  ///the step amount for wasd
	if (action == GLFW_PRESS)
	{
		atlas::math::Matrix4 translateMat;
		translateMat = (Matrix4(1.0f));
		switch (key)
		{
		case GLFW_KEY_R:
			mCamera.resetCamera();
			break;

		case GLFW_KEY_Q:
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
		case GLFW_KEY_W:

			break;
		case GLFW_KEY_S:

			break;
		case GLFW_KEY_A:
			//		eye -= xaxis;
			//		dirVec -= xaxis;
			break;
		case GLFW_KEY_D:
			//		eye += xaxis;
			//	dirVec += xaxis;
			break;
		case GLFW_KEY_I:
			//		printf("eye = %f, %f, %f dirVec = %f, %f, %f \n", eye.x(), eye.y(), eye.z(), dirVec.x(), dirVec.y(), dirVec.z());
			break;
			/*case GLFW_KEY_SPACE:
			mIsPlaying = !mIsPlaying;*/
		case GLFW_KEY_V:
			break;
		case GLFW_KEY_F:

			flyby();
			break;

		default:
			break;
		}
	}
}

void SimpleGLScene::flyby()
{
	mFlyby = !mFlyby;
	mSpline.ResetCurrentFrame();
}