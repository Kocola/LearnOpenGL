#ifndef					__MESH_H__
#define					__MESH_H__

#include "Common.h"
#include "ShapeMesh.h"
#include "Singleton.h"

template<typename T>
class Mesh : public Singleton<Mesh<T>>
{
public:
	Mesh()
	{
		std::cout << "----------Mesh Init----------" << std::endl;
		_mesh = new T();
	}

	RenderInfo getRenderInfo()
	{
		return _mesh->getRenderInfo();
	}

private:
	ShapeMesh* _mesh;
};

// template<typename T>
// Mesh<T>::Mesh()
// {
// 	_mesh = new T();
// }
// 
// template<typename T>
// RenderInfo Mesh<T>::getRenderInfo()
// {
// 	return _mesh->getRenderInfo();
// }

#endif