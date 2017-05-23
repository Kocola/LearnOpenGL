#include "ShapeMesh.h"

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

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, this->_renderData.vertexs.size() * sizeof(this->_renderData.vertexs[0]),
		&this->_renderData.vertexs[0], GL_STATIC_DRAW);
	//Vertex相关数据在shader中使用固定的layout
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offsetof(Vertex, texCoord)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

// 	std::cout << "----------" << "VAO : " << VAO
// 		<< ", VBO : " << VBO << std::endl;

	return RenderInfo(VAO, VBO, 0, _renderData.vertexs.size());
}

void ShapeMesh::setRenderData(const RenderData& renderData_)
{
	_renderData = renderData_;
}