
#pragma once
#include <atlas/utils/Geometry.hpp>
#include "atlas/Atlas.hpp"


class Cloth : public atlas::utils::Geometry
{
public:
	Cloth();
	~Cloth();

	void renderGeometry(atlas::math::Matrix4 projection,
		atlas::math::Matrix4 view);

	void updateGeometry(atlas::utils::Time const& t) override;
	void EulerIntegrator(atlas::math::Vector &pos, atlas::math::Vector velocity, atlas::math::Vector accel, atlas::utils::Time const& t);

private:
	
	void MakeVertices(int width, int height);
	void BuildTriangleVec(int width, int height);
	std::vector<atlas::math::Vector> vertices;
	std::vector<atlas::math::Vector> triangle_vec; //defines the order in which vertices are used in the triangle strips
	std::vector<atlas::math::Vector> vtexcoord;  //texture coords in case the cloth ever gets a texture

	std::vector<std::pair<atlas::math::Vector, atlas::math::Vector> > vertexMap;
	std::vector<std::pair<atlas::math::Vector, atlas::math::Vector> > vertexVelocity;
	

	atlas::math::Vector mRefPosition;
	atlas::math::Vector mModelVelocity;
	atlas::math::Vector mModelPosition;
	atlas::math::Vector mModelOldPosition;

	atlas::math::Matrix4 mRefMatrix;
	
	float m_fVertexWeight;
	int m_iClothWidth;
	int m_iClothHeight;

	GLuint mVao;
	GLuint mBuffer;

};