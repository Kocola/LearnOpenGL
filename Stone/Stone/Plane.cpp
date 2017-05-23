#include "Mesh.h"
#include "Plane.h"
#include "PlaneMesh.h"

Plane::Plane()
	: Shape(Mesh<PlaneMesh>::getInstance().getRenderInfo())
{
}