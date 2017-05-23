#include "SphereMesh.h"

SphereMesh::SphereMesh()
{
	std::cout << "----------SphereMesh Init----------" << std::endl;
	auto renderData = calcRenderData();
	setRenderData(renderData);
}

SphereMesh::RenderData SphereMesh::calcRenderData()
{
	const float radius = 1.0f;

	int division = (int)(1024 * radius) / (4 * 4);
	float angleStrip = 360.0f / division;	//偏角间隔

	std::vector<Vertex> vertices;
	int indices[] = { 0, 1, 2, 2, 3, 0 };		//两个三角面片构成一个四角面片

	for (int y = 0; y < division / 2; ++y)
	{
		float pitch = (y - division / 4) * angleStrip;
		for (int x = 0; x < division; ++x)
		{
			float yaw = x * angleStrip;
			glm::vec3 position[4];
			glm::vec2 texCoord[4];

			//bottom left
			position[0] = sphericalToCartesianCoord(radius, pitch, yaw);
			texCoord[0] = glm::vec2(float(x) / division, float(y) * 2.0f / division);
			//bottom right
			position[1] = sphericalToCartesianCoord(radius, pitch, yaw + angleStrip);
			texCoord[1] = glm::vec2(float(x + 1) / division, float(y) * 2.0f / division);
			//top right
			position[2] = sphericalToCartesianCoord(radius, pitch + angleStrip,
				yaw + angleStrip);
			texCoord[2] = glm::vec2(float(x + 1) / division, float(y + 1) * 2.0f / division);
			//top left
			position[3] = sphericalToCartesianCoord(radius, pitch + angleStrip, yaw);
			texCoord[3] = glm::vec2(float(x) / division, float(y + 1)  * 2.0f / division);

			for (int i = 0; i < 6; ++i)
			{
				const int index = indices[i];
				Vertex vertex;

				vertex.position = position[index];
				vertex.normal = vertex.position;
				vertex.texCoord = texCoord[index];

				vertices.push_back(vertex);
			}
		}
	}

	RenderData renderData;
	renderData.vertexs = std::move(vertices);
	return renderData;
}

glm::vec3 SphereMesh::sphericalToCartesianCoord(float radius_, float pitch_, float yaw_)
{
	glm::vec3 position;

	float rPitch = glm::radians(pitch_);
	float rYaw = glm::radians(yaw_);

	position.y = radius_ * glm::sin(rPitch);
	position.x = radius_ * glm::cos(rPitch) * glm::sin(rYaw);
	position.z = radius_ * glm::cos(rPitch) * glm::cos(rYaw);
	// 	position.y = _radius * glm::sin(rPitch);
	// 	position.z = _radius * glm::cos(rPitch) * glm::sin(rYaw);
	// 	position.x = _radius * glm::cos(rPitch) * glm::cos(rYaw);

	return position;
}
