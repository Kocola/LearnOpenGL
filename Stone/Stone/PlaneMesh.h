#ifndef					__PLANE_MESH_H__
#define					__PLANE_MESH_H__

#include "ShapeMesh.h"

class PlaneMesh : public ShapeMesh
{
public:
	PlaneMesh();

private:
	RenderData calcRenderData() override;
};

#endif