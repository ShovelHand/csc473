#ifndef LAB01_INCLUDE_SIMPLE_GL_SCENE_HPP
#define LAB01_INCLUDE_SIMPLE_GL_SCENE_HPP

#pragma once

#include <atlas/utils/Scene.hpp>
#include "MayaCamera.hpp"
#include "Spline.hpp"
#include "Ground.h"
#include "FrameBuffer.h"

class SimpleGLScene : public atlas::utils::Scene
{
public:
    SimpleGLScene();
    ~SimpleGLScene();

	//camera and mouse/key press stuff
	void mousePressEvent(int button, int action, int modifiers, double xPos, double yPos) override;
	void mouseMoveEvent(double xPos, double yPos) override;
	void keyPressEvent(int key, int scancode, int action, int mods) override;
	void mouseScrollEvent(double xOffset, double yOffset) override;
    void renderScene() override;
	void updateScene(double time) override;

	void flyby();

private:

	bool mIsDragging;

	float mAnimTime;
	float mLastTime;
	float mTick;
	Spline mSpline;//the spline along which the maya camera runs.
	float mAnimLength;
	bool mFlyby;
	MayaCamera mCamera;
	Ground ground;
	
	atlas::math::Vector eye;
	atlas::math::Vector dirVec;

	void mousemove(int x, int y);
	void selection_button(int button, int action);
	void mouseScroll(double xOffset, double yOffset);

	//mirror reflection stuff
	FrameBuffer fb;
	GLuint fb_tex;
};



#endif