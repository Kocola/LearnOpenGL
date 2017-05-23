#include "PointLight.h"
#include "ResourceManager.h"

PointLight::Attenuation::Attenuation(
	float constant_ /* = 1.0f */, float linear_ /* = 0.35f */, float quadratic_ /* = 0.44f */)
	: constant(constant_), linear(linear_), quadratic(quadratic_)
{
}

namespace
{ 
	const std::string defaultShaderName = "pointLight";
	const glm::vec3 defaultPosition = glm::vec3(0.0f);
};

PointLight::PointLight(const glm::vec3& cameraPosition_)
	: Light(cameraPosition_),
	_attenuation(Attenuation()), _attenuationName("lightAttenuation")
{
	_lightShaderProgram = ResourceManager::getInstance().getShaderProgram("basic");
	_shaderProgram = ResourceManager::getInstance().getShaderProgram(defaultShaderName);
}

void PointLight::draw()
{
	if (_lightObject == nullptr)
	{
		return;
	}
	_lightShaderProgram.setUniformValue("lightColor", getLightColor());
	_lightObject->setModelMatrix(_modelMatrix);
	_lightObject->setShaderProgram(_lightShaderProgram);
	_lightObject->draw();
}

void PointLight::setAttenuation(const Attenuation& attenuation_)
{
	this->_attenuation = attenuation_;
}

void PointLight::setModelMatrix(const glm::mat4& modelMatrix_)
{
	this->_modelMatrix = modelMatrix_;
}

void PointLight::setLightObject(Shape* lightObject_)
{
	this->_lightObject = lightObject_;
}

void PointLight::updateShaderProgram()
{
	Light::updateShaderProgram();
	updateModelMatrix();
	updateLightPosition();
	updateAttenuation();
}

void PointLight::updateAttenuation()
{
	_shaderProgram.setUniformValue((_attenuationName + ".constant").c_str(), _attenuation.constant);
	_shaderProgram.setUniformValue((_attenuationName + ".linear").c_str(), _attenuation.linear);
	_shaderProgram.setUniformValue((_attenuationName + ".quadratic").c_str(), _attenuation.quadratic);
}

void PointLight::updateModelMatrix()
{
	_shaderProgram.setUniformValue("model", _modelMatrix);
}

void PointLight::updateLightPosition()
{
	auto position = glm::vec3(_modelMatrix[3]);
	_shaderProgram.setUniformValue("lightPos", position);
}