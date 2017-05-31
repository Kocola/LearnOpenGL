#include "ShapeMesh.h"

ShapeMesh::TangentSpace::TangentSpace(const glm::vec3& tangent_, const glm::vec3& bitangent_)
	: tangent(tangent_), bitangent(bitangent_)
{
}

ShapeMesh::RenderData::RenderData(const std::vector<Vertex>& vertex_,
	const std::vector<IndiceType>& indices_)
	: vertexs(vertex_), indices(indices_)
{
}

RenderInfo ShapeMesh::getRenderInfo()
{
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	auto vertexDataSize = this->_renderData.vertexs.size()
		* sizeof(this->_renderData.vertexs[0]);
	auto tangentSpaceDataSize = 0;
	if (!this->_tangentSpaces.empty())
	{
		tangentSpaceDataSize = this->_tangentSpaces.size()
			* sizeof(this->_tangentSpaces[0]);
	}

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexDataSize + tangentSpaceDataSize,
		nullptr, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, vertexDataSize, &this->_renderData.vertexs[0]);
	//Vertex相关数据在shader中使用固定的layout
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offsetof(Vertex, texCoord)));
	glEnableVertexAttribArray(2);

	if (tangentSpaceDataSize != 0)
	{
		glBufferSubData(GL_ARRAY_BUFFER, vertexDataSize, tangentSpaceDataSize,
			&this->_tangentSpaces[0]);
		// 加入切线空间    ----    2017.5.29
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TangentSpace), BUFFER_OFFSET(vertexDataSize));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(TangentSpace), BUFFER_OFFSET(vertexDataSize +
			offsetof(TangentSpace, bitangent)));
		glEnableVertexAttribArray(4);
	}
	
// 	std::cout << "----------" << "VAO : " << VAO
// 		<< ", VBO : " << VBO << std::endl;

	return RenderInfo(VAO, VBO, 0, _renderData.vertexs.size());
}

void ShapeMesh::setRenderData(const RenderData& renderData_)
{
	this->_renderData = renderData_;
}

void ShapeMesh::setTangentSpace(const std::vector<TangentSpace>& tangentSpaces_)
{
	this->_tangentSpaces = tangentSpaces_;
}
