#include <iostream>

#include "Common.h"
#include "ModelFile.h"
#include "PlaneFit.h"
#include "ResourceManager.h"
#include "StealRubb.h"
#include "Timer.h"

StelaRubb::StelaRubb()
	:Application(SCREEN_WIDTH, SCREEN_HEIGHT, "StelaRubb")
	,_modelMatrix(glm::mat4(1.0f))
{
	//init();
	initVertex();
	bindVertex();
}

bool StelaRubb::init()
{
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f,
		0.5f, 0.5f, 0.0f,	1.0f, 1.0f,
		//-0.5f, -0.5f, 0.0f,
		//0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,	0.0f, 1.0f
		
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	glCheckError();

	glGenVertexArrays(1, &this->_VAO);
	glBindVertexArray(this->_VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), BUFFER_OFFSET(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	
	GLuint VEO;
	glGenBuffers(1, &VEO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glDeleteBuffers(1, &VBO);	//绑定了VAO，这里仅相当于调用glBindBuffers(0);
	glCheckError();

	return true;
}

bool StelaRubb::initVertex()
{
	Timer timer;
	timer.start();
	ModelFile modelFile("../file/360w.ply");
	this->_points		= modelFile.readVertexs();
	this->_faces		= modelFile.readFaces();
	std::cout << "----------" << "打开模型耗时：" << timer.calcInvertal()
		<< "----------" << std::endl;
	PlaneFit planeFit(1, 1, _points, _faces);
	this->_colors		= planeFit.calcPointColor();

	//this->setZ(0.0f);
	_modelMatrix = calcModelMatrix(planeFit);
//	this->_colors = std::vector<glm::vec3>(_points.size(), glm::vec3(0.0, 0.0, 0.0));
	std::cout << "----------" << "数据处理耗时：" << timer.calcInvertal()
		<<"----------"<< std::endl;
	return true;
}

bool StelaRubb::bindVertex()
{
	glCheckError();

	glGenVertexArrays(1, &this->_VAO);
	glBindVertexArray(this->_VAO);

	GLuint pVBO;
	glGenBuffers(1, &pVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pVBO);
	glBufferData(GL_ARRAY_BUFFER, this->_points.size() * sizeof(_points[0]), &_points[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint cVBO;
	glGenBuffers(1, &cVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cVBO);
	glBufferData(GL_ARRAY_BUFFER, this->_colors.size() * sizeof(_colors[0]), &_colors[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint VEO;
	glGenBuffers(1, &VEO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->_faces.size() * sizeof(_faces[0]), &_faces[0], GL_STATIC_DRAW);
	
	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}

void StelaRubb::setZ(float z_)
{
	for (int i = 0; i < _points.size(); ++i)
	{
		_points[i].z = z_;
	}
}

glm::mat4 StelaRubb::calcModelMatrix(const PlaneFit& planeFit_)
{
	const float xOffset = (planeFit_._minX + (planeFit_._maxX - planeFit_._minX) / 2.0f);
	const float yOffset = (planeFit_._minY + (planeFit_._maxY - planeFit_._minY) / 2.0f);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-xOffset, -yOffset, 0.0f));
//	model = glm::translate(model, glm::vec3(-planeFit_._maxX, -planeFit_._maxY, 0.0f));
	return model;
}

void StelaRubb::draw()
{
// 	auto texture = ResourceManager::getInstance().getTexture2DPointer("moss");
// 	texture->bind();
// 	auto pShader = ResourceManager::getInstance().getShaderProgramPointer("basic");
// 	pShader->bind();
// 	glBindVertexArray(this->_VAO);
// 	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->VEO);*/
// 	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
// 	//glDrawArrays(GL_TRIANGLES, 0, 6);
// 	glBindVertexArray(0);

	static bool isFirst = true;
	static Timer timer;

	if (isFirst)
	{
		timer.start();
	}

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	auto pShader = ResourceManager::getInstance().getShaderProgramPointer("stelaRubb");
	pShader->setUniformValue("model", _modelMatrix);
	pShader->use();
	glBindVertexArray(this->_VAO);
	glDrawElements(GL_TRIANGLES, _faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	pShader->release();

	if (isFirst)
	{
		std::cout << "----------" << "渲染耗时：" << timer.calcInvertal()
			<< "----------" << std::endl;
	}

	isFirst = false;
}