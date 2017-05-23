#include "Test.h"
#include "Cube.h"
#include "CubeMap.h"
#include "DirectionalLight.h"
#include "Light.h"
#include "Plane.h"
#include "PointLight.h"
#include "ResourceManager.h"
#include "Shape.h"
#include "SkyBox.h"
#include "Sphere.h"
#include "SpotLight.h"

Test::Test()
	:Application(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL")
{
	init();
}

void Test::draw()
{
// 	SkyBox* skyBox = new SkyBox();
// 	skyBox->setSideLen(40);
// 	skyBox->draw();
// 
// 	Shape* shape = new Sphere();
// 	shape->setScale(glm::vec3(3.0f));
// 	shape->setRotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
// 	shape->setTexture2D(ResourceManager::getInstance().getTexture2D("earth"));
// 	shape->setShaderProgram(ResourceManager::getInstance().getShaderProgram("basic"));
// 	shape->draw();
// 
// 	Shape* cube = new Cube();
// 	cube->setTranslate(glm::vec3(0, 0, 15));
// 	cube->setRotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
// 	cube->setShaderProgram(ResourceManager::getInstance().getShaderProgram("basic"));
// 	cube->draw();
// 
// 	delete skyBox;
// 	delete shape;
// 	delete cube;

	//glCullFace(GL_FRONT);
	firstDraw();
	//glCullFace(GL_BACK);

	secondDraw();
// 	glCheckError();
//  Plane* plane = new Plane();
//  plane->setTexture2D(_depthMap);
// 	plane->setShaderProgram(ResourceManager::getInstance().getShaderProgram("framebuffer"));
// 	plane->draw();
}


void Test::init()
{
	_depthMap.setImageFormat(GL_DEPTH_COMPONENT);
	_depthMap.setInternalFormat(GL_DEPTH_COMPONENT);
	_depthMap.setDataType(GL_FLOAT);
	_depthMap.setWrapType(GL_CLAMP_TO_BORDER);
	_depthMap.generate(SHADOW_WIDTH, SCREEN_HEIGHT, nullptr);

	glGenFramebuffers(1, &_depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
		_depthMap.getTextureID(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	_simpleDepthShaderProgram = ResourceManager::getInstance().getShaderProgram("simpleDepth");
	_simpleShadowShaderProgram = ResourceManager::getInstance().getShaderProgram("simpleShadow");
	_lightPos = glm::vec3(15.0f, 0.0f, 15.0f);
}

void Test::firstDraw()
{
	//首先渲染深度贴图
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	GLfloat near_plane = 1.0f;
	GLfloat far_plane = 100.0f;
	//glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 lightProjection = glm::perspective(45.0f, SCREEN_WIDTH * 1.0f / SCREEN_HEIGHT, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(_lightPos, glm::vec3(0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	_lightSpaceMatrix = lightProjection * lightView;
	_simpleDepthShaderProgram.setUniformValue("lightSpaceMatrix", _lightSpaceMatrix);

	Shape* shape = new Sphere();
	shape->setScale(glm::vec3(10.0f));
	shape->setRotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	shape->setTexture2D(ResourceManager::getInstance().getTexture2D("earth"));
	shape->setShaderProgram(_simpleDepthShaderProgram);
	shape->draw();
	
	Shape* cube = new Cube();
	cube->setTranslate(glm::vec3(10, 0, 10));
	cube->setRotate(30.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	cube->setShaderProgram(_simpleDepthShaderProgram);
	cube->draw();
	
	delete shape;
	delete cube;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Test::secondDraw()
{
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_simpleShadowShaderProgram.setUniformValue("lightPos", _lightPos);

	_simpleShadowShaderProgram.setUniformValue("viewPos", getCameraPosition());
	_simpleShadowShaderProgram.setUniformValue("lightSpaceMatrix", _lightSpaceMatrix);

	Shape* shape = new Sphere();
	shape->setScale(glm::vec3(10.0f));
	shape->setRotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	std::vector<Texture2D> sphereTexs{ ResourceManager::getInstance().getTexture2D("earth"), _depthMap };
	shape->setTexture2D(sphereTexs, {"diffuseTexture", "shadowMap"});
	shape->setShaderProgram(_simpleShadowShaderProgram);
	shape->draw();

	Shape* cube = new Cube();
	cube->setTranslate(glm::vec3(10, 0, 10));
	cube->setRotate(30.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	std::vector<Texture2D> cubeTexs{ ResourceManager::getInstance().getTexture2D("moss"), _depthMap };
	cube->setTexture2D(cubeTexs, { "diffuseTexture", "shadowMap" });
	cube->setShaderProgram(_simpleShadowShaderProgram);
	cube->draw();
	glCheckError();

	delete shape;
	delete cube;
}
