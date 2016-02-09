#ifndef LAB01_INCLUDE_SIMPLE_GL_SCENE_HPP
#define LAB01_INCLUDE_SIMPLE_GL_SCENE_HPP

#pragma once

#include <atlas/utils/Scene.hpp>

class SimpleGLScene : public atlas::utils::Scene
{
public:
    SimpleGLScene();
    ~SimpleGLScene();

    void renderScene() override;
	void updateScene(double time) override;

private:


	float mAnimTime;
	float mLastTime;
	float mTick;
};

#endif
