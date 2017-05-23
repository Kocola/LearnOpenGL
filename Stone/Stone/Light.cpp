#include "Light.h"
#include "ResourceManager.h"

Light::LightColor::LightColor(const glm::vec3 color_ /* = glm::vec3(1.0f) */,
	float ambientStrength_ /* = 0.2f */, float diffuseStrength_ /* = 0.5f */, float specularStrength_ /* = 1.0f */)
{
	color				= color_;
	ambient		= color_ * ambientStrength_;
	diffuse			= color_ * diffuseStrength_;
	specular		= color_ * specularStrength_;
}

namespace{
	const glm::vec3 defaultLightColor = glm::vec3(1.0f);
}

Light::Light(const glm::vec3& cameraPosition_)
	: _cameraPosition(cameraPosition_),
	_lightColor(defaultLightColor), _lightColorAttribName("lightColor")
{
}

Light::~Light()
{
}

ShaderProgram Light::getLightShader()
{
	updateShaderProgram();
	return _shaderProgram;
}

void Light::updateShaderProgram()
{
	updateLightColor();
	updateCameraPosition();
}

glm::vec3 Light::getLightColor() const
{
	return _lightColor.color;
}

void Light::updateLightColor()
{
	_shaderProgram.setUniformValue((_lightColorAttribName + ".ambient").c_str(), _lightColor.ambient);
	_shaderProgram.setUniformValue((_lightColorAttribName + ".diffuse").c_str(), _lightColor.diffuse);
	_shaderProgram.setUniformValue((_lightColorAttribName + ".specular").c_str(), _lightColor.specular);
}

void Light::updateCameraPosition()
{
	_shaderProgram.setUniformValue("viewPos", this->_cameraPosition);
}

void Light::setLightColor(const glm::vec3& lightColor_)
{
	this->_lightColor = LightColor(lightColor_);
}

void Light::setLightColor(const LightColor& lightColor_)
{
	this->setLightColor(lightColor_.color);
}

void Light::setCameraPosition(const glm::vec3& cameraPosition_)
{
	this->_cameraPosition = cameraPosition_;
}