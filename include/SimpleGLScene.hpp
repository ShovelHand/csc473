#ifndef LAB01_INCLUDE_SIMPLE_GL_SCENE_HPP
#define LAB01_INCLUDE_SIMPLE_GL_SCENE_HPP

#pragma once

#include <atlas/utils/Scene.hpp>
#include "LinearSpring.h"
#include "AngularSpring.h"

class SimpleGLScene : public atlas::utils::Scene
{
public:
    SimpleGLScene();
    ~SimpleGLScene();

    void renderScene() override;
	void updateScene(double time) override;

private:
	LinearSpring m_linSpring;
	AngularSpring m_angSpring;

	float mAnimTime;
	float mLastTime;
	float mTick;
};

#endif
