#include <iostream>

#include "Common.h"
#include "ModelFile.h"
#include "Plane.h"
#include "PlaneFit.h"
#include "ResourceManager.h"
#include "StelaRubb.h"
#include "Timer.h"

StelaRubb::StelaRubb()
	:Application(SCREEN_WIDTH, SCREEN_HEIGHT, "StelaRubb")
	,_modelMatrix(glm::mat4(1.0f))
{
	init();
	initVertex();
	bindVertex();
}


StelaRubb::~StelaRubb()
{
	if (_plane != nullptr)
	{
		delete _plane;
		_plane = nullptr;
	}
}

bool StelaRubb::init()
{
	initTexture();
	initFBO();
	initShaderProgram();
	initShape();

	return true;
}

bool StelaRubb::initVertex()
{
	Timer timer;
	timer.start();
	ModelFile modelFile("../file/2800W2.ply");
	this->_points		= modelFile.readVertexs();
	this->_faces		= modelFile.readFaces();
	std::cout << "----------" << "打开模型耗时：" << timer.calcInvertal()
		<< "----------" << std::endl;
	PlaneFit planeFit(1, 1, _points, _faces);
	this->_colors		= planeFit.calcPointColor();

	this->setZ(0.0f);		//设置Z坐标为0，这样整个碑面会显示在z = 0的平面上
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
	//model = glm::rotate(model, 30.0f, glm::vec3(1.0, 0.0, 0.0));
	return model;
}

void StelaRubb::draw1()
{
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

void StelaRubb::initTexture()
{
	glCheckError();

	//初始化颜色纹理
	_colorTexture.setDataType(GL_FLOAT);
	_colorTexture.setInternalFormat(GL_RGB16F);
	_colorTexture.setImageFormat(GL_RGB);
	//_colorMap.setWrapType(GL_CLAMP_TO_BORDER);
	_colorTexture.generate(SCREEN_WIDTH, SCREEN_HEIGHT, nullptr);

	//初始化深度纹理
	_depthTexture.setDataType(GL_FLOAT);
	_depthTexture.setInternalFormat(GL_DEPTH_COMPONENT);
	_depthTexture.setImageFormat(GL_DEPTH_COMPONENT);
	_depthTexture.setWrapType(GL_CLAMP_TO_BORDER);
	_depthTexture.generate(SCREEN_WIDTH, SCREEN_HEIGHT, nullptr);

	glCheckError();
}

void StelaRubb::initFBO()
{
	glCheckError();

	glGenFramebuffers(1, &_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, _colorTexture.getTextureID(), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, _depthTexture.getTextureID(), 0);

	checkFrameBufferStatus();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCheckError();
}

void StelaRubb::initShaderProgram()
{
	_postProcessShaderProgram = ResourceManager::getInstance().getShaderProgram(
		"stelaRubbPostProcess");
}

void StelaRubb::initShape()
{
	glCheckError();

	_plane = new Plane();

	glCheckError();
}

void StelaRubb::firstDraw()
{
	glCheckError();

	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCheckError();
}

void StelaRubb::secondDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	
	_plane->setShaderProgram(_postProcessShaderProgram);
	_plane->setTexture2D(_colorTexture, "colorTexture");
	_plane->draw();

	glEnable(GL_DEPTH_TEST); 
}

void StelaRubb::draw()
{
	glCheckError();

	firstDraw();

	secondDraw();

	glCheckError();
}
