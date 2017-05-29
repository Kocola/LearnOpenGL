#include "PlaneMesh.h"

PlaneMesh::PlaneMesh()
{
	std::cout << "----------PlaneMesh Init----------" << std::endl;
	auto renderData = calcRenderData();
	setRenderData(renderData);
}

PlaneMesh::RenderData PlaneMesh::calcRenderData()
{
	//���㣬����������������
	//�����λ��ƣ�2�������Σ�6������
	GLfloat points[] = {
// 		1.0f, -0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
// 		-1.0f, -0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
// 		-1.0f, -0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
// 
// 		-1.0f, -0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
// 		1.0f, -0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
// 		1.0f, -0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, -0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-1.0f, -1.0f, -0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, -0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
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

	calcTangentSpace(vertices);

	RenderData renderData;
	renderData.vertexs.swap(vertices);
	return renderData;
}

void PlaneMesh::calcTangentSpace(const std::vector<Vertex>& vertexs_)
{
	std::vector<TangentSpace> tangentSpaces;
	for (int i = 0; i < vertexs_.size(); i += 3)
	{
		auto tmp = calcTangentSpace(vertexs_[i], vertexs_[i + 1], vertexs_[i + 2]);
		//ÿ�����㶼���������ߺ͸����ߵ����ݣ�
		//�佫����Ϊ�������ݴ��붥����ɫ��		----		2017.5.29
		tangentSpaces.push_back(tmp);
		tangentSpaces.push_back(tmp);
		tangentSpaces.push_back(tmp);
	}
	setTangentSpace(tangentSpaces);
}

PlaneMesh::TangentSpace PlaneMesh::calcTangentSpace(const Vertex& v1, const Vertex& v2, 
	const Vertex& v3)
{
	auto e1 = v1.position - v2.position;
	auto e2 = v2.position - v3.position;
	auto deltaUV1 = v1.texCoord - v2.texCoord;
	auto deltaUV2 = v2.texCoord - v3.texCoord;
	auto f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
	
	glm::vec3 tangent;
	tangent.x = f * (deltaUV2.y * e1.x - deltaUV1.y * e2.x);
	tangent.y = f * (deltaUV2.y * e1.y - deltaUV1.y * e2.y);
	tangent.z = f * (deltaUV2.y * e1.z - deltaUV1.y * e2.z);
	tangent = glm::normalize(tangent);

	glm::vec3 bitangent;
	bitangent.x = f * (-deltaUV2.x * e1.x + deltaUV1.x * e2.x);
	bitangent.y = f * (-deltaUV2.x * e1.y + deltaUV1.x * e2.y);
	bitangent.z = f * (-deltaUV2.x * e1.z + deltaUV1.x * e2.z);
	bitangent = glm::normalize(bitangent);

	return TangentSpace(tangent, bitangent);
}
