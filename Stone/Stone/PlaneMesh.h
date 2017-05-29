#ifndef					__PLANE_MESH_H__
#define					__PLANE_MESH_H__

#include <vector>

#include "ShapeMesh.h"

class PlaneMesh : public ShapeMesh
{
public:
	PlaneMesh();

private:
	RenderData calcRenderData() override;

	void calcTangentSpace(const std::vector<Vertex>& vertexs);

	TangentSpace calcTangentSpace(const Vertex& v1, const Vertex& v2,
		const Vertex& v3);
};

#endif