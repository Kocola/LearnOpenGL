#include "Cube.h"
#include "Plane.h"
#include "PointLight.h"
#include "ResourceManager.h"
#include "Shape.h"
#include "SkyBox.h"
#include "Sphere.h"
#include "SunEarthMoon.h"

SunEarthMoon::SunEarthMoon()
	:Application(SCREEN_WIDTH, SCREEN_HEIGHT, "SunEarthMoon")
{
	init();
}

SunEarthMoon::~SunEarthMoon()
{
	if (_sun != nullptr)
	{
		delete _sun;
		_sun = nullptr;
	}
	if (_earth != nullptr)
	{
		delete _earth;
		_earth = nullptr;
	}
	if (_moon != nullptr)
	{
		delete _moon;
		_moon = nullptr;
	}
}

void SunEarthMoon::init()
{
	glCheckError();
	std::vector<Image> images(6, Image(1024, 1024,
		nullptr));
	_cubeMap.setInternalFormat(GL_DEPTH_COMPONENT);
	_cubeMap.setImageFormat(GL_DEPTH_COMPONENT);
	_cubeMap.setDataType(GL_FLOAT);
	_cubeMap.generate(images);
	glCheckError();
	initFBO();
	initShadowMVP();
	initShaderProgram();
	initShape();
	initShapeInfo();
	glCheckError();
}

void SunEarthMoon::initFBO()
{
	glGenFramebuffers(1, &_depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _cubeMap.getCubeMapID(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SunEarthMoon::initShadowMVP()
{
	GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
	GLfloat nearPlane = 1.0f;
	_farPlane = 200.0f;
	_shadowProj = glm::perspective(90.0f, aspect, nearPlane, _farPlane);

	_lightPos = glm::vec3(0.0f, 0.0f, 0.0f);	//光源在原点
	
	_shadowTransforms.push_back(_shadowProj *
		glm::lookAt(_lightPos, _lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0f, -1.0f, 0.0f)));
	_shadowTransforms.push_back(_shadowProj *
		glm::lookAt(_lightPos, _lightPos + glm::vec3(-1.0f, 0.0, 0.0), glm::vec3(0.0f, -1.0f, 0.0f)));
	_shadowTransforms.push_back(_shadowProj *
		glm::lookAt(_lightPos, _lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	_shadowTransforms.push_back(_shadowProj *
		glm::lookAt(_lightPos, _lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	_shadowTransforms.push_back(_shadowProj *
		glm::lookAt(_lightPos, _lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	_shadowTransforms.push_back(_shadowProj *
		glm::lookAt(_lightPos, _lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
}

void SunEarthMoon::initShaderProgram()
{
	_depthShaderProgram = ResourceManager::getInstance().getShaderProgram("pointDepth");
 	for (int i = 0; i < 6; ++i)
 	{
 		std::string name = std::string("shadowMatrices[") + std::to_string(i) + std::string("]");
 		_depthShaderProgram.setUniformValue(name.c_str(), _shadowTransforms[i]);
 	}
 	_depthShaderProgram.setUniformValue("far_plane", _farPlane);
 	_depthShaderProgram.setUniformValue("lightPos", _lightPos);

	_shadowShaderProgram = ResourceManager::getInstance().getShaderProgram("pointShadow");
 	_shadowShaderProgram.setUniformValue("lightPos", _lightPos);
 	_shadowShaderProgram.setUniformValue("far_plane", _farPlane);
}

void SunEarthMoon::initShape()
{
	_sun = new Sphere();
 	auto sunTexture2D = ResourceManager::getInstance().getTexture2D("sun");
	//_sun->setScale(glm::vec3(10.0f));		//无用
 	_sun->setTexture2D(sunTexture2D);

	_earth = new Sphere();
	auto earthTexture2D = ResourceManager::getInstance().getTexture2D("earth");
	_earth->setTexture2D(earthTexture2D);

	_moon = new Sphere();
	auto moonTexture2D = ResourceManager::getInstance().getTexture2D("moon");
	_moon->setTexture2D(moonTexture2D);
}

void SunEarthMoon::initShapeInfo()
{
	_earthRotateAngle = 0.0f;
	_earthRotateSpeed = 0.6f;
	_moonRotateAngle = 0.0f;
	_moonRotateSpeed = 10.0f;

	_earthPosition = glm::vec3(0.0f, 20.0f, 0.0f);
	_moonPosition = glm::vec3(0.0f, -10.0f, 0.0f);
}

void SunEarthMoon::draw()
{
	static GLfloat angle = 0.0f;

	auto sunModel = glm::mat4(1.0f);
	sunModel = glm::rotate(sunModel, angle, glm::vec3(0.0, 0.0, 1.0));
// 	_earth->setTranslate(_earthPosition);
// 	_earth->setRotate(_earthRotateAngle, glm::vec3(0.0f, 1.0f, 0.0f));
// 	_earth->setScale(glm::vec3(13.0f));
	auto earthModel = glm::mat4(1.0f);
	earthModel = glm::translate(earthModel, _earthPosition);
	earthModel = glm::rotate(earthModel, _earthRotateAngle, glm::vec3(0.0, 0.0, 1.0));

	auto moonModel = glm::mat4(1.0f);
	moonModel = glm::translate(moonModel, _moonPosition);
	moonModel = glm::rotate(moonModel, _moonRotateAngle, glm::vec3(0.0, 0.0, 1.0));
	auto model = earthModel * moonModel;
	_moon->setModelMatrix(sunModel * earthModel * moonModel);

	earthModel = glm::scale(sunModel * earthModel, glm::vec3(3.0f));
	_earth->setModelMatrix(earthModel);

	angle += 0.20f;
	_earthRotateAngle += _earthRotateSpeed;
	_moonRotateAngle += _moonRotateSpeed;

	if (angle > 360)
	{
		angle -= 360;
	}

	if (_earthRotateAngle > 360)
	{
		_earthRotateAngle -= 360;
	}

	if (_moonRotateAngle > 360)
	{
		_moonRotateAngle -= 360;
	}

	firstDraw();
	secondDraw();
}

void SunEarthMoon::firstDraw()
{
	glCheckError();
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_earth->setShaderProgram(_depthShaderProgram);
	_earth->draw();

	_moon->setShaderProgram(_depthShaderProgram);
	_moon->draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SunEarthMoon::secondDraw()
{
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_shadowShaderProgram.setUniformValue("viewPos", getCameraPosition());

	_shadowShaderProgram.setUniformValue("depthMap", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMap.getCubeMapID());
	
	//auto shader = ResourceManager::getInstance().getShaderProgram("basic");

	_shadowShaderProgram.setUniformValue("reverseNormal", 1.0f);

	auto sunModel = glm::mat4(1.0f);
	sunModel = glm::scale(sunModel, glm::vec3(5.0f));
	auto sunLight = new PointLight(getCameraPosition());
	sunLight->setModelMatrix(sunModel);
	sunLight->setLightObject(_sun);
	sunLight->draw();

	_earth->setShaderProgram(_shadowShaderProgram);
	_earth->draw();

	_moon->setShaderProgram(_shadowShaderProgram);
	_moon->draw();

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
