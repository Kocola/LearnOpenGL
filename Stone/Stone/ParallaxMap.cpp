#include "ParallaxMap.h"
#include "Plane.h"
#include "ResourceManager.h"

ParallaxMap::ParallaxMap()
	: Application(1024, 768, "parallaxMap"),
	_plane(nullptr)
{
	init();
}

ParallaxMap::~ParallaxMap()
{
	if (_plane != nullptr)
	{
		delete _plane;
		_plane = nullptr;
	}
}

void ParallaxMap::init()
{
	_diffuseTexture = ResourceManager::getInstance().getTexture2D("parallaxBricks");
	_normalTexture = ResourceManager::getInstance().getTexture2D("parallaxBricksNormal");
	_depthTexture = ResourceManager::getInstance().getTexture2D("parallaxBricksDepth");

	_shaderProgram = ResourceManager::getInstance().getShaderProgram("parallaxOcclusionMap");

	_plane = new Plane();
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0, -5.0, 0.0));
	model = glm::rotate(model, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f));
	_plane->setModelMatrix(model);
	_plane->setTexture2D(std::vector < Texture2D > {_diffuseTexture, _normalTexture, _depthTexture},
		std::vector < std::string > {"diffuseMap", "normalMap", "depthMap"});
}

void ParallaxMap::draw()
{
	glCheckError();
	_shaderProgram.setUniformValue("lightPos", glm::vec3(0.0, 0.0, 0.0));
	_shaderProgram.setUniformValue("viewPos", getCameraPosition());
	_shaderProgram.setUniformValue("heightScale", 0.1f);
	_plane->setShaderProgram(_shaderProgram);
	_plane->draw();
	glCheckError();
}
