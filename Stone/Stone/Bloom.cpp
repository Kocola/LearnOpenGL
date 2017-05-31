#include "Bloom.h"
#include "Cube.h"
#include "Plane.h"
#include "ResourceManager.h"
#include "Sphere.h"

Bloom::Bloom()
	: Application(SCREEN_WIDTH, SCREEN_HEIGHT, "Bloom"),
	_pointLight(glm::vec3(0.0f)),
	_cube(nullptr), _plane(nullptr), _lightObject(nullptr)
{
	init();
}

Bloom::~Bloom()
{
	if (_cube != nullptr)
	{
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

void Bloom::init()
{
	initTexture();
	initFBO();
	initShaderProgram();
	initShape();
}

void Bloom::initTexture()
{
	glCheckError();

	for (int i = 0; i < sizeof(_colorBuffers) / sizeof(_colorBuffers[0]); ++i)
	{
		_colorBuffers[i].setInternalFormat(GL_RGB16F);
		_colorBuffers[i].setImageFormat(GL_RGB);
		_colorBuffers[i].setDataType(GL_FLOAT);
		_colorBuffers[i].generate(SCREEN_WIDTH, SCREEN_HEIGHT, nullptr);
	}

	for (int i = 0; i < sizeof(_pingpongBuffers) / sizeof(_pingpongBuffers[0]); ++i)
	{
		_pingpongBuffers[i].setInternalFormat(GL_RGB16F);
		_pingpongBuffers[i].setImageFormat(GL_RGB);
		_pingpongBuffers[i].setDataType(GL_FLOAT);
		_pingpongBuffers[i].generate(SCREEN_WIDTH, SCREEN_HEIGHT, nullptr);
	}

	_depthTexture.setDataType(GL_FLOAT);
	_depthTexture.setInternalFormat(GL_DEPTH_COMPONENT);
	_depthTexture.setImageFormat(GL_DEPTH_COMPONENT);
	_depthTexture.setWrapType(GL_CLAMP_TO_BORDER);
	_depthTexture.generate(SCREEN_WIDTH, SCREEN_HEIGHT, nullptr);

	glCheckError();
}

void Bloom::initFBO()
{
	glCheckError();

	glGenFramebuffers(1, &_hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _hdrFBO);
	for (int i = 0; i < sizeof(_colorBuffers) / sizeof(_colorBuffers[0]); ++i)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
			GL_TEXTURE_2D, _colorBuffers[i].getTextureID(), 0);
	}
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, _depthTexture.getTextureID(), 0);

	//显式告知OpenGL我们正在通过glDrawBuffers渲染多个颜色缓冲，
	//否则，OpenGL只会渲染到帧缓冲的第一个颜色附件
	GLuint attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(sizeof(attachments) / sizeof(attachments[0]), attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	auto pingpongFBOLen = sizeof(_pingpongFBO) / sizeof(_pingpongFBO[0]);
	glGenFramebuffers(pingpongFBOLen, _pingpongFBO);
	for (int i = 0; i < pingpongFBOLen; ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _pingpongFBO[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
			_pingpongBuffers[i].getTextureID(), 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	glCheckError();
}

void Bloom::initShaderProgram()
{
	glCheckError();

	_hdrShaderProgram = ResourceManager::getInstance().getShaderProgram("bloomBlend");
	_hdrShaderProgram.setUniformValue("exposure", 1.0f);

	_bloomShaderProgram = ResourceManager::getInstance().getShaderProgram("bloom");
	glm::vec3 lightPos = glm::vec3(0.0f);
	_bloomShaderProgram.setUniformValue("lightPos", lightPos);
	
	glCheckError();
}

void Bloom::initShape()
{
	glCheckError();

	_cube = new Cube();
	auto model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0, 0.0, -12.0));
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f));
	_cube->setModelMatrix(model);

	_plane = new Plane();

	_lightObject = new Sphere();
	_lightObject->setTexture2D(ResourceManager::getInstance().getTexture2D("white"));
	model = glm::mat4(1.0f);
	model = glm::rotate(model, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0, 0.0, 7.0));
	model = glm::scale(model, glm::vec3(0.8f));
	_lightObject->setModelMatrix(model);

	glCheckError();
}

void Bloom::draw()
{
	glCheckError();

	firstDraw();
	secondDraw();

	glCheckError();
}

void Bloom::firstDraw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _hdrFBO);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClearColor(0.0, 0.0, 0.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	_lightObject->draw();

	_bloomShaderProgram.setUniformValue("viewPos", getCameraPosition());
	_cube->setShaderProgram(_bloomShaderProgram);
	_cube->setTexture2D(ResourceManager::getInstance().getTexture2D("moss"), "diffuseTexture");
	_cube->draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLboolean horizontal = GL_TRUE, firstIteration = GL_TRUE;
	GLuint amount = 10;

	ShaderProgram bloomBlur = ResourceManager::getInstance().getShaderProgram("bloomBlur");
	_plane->setShaderProgram(bloomBlur);
	for (int i = 0; i < amount; ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _pingpongFBO[horizontal]);
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glClearColor(0.0, 0.0, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		bloomBlur.setUniformValue("horizontal", horizontal);
		if (firstIteration)
		{
			_plane->setTexture2D(_colorBuffers[1], "image");
		}
		else
		{
			_plane->setTexture2D(_pingpongBuffers[!horizontal], "image");
		}
		_plane->draw();
		horizontal = !horizontal;
		if (firstIteration)
		{
			firstIteration = GL_FALSE;
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Bloom::secondDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	_plane->setShaderProgram(_hdrShaderProgram);
	_plane->setTexture2D(std::vector < Texture2D > {_colorBuffers[0], _pingpongBuffers[0]},
		std::vector < std::string > {"scene", "bloomBlur"});
	_plane->draw();

	glEnable(GL_DEPTH_TEST);
}
