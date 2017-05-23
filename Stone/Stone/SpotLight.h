#ifndef					__SPOT_LIGHT_H__
#define					__SPOT_LIGHT_H__

#include "PointLight.h"

class SpotLight : public PointLight
{
public:
	struct SpotData
	{
		glm::vec3			direction;
		float					incutoff;
		float					outcutoff;
		SpotData(const glm::vec3& direction_ = glm::vec3(1.0f), float incutoff = 12.5f,
			float outcutoff = 17.5f);
	};

public:
	SpotLight(const glm::vec3& cameraPosition_);

public:
	void setLightDirection(const glm::vec3& lightDirection_);

	void setLightIncutoff(float incutoff_);

	void setLightOutcutoff(float outcutoff_);

	void setLight(const SpotData& spotData_);

protected:
	virtual void updateShaderProgram() override;

private:
	void updateSpotData();

private:
	SpotData	_spotData;
	std::string	_spotDataName;
};

#endif