
#pragma once
#include <atlas/utils/Geometry.hpp>
#include "atlas/Atlas.hpp"

class Cloth : public atlas::utils::Geometry
{
public:
	Cloth();
	~Cloth();

	void renderGeometry();

	void updateGeometry(atlas::utils::Time const& t) override;
	void EulerIntegrator(atlas::utils::Time const& t);

private:
	
	void MakeVertices(int width, int height);
	std::vector<atlas::math::Vector> vertices;
	std::vector<atlas::math::Vector> triangle_vec; //defines the order in which vertices are used in the triangle strips
//	std::vector<vec2> vtexcoord;  //texture coords for height map. Covers whole mesh

	atlas::math::Vector mRefPosition;
	atlas::math::Vector mModelVelocity;
	atlas::math::Vector mModelPosition;
	atlas::math::Vector mModelOldPosition;

	atlas::math::Matrix4 mRefMatrix;

	GLuint mVao;
	GLuint mBuffer;

};