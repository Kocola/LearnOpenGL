#include "DirectionalLight.h"
#include "ResourceManager.h"

namespace{
	const std::string defaultShaderName = "directionalLight";
}

DirectionalLight::DirectionalLight(const glm::vec3& cameraPosition_)
	: Light(cameraPosition_),
	_direction(glm::vec3(1.0f, 1.0f, 1.0f))
{
	_shaderProgram = ResourceManager::getInstance().getShaderProgram(defaultShaderName);
}

void DirectionalLight::setLightDirection(const glm::vec3& direction_)
{
	this->_direction = _direction;
}

void DirectionalLight::updateShaderProgram()
{
	Light::updateShaderProgram();
	updateLightDirection();
}

void DirectionalLight::updateLightDirection()
{
	_shaderProgram.setUniformValue("lightDirection", this->_direction);
}