#ifndef					__CUBE_MESH_H__
#define					__CUBE_MESH_H__

#include "ShapeMesh.h"

class CubeMesh : public ShapeMesh
{
public:
	CubeMesh();

private:
	virtual RenderData calcRenderData();
};

#endif