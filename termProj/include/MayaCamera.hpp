#ifndef LAB04_INCLUDE_MAYA_CAMERA_HPP
#define LAB04_INCLUDE_MAYA_CAMERA_HPP

#pragma once
#include <atlas/utils/Camera.hpp>
#include <memory>

class MayaCamera : public atlas::utils::Camera
{
public:
    MayaCamera();
    ~MayaCamera();

    void mouseDown(atlas::math::Point2 const& point,
        CameraMovements movement) override;
    void mouseDrag(atlas::math::Point2 const& point) override;
    void mouseUp() override;
    void resetCamera() override;
    atlas::math::Matrix4 getCameraMatrix() override;

	void setTrackVector(atlas::math::Vector eye);
	atlas::math::Vector getTrackVector();
	void translateTrackVector(float delta);
	void inOutDolly(int val);
	atlas::math::Matrix4 lookAt(atlas::math::Vector & eye, atlas::math::Vector & center, atlas::math::Vector & up);

private:
    struct MayaCameraImpl;
    std::unique_ptr<MayaCameraImpl> mImpl;
};

#endif


