#ifndef					__LIGHT_H__
#define					__LIGHT_H__

#include "Common.h"
#include "Shape.h"

class Light
{
public:
	struct LightColor
	{
		glm::vec3 color;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		LightColor(const glm::vec3 color_ = glm::vec3(1.0f), float ambientStrength_ = 0.1f, 
			float diffuseStrength_ = 0.5f, float specularStrength_ = 1.0f);
	};

public:
	Light(const glm::vec3& cameraPosition_);

	virtual ~Light() = 0;

public:
	ShaderProgram getLightShader();

public:
	void setLightColor(const glm::vec3& lightColor_);

	void setCameraPosition(const glm::vec3& cameraPosition_);

protected:
	//提供给子类的接口，用于更新Light类中需要更新的内容，这样子类就不需管理
	//这些父类需要更新的内容细节
	virtual void updateShaderProgram();

	glm::vec3 getLightColor() const;

private:
	void updateCameraPosition();

	void updateLightColor();

	void setLightColor(const LightColor& lightColor_);

protected:
	//渲染该光源作用下的物体
	ShaderProgram		_shaderProgram;

private:
	LightColor				_lightColor;
	std::string				_lightColorAttribName;
	glm::vec3				_cameraPosition;
};

#endif