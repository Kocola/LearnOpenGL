#ifndef					__SHAPE_MESH_H__
#define					__SHAPE_MESH_H__

#include <vector>

#include "Common.h"

class ShapeMesh
{
	template<typename T>
	friend class Mesh;
public:
	RenderInfo getRenderInfo();

protected:
	struct RenderData
	{
		std::vector<Vertex>			vertexs;
		std::vector<IndiceType>	indices;

		RenderData() = default;

		RenderData(const std::vector<Vertex>& vertex_,
			const std::vector<IndiceType>& indices_); 
	};

	virtual RenderData calcRenderData() = 0;

	void setRenderData(const RenderData& renderData_);

protected:
	ShapeMesh() = default;

private:
	//禁止拷贝构造和赋值构造
	ShapeMesh(const ShapeMesh&) = delete;

	ShapeMesh& operator=(const ShapeMesh&) = delete;

private:
	RenderData _renderData;
};

#endif