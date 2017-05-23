#include "Cube.h"
#include "CubeMesh.h"
#include "Mesh.h"

Cube::Cube()
	:Shape(Mesh<CubeMesh>::getInstance().getRenderInfo())
{
}