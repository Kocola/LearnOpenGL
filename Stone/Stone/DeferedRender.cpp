#include "Cube.h"
#include "DeferedRender.h"
#include "Plane.h"
#include "ResourceManager.h"
#include "Sphere.h"

DenderedRender::LightInfo::LightInfo(const glm::vec3& position_, 
	float radius_,
	const glm::vec3& color_ /* = glm::vec3(1.0f) */)
	: position(position_), radius(radius_), color(color_)
{
}

DenderedRender::DenderedRender()
	: Application(SCREEN_WIDTH, SCREEN_HEIGHT, "DeferedRender"),
	_plane(nullptr)
{
	init();
}

DenderedRender::~DenderedRender()
{
	for (int i = 0; i < _objects.size(); ++i)
	{
		if (_objects[i] != nullptr)
		{
			delete _objects[i];
			_objects[i] = nullptr;
		}
	}

	for (int i = 0; i < _lightSources.size(); ++i)
	{
		if (_lightSources[i] != nullptr)
		{
			delete _lightSources[i];
			_lightSources[i] = nullptr;
		}
	}

	if (_plane != nullptr)
	{
		delete _plane;
		_plane = nullptr;
	}
}

void DenderedRender::init()
{
	initTexture();
	initFBO();
	initShaderProgram();
	initShape();
}

void DenderedRender::initTexture()
{
	glCheckError();

	//深度缓冲，如果没有会无法进行GL_DEPTH_TEST
	_depthStencilTexture.setInternalFormat(GL_DEPTH_COMPONENT);
	_depthStencilTexture.setImageFormat(GL_DEPTH_COMPONENT);
	_depthStencilTexture.setDataType(GL_FLOAT);
	_depthStencilTexture.setWrapType(GL_CLAMP_TO_BORDER);
	_depthStencilTexture.generate(SCREEN_WIDTH, SCREEN_HEIGHT, nullptr);

	//位置颜色缓冲
	_positionTexture.setInternalFormat(GL_RGB16F);
	_positionTexture.setImageFormat(GL_RGB);
	_positionTexture.setDataType(GL_FLOAT);
	_positionTexture.generate(SCREEN_WIDTH, SCREEN_HEIGHT, nullptr);

	//法线颜色缓冲
	_normalTexture.setInternalFormat(GL_RGB16F);
	_normalTexture.setImageFormat(GL_RGB);
	_normalTexture.setDataType(GL_FLOAT);
	_normalTexture.generate(SCREEN_WIDTH, SCREEN_HEIGHT, nullptr);

	//颜色+镜面颜色缓冲
	_albedoSpecTexture.setInternalFormat(GL_RGBA);
	_albedoSpecTexture.setImageFormat(GL_RGBA);
	_albedoSpecTexture.setDataType(GL_FLOAT);
	_albedoSpecTexture.generate(SCREEN_WIDTH, SCREEN_HEIGHT, nullptr);

	glCheckError();
}

void DenderedRender::initFBO()
{
	glGenFramebuffers(1, &_gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _gBuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, _depthStencilTexture.getTextureID(), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, _positionTexture.getTextureID(), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, _normalTexture.getTextureID(), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
		GL_TEXTURE_2D, _albedoSpecTexture.getTextureID(), 0);

	//告诉OpenGL我们将要使用（帧缓冲的）哪种颜色附件进行渲染
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	checkFrameBufferStatus();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCheckError();
}

void DenderedRender::initShaderProgram()
{
	_geometryPassShaderProgram = ResourceManager::getInstance().getShaderProgram("deferedRender");

	_lightRenderShaderProgram = ResourceManager::getInstance().getShaderProgram("deferedRenderLight");
	_lightRenderShaderProgram2 = ResourceManager::getInstance().getShaderProgram("deferedRenderLight2");
}

void DenderedRender::initShape()
{
	_plane = new Plane();

	for (int i = 0; i < 10; ++i)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, i * 45.0f, glm::vec3(0.0, 1.0, 0.0));
		model = glm::translate(model, glm::vec3(5, 0.0, 0.0));
		model = glm::scale(model, glm::vec3(1.0f));

		Shape* shape = new Cube();
		shape->setModelMatrix(model);
		_objects.push_back(shape);
	}

	const GLfloat constant = 1.0f;
	const GLfloat linear = 0.7;
	const GLfloat quadratic = 1.8;
	auto lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	const GLfloat lightMax = std::fmax(std::fmax(lightColor.r, lightColor.g), lightColor.b);
	const GLfloat radius =
		(-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0 / 5.0) * lightMax))) / (2 * quadratic);

	for (int i = 0; i < 3; ++i)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3((i - 1) * 4, 1.2, 0.0));
		model = glm::scale(model, glm::vec3(0.2f));

		Shape* shape = new Sphere();
		shape->setModelMatrix(model);
		_lightSources.push_back(shape);

		auto position = glm::vec3(model * glm::vec4(0.0, 0.0, 0.0, 1.0f));
		_lightInfos.push_back(LightInfo(position, radius, lightColor));

		_lightRenderShaderProgram.setUniformValue((std::string("lights[") + std::to_string(i) + std::string("].position")).c_str(), position);
		_lightRenderShaderProgram.setUniformValue((std::string("lights[") + std::to_string(i) + std::string("].color")).c_str(), glm::vec3(1.0f));
		_lightRenderShaderProgram.setUniformValue((std::string("lights[") + std::to_string(i) + std::string("].radius")).c_str(), radius);
		_lightRenderShaderProgram.setUniformValue((std::string("lights[") + std::to_string(i) + std::string("].linear")).c_str(), linear);
		_lightRenderShaderProgram.setUniformValue((std::string("lights[") + std::to_string(i) + std::string("].quadratic")).c_str(), quadratic);

		_lightRenderShaderProgram2.setUniformValue((std::string("lights[") + std::to_string(i) + std::string("].position")).c_str(), position);
		_lightRenderShaderProgram2.setUniformValue((std::string("lights[") + std::to_string(i) + std::string("].color")).c_str(), glm::vec3(1.0f));
		_lightRenderShaderProgram2.setUniformValue((std::string("lights[") + std::to_string(i) + std::string("].radius")).c_str(), radius);
		_lightRenderShaderProgram2.setUniformValue((std::string("lights[") + std::to_string(i) + std::string("].linear")).c_str(), linear);
		_lightRenderShaderProgram2.setUniformValue((std::string("lights[") + std::to_string(i) + std::string("].quadratic")).c_str(), quadratic);

		model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(radius));
		Shape* lightVolumn = new Sphere();
		lightVolumn->setModelMatrix(model);
		_lightVolumns.push_back(lightVolumn);
	}
}

void DenderedRender::draw()
{
	firstDraw();
	secondDraw();
}

void DenderedRender::firstDraw()
{
	glCheckError();

	glBindFramebuffer(GL_FRAMEBUFFER, _gBuffer);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	Texture2D diffuseTexture = ResourceManager::getInstance().getTexture2D("moss");
	Texture2D	specularTexture = ResourceManager::getInstance().getTexture2D("specularMoss");
	for (int i = 0; i < _objects.size(); ++i)
	{
		_objects[i]->setTexture2D(std::vector < Texture2D > {diffuseTexture, specularTexture},
			std::vector < std::string > {"diffuseTexture", "specularTexture"});
		_objects[i]->setShaderProgram(_geometryPassShaderProgram);
		_objects[i]->draw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCheckError();
}

void DenderedRender::secondDraw()
{
	//commonSecondDraw();
	lightVolumnSecondDraw();
	//stencilLightVolumnSecondDraw();
}

void DenderedRender::commonSecondDraw()
{
	glCheckError();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_plane->setTexture2D(std::vector < Texture2D > {_positionTexture, _normalTexture, _albedoSpecTexture},
		std::vector < std::string > {"gPosition", "gNormal", "gAlbedoSpec"});
	_lightRenderShaderProgram.setUniformValue("viewPos", getCameraPosition());
	_plane->setShaderProgram(_lightRenderShaderProgram);
	_plane->draw();

	//将自定义帧缓冲gBuffer内的深度缓冲信息复制到默认帧缓冲的深度缓冲区
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	for (int i = 0; i < _lightSources.size(); ++i)
	{
		_lightSources[i]->setTexture2D(ResourceManager::getInstance().getTexture2D("white"));
		_lightSources[i]->draw();
	}

	glCheckError();
}

void DenderedRender::lightVolumnSecondDraw()
{
	glCheckError();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	//glBlendFuncSeparate(GL_SRC_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ONE);
	glBlendEquation(GL_MAX);

	for (int i = 0; i < _lightVolumns.size(); ++i)
	{
		_lightVolumns[i]->setTexture2D(std::vector < Texture2D > {_positionTexture, _normalTexture, _albedoSpecTexture},
			std::vector < std::string > {"gPosition", "gNormal", "gAlbedoSpec"});
		_lightRenderShaderProgram2.setUniformValue("viewPos", getCameraPosition());
		_lightVolumns[i]->setShaderProgram(_lightRenderShaderProgram2);
		_lightVolumns[i]->draw();
	}

	glDisable(GL_BLEND);

	//将自定义帧缓冲gBuffer内的深度缓冲信息复制到默认帧缓冲的深度缓冲区
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	for (int i = 0; i < _lightSources.size(); ++i)
	{
		_lightSources[i]->setTexture2D(ResourceManager::getInstance().getTexture2D("white"));
		_lightSources[i]->draw();
	}

	glDisable(GL_CULL_FACE);
	glCheckError();
}

void DenderedRender::stencilLightVolumnSecondDraw()
{
	glCheckError();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	for (int i = 0; i < _lightVolumns.size(); ++i)
	{
		_lightVolumns[i]->draw();
	}

	glEnable(GL_DEPTH_TEST);
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilMask(0x00);	//禁止修改模板缓冲

	commonSecondDraw();

	glStencilMask(0xFF);	//如果没有这行，会出现缓冲区看来未清空的结果！不清楚原因

	glCheckError();
}
