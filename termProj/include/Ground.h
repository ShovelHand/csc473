#pragma once
#include <atlas/utils/Geometry.hpp>
#include <atlas/core/Macros.hpp>
#include <atlas/core/Float.hpp>
#include <atlas/core/Log.hpp>

class Ground : public atlas::utils::Geometry
{
public:
	Ground();
	~Ground();

	void renderGeometry(atlas::math::Matrix4 projection,
		atlas::math::Matrix4 view) override;

	void updateGeometry();

private:

	GLuint mVao;
	GLuint mBuffer;
	GLuint _tex; ///< Texture ID

	atlas::math::Matrix4 mRefMatrix;
	atlas::math::Vector mModelPosition;

};