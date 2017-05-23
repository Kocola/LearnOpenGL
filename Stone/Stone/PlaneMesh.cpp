#include "PlaneMesh.h"

PlaneMesh::PlaneMesh()
{
	std::cout << "----------PlaneMesh Init----------" << std::endl;
	auto renderData = calcRenderData();
	setRenderData(renderData);
}

PlaneMesh::RenderData PlaneMesh::calcRenderData()
{
	//顶点，法向量，纹理坐标
	//三角形绘制，2个三角形，6个顶点
	GLfloat points[] = {
// 		1.0f, -0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
// 		-1.0f, -0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
// 		-1.0f, -0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
// 
// 		-1.0f, -0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
// 		1.0f, -0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
// 		1.0f, -0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, -0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

		-1.0f, -1.0f, -0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, -0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

	};

	std::vector<Vertex> vertices;
	for (int i = 0; i < 6; ++i)
	{
		Vertex vertex;
		vertex.position = glm::vec3(points[8 * i], points[8 * i + 1], points[8 * i + 2]);
		vertex.normal = glm::vec3(points[8 * i + 3], points[8 * i + 4], points[8 * i + 5]);
		vertex.texCoord = glm::vec2(points[8 * i + 6], points[8 * i + 7]);
		vertices.push_back(vertex);
	}

	RenderData renderData;
	renderData.vertexs.swap(vertices);
	return renderData;
}