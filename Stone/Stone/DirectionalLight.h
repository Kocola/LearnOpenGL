#ifndef					__DIRECTIONAL_LIGHT_H__
#define					__DIRECTIONAL_LIGHT_H__

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(const glm::vec3& cameraPosition_);

public:
	void setLightDirection(const glm::vec3& direction_);

protected:
	virtual void updateShaderProgram() override;

private:
	void updateLightDirection();

private:
	glm::vec3				_direction;
};

#endif