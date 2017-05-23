#include "ResourceManager.h"
#include "SpotLight.h"

SpotLight::SpotData::SpotData(const glm::vec3& direction_ /* = glm::vec3(1.0f) */,
	float incutoff_ /* = 12.5f */, float outcutoff_ /* = 17.5f */)
	: direction(direction_), incutoff(incutoff_), outcutoff(outcutoff_)
{
}

namespace
{
	const std::string defaultShaderName = "spotLight";
	const glm::vec3 defaultDirection = glm::vec3(1.0f);
};

SpotLight::SpotLight(const glm::vec3& cameraPosition_)
	: PointLight(cameraPosition_), _spotData(SpotData()),
	_spotDataName("spotLight")
{
	_shaderProgram = ResourceManager::getInstance().getShaderProgram(defaultShaderName);
}

void SpotLight::setLightDirection(const glm::vec3& lightDirection_)
{
	_spotData.direction = lightDirection_;
}

void SpotLight::setLightIncutoff(float incutoff_)
{
	_spotData.incutoff = incutoff_;
}

void SpotLight::setLightOutcutoff(float outcutoff_)
{
	_spotData.outcutoff = outcutoff_;
}

void SpotLight::setLight(const SpotData& spotData_)
{
	setLightDirection(spotData_.direction);
	setLightIncutoff(spotData_.incutoff);
	setLightOutcutoff(spotData_.outcutoff);
}

void SpotLight::updateShaderProgram()
{
	PointLight::updateShaderProgram();
	updateSpotData();
}

void SpotLight::updateSpotData()
{
	_shaderProgram.setUniformValue((_spotDataName + ".direction").c_str(), _spotData.direction);
	_shaderProgram.setUniformValue((_spotDataName + ".incutoff").c_str(), glm::cos(glm::radians(_spotData.incutoff)));
	_shaderProgram.setUniformValue((_spotDataName + ".outcutoff").c_str(), glm::cos(glm::radians(_spotData.outcutoff)));
}
