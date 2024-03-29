#ifndef LAB03_INCLUDE_INTEGRATION_SCENE_HPP
#define LAB03_INCLUDE_INTEGRATION_SCENE_HPP

#pragma once

#include <atlas/utils/Scene.hpp>

#include "MayaCamera.hpp"
#include "Grid.hpp"
#include "CannonBall.hpp"
#include "LinearSpring.h"

class IntegrationScene : public atlas::utils::Scene
{
public:
    IntegrationScene();
    ~IntegrationScene();

    void mousePressEvent(int button, int action, int modifiers,
        double xPos, double yPos) override;
    void mouseMoveEvent(double xPos, double yPos) override;
    void keyPressEvent(int key, int scancode, int action, int mods) override;

    void screenResizeEvent(int width, int height) override;
    void renderScene() override;
    void updateScene(double time) override;

private:
    bool mIsDragging;
    bool mIsPlaying;

    float mLastTime;
    float mFPS;
    float mTick;

    float mAnimTime;
    float mAnimLength;

//    MayaCamera mCamera;
 //   Grid mGrid;
 //   CannonBall mBall;

	LinearSpring m_linSpring;
};

#endif