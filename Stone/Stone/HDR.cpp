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
	glCheckError();

	_colorTexture.setDataType(GL_FLOAT);
	_colorTexture.setInternalFormat(GL_RGB16F);
	_colorTexture.setImageFormat(GL_RGB);
	_colorTexture.generate(SCREEN_WIDTH, SCREEN_HEIGHT, nullptr);

	_depthTexture.setDataType(GL_FLOAT);
	_depthTexture.setInternalFormat(GL_DEPTH_COMPONENT);
	_depthTexture.setImageFormat(GL_DEPTH_COMPONENT);
	_depthTexture.setWrapType(GL_CLAMP_TO_BORDER);
	_depthTexture.generate(SCREEN_WIDTH, SCREEN_HEIGHT, nullptr);

	glCheckError();
}

void HDR::initFBO()
{
	glCheckError();

	glGenFramebuffers(1, &_hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _hdrFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,_colorTexture.getTextureID(), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
		GL_TEXTURE_2D, _depthTexture.getTextureID(), 0);
	glCheckError();

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCheckError();
}

void HDR::initShaderProgram()
{
	_exposure = 1.1f;
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
	//_cube->setTexture2D(ResourceManager::getInstance().getTexture2D("normalMoss"));
	
	_plane = new Plane();

	_lightObject = new Sphere();
	_lightObject->setTexture2D(ResourceManager::getInstance().getTexture2D("earth"));
}

void HDR::draw()
{
	glCheckError();
	firstDraw();
	secondDraw();
	glCheckError();
}

void HDR::firstDraw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _hdrFBO);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0f);
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
	glDisable(GL_DEPTH_TEST);

	_plane->setShaderProgram(_hdrShaderProgram);
	_plane->setTexture2D(_colorTexture, "hdrBuffer");
	_plane->draw();

	glEnable(GL_DEPTH_TEST);
}
