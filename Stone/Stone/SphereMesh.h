#ifndef					__SPHERE_MESH_H__
#define					__SPHERE_MESH_H__

#include "ShapeMesh.h"

class SphereMesh : public ShapeMesh
{
public:
	SphereMesh();

private:
	virtual RenderData calcRenderData();

	glm::vec3 sphericalToCartesianCoord(float radius_, float pitch_,
		float yaw_);
};

#endif