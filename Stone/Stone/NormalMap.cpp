#include "Plane.h"
#include "NormalMap.h"
#include "ResourceManager.h"
#include "Shape.h"
#include "Utility.h"

NormalMap::NormalMap()
	: Application(SCREEN_WIDTH, SCREEN_HEIGHT, "normalMap"),
	_plane(nullptr)
{
	init();
}

NormalMap::~NormalMap()
{
	if (_plane != nullptr)
	{
		delete _plane;
	}
}

void NormalMap::init()
{
	_texture2D = ResourceManager::getInstance().getTexture2D("brickWall");
	_normalTexture2D = ResourceManager::getInstance().getTexture2D("normalBrickWall");
	_shaderProgram = ResourceManager::getInstance().getShaderProgram("simpleNormalMap");

	_plane = new Plane();
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0, 0.0, -5.0));
	//model = glm::rotate(model, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f));
	_plane->setModelMatrix(model);
	_plane->setTexture2D(std::vector < Texture2D > {_texture2D, _normalTexture2D},
		std::vector < std::string > {"diffuseTexture", "normalMap"});
}

void NormalMap::draw()
{
	_shaderProgram.setUniformValue("lightPos", glm::vec3(0.0, 0.0, 0.0));
	_shaderProgram.setUniformValue("viewPos", getCameraPosition());
	_plane->setShaderProgram(_shaderProgram);
	_plane->draw();
}