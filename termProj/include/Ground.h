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
		atlas::math::Matrix4 view);

	void updateGeometry();
	void setFrameBufferTex(GLuint texture){ _tex_mirror = texture; bMirrorTexSet = true; }
	void init(GLuint texture);

private:

	GLuint mVao;
	GLuint mBuffer;
	GLuint _tex; ///< Texture ID
	bool bMirrorTexSet;
	GLuint _tex_mirror; ///< Texture ID of mirror texture

	atlas::math::Matrix4 mRefMatrix;
	atlas::math::Vector mModelPosition;
};