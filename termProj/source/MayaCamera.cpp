#include "MayaCamera.hpp"

#include <iostream>

USING_ATLAS_MATH_NS;
USING_GLM_NS;

struct MayaCamera::MayaCameraImpl
{
    MayaCameraImpl() :
        dolly(30.0f),
        tumbleVector(45.0f),
        trackVector(0.0f),
        lastPos(0.0f),
        movement(MayaCamera::CameraMovements::IDLE)
    {
        resetMatrices();
    }

    void resetAll()
    {
        dolly = 30.0f;
        tumbleVector = atlas::math::Vector(45.0f);
        trackVector = atlas::math::Vector(0.0f);
        resetMatrices();
    }

    void resetMatrices()
    {
        USING_ATLAS_MATH_NS;
        USING_GLM_NS;

        dollyMatrix = translate(Matrix4(1.0f), Vector(0, 0, -1.0f * dolly));
        trackMatrix = Matrix4(1.0f);
		trackMatrix *= glm::translate(Matrix4(1.0f), trackVector);
        tumbleMatrix = 
            rotate(Matrix4(1.0f), radians(tumbleVector.x), Vector(1, 0, 0)) *
            rotate(Matrix4(1.0f), radians(tumbleVector.y), Vector(0, 1, 0));
    }

    float dolly;
    atlas::math::Vector tumbleVector;
    atlas::math::Vector trackVector;
    atlas::math::Point2 lastPos;
    MayaCamera::CameraMovements movement;

    atlas::math::Matrix4 dollyMatrix;
    atlas::math::Matrix4 tumbleMatrix;
    atlas::math::Matrix4 trackMatrix;
};

MayaCamera::MayaCamera() :
    mImpl(new MayaCameraImpl)
{ }

MayaCamera::~MayaCamera()
{ }

void MayaCamera::mouseDown(atlas::math::Point2 const& point, 
    CameraMovements movement)
{
    mImpl->movement = movement;
    mImpl->lastPos = point;
}

void MayaCamera::mouseDrag(atlas::math::Point2 const& point)
{
    USING_ATLAS_MATH_NS;
    USING_GLM_NS;

    float deltaX = point.x - mImpl->lastPos.x;
    float deltaY = point.y - mImpl->lastPos.y;

    switch (mImpl->movement)
    {
    case CameraMovements::DOLLY:
        mImpl->dolly -= 0.0008f * deltaY;
        mImpl->dollyMatrix = 
            translate(Matrix4(1.0), Vector(0, 0, -1.0f * mImpl->dolly));
        break;

    case CameraMovements::TUMBLE:
        mImpl->tumbleVector.x += 0.005f * deltaY;
        mImpl->tumbleVector.y += 0.005f * deltaX;
        mImpl->tumbleMatrix =
            rotate(mat4(1.0), radians(mImpl->tumbleVector.x), vec3(1, 0, 0)) *
            rotate(mat4(1.0), radians(mImpl->tumbleVector.y), vec3(0, 1, 0));

    case CameraMovements::TRACK:
        break;
        mImpl->trackVector.x += 0.0005f * deltaX;
        mImpl->trackVector.y -= 0.0005f * deltaY;
        mImpl->trackMatrix = translate(mat4(1.0), mImpl->trackVector);
        break;

    case CameraMovements::IDLE:
        break;
    }
}

void MayaCamera::mouseUp()
{
    mImpl->movement = CameraMovements::IDLE;
}

void MayaCamera::resetCamera()
{
    mImpl->resetAll();
}

atlas::math::Matrix4 MayaCamera::getCameraMatrix()
{
    return mImpl->dollyMatrix * mImpl->trackMatrix * mImpl->tumbleMatrix;
}

void MayaCamera::setTrackVector(atlas::math::Vector eye)
{
	USING_ATLAS_MATH_NS;
	USING_GLM_NS;
	mImpl->trackVector = eye;
	mImpl->resetMatrices();
}

void MayaCamera::inOutDolly(int val)
{
	mImpl->dolly -=  val;
	mImpl->dollyMatrix =
		translate(Matrix4(1.0), Vector(0, 0, -1.0f * mImpl->dolly));
}

void MayaCamera::translateTrackVector(float delta)
{
	mImpl->dolly -= 0.0008f * delta;
	mImpl->dollyMatrix =
		translate(Matrix4(1.0), Vector(0, 0, -1.0f * mImpl->dolly));
}

atlas::math::Matrix4 MayaCamera::lookAt(atlas::math::Vector & eye, atlas::math::Vector & center, atlas::math::Vector & up){
//	typedef Eigen::Matrix<typename Derived::Scalar, 4, 4> Matrix4;
//	typedef Eigen::Matrix<typename Derived::Scalar, 3, 1> Vector3;
	Vector f = normalize(center - eye);
	Vector u = normalize(up);
	Vector s = normalize(cross(f, u));
	u = cross(s, f);
	Matrix4 mat;
	mat[0][0] = s.x;
	mat[0][1] = s.y;
	mat[0][2] = s.z;
	mat[0][3] = dot(-s, eye); 
	mat[1][0] = u.x;
	mat[1][1] = u.y;
	mat[1][2] = u.z;
	mat[1][3] = dot(-u, eye);
	mat[2][0] = -f.x;
	mat[2][1] = -f.y;
	mat[2][2] = -f.z;
	mat[2][3] = dot(f, eye);
	mat[3]  = Vector4(0, 0, 0, 1);
	return mat;
}