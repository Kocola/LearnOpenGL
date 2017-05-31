#include "Cube.h"
#include "HDR.h"
#include "Plane.h"
#include "ResourceManager.h"
#include "Sphere.h"

HDR::HDR()
	: Application(SCREEN_WIDTH, SCREEN_HEIGHT, "HDR"),
	_pointLight(glm::vec3(0.0f)), _cube(nullptr), _plane(nullptr)
{
	init();
}

HDR::~HDR()
{
	if (_cube != nullptr){
		delete _cube;
		_cube = nullptr;
	}

	if (_plane != nullptr)
	{
		delete _plane;
		_plane = nullptr;
	}
	if (_lightObject != nullptr)
	{
		delete _lightObject;
		_lightObject = nullptr;
	}
}

void HDR::init()
{
	initTexture();
	initFBO();
	initShaderProgram();
	initShape();
}

void HDR::initTexture()
{
	_colorTexture.setDataType(GL_FLOAT);
	_colorTexture.setInternalFormat(GL_RGB16F);
	_colorTexture.setImageFormat(GL_RGB);
	_colorTexture.generate(SCREEN_WIDTH, SCREEN_HEIGHT, nullptr);
}

void HDR::initFBO()
{
	glGenFramebuffers(1, &_hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _hdrFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _colorTexture.getTextureID(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void HDR::initShaderProgram()
{
	_exposure = 1.0f;
	_hdrShaderProgram = ResourceManager::getInstance().getShaderProgram("exposureHdr");
	_hdrShaderProgram.setUniformValue("exposure", _exposure);
}

void HDR::initShape()
{
	_cube = new Cube();
	auto model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0, 0.0, -30.0));
	//model = glm::rotate(model, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f));
	_cube->setModelMatrix(model);
	
	_plane = new Plane();

	_lightObject = new Sphere();
	_lightObject->setTexture2D(ResourceManager::getInstance().getTexture2D("sun"));
}

void HDR::draw()
{
	glCheckError();
	firstDraw();
	//secondDraw();
	glCheckError();
}

void HDR::firstDraw()
{
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//glBindFramebuffer(GL_FRAMEBUFFER, _hdrFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0, 0.0, 5.0));
	model = glm::scale(model, glm::vec3(0.5f));
	_pointLight.setModelMatrix(model);
	_pointLight.setCameraPosition(getCameraPosition());
	_pointLight.setLightObject(_lightObject);
	_pointLight.draw();
	//_pointLight.setLightColor(glm::vec3(1.0f, 0.0f, 0.0f));
	ShaderProgram lightShaderProgram = _pointLight.getLightShader();
	_cube->setShaderProgram(lightShaderProgram);
	_cube->draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void HDR::secondDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_plane->setShaderProgram(_hdrShaderProgram);
	_plane->setTexture2D(_colorTexture, "hdrBuffer");
	_plane->draw();
}
