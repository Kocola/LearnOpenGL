#include "Mesh.h"
#include "Sphere.h"
#include "SphereMesh.h"

Sphere::Sphere()
	:Shape(Mesh<SphereMesh>::getInstance().getRenderInfo())
{
}