#ifndef					__POINT_LIGHT_H__
#define					__POINT_LIGHT_H__

#include "Common.h"
#include "Light.h"

class PointLight : public Light
{
public:
	struct Attenuation
	{
		float constant;
		float linear;
		float quadratic;

		Attenuation(float constant_ = 1.0f, float linear_ = 0.35f, float quadratic_ = 0.44f);
	};

public:
	PointLight(const glm::vec3& cameraPosition_);

public:
	void draw();

public:
	void setAttenuation(const Attenuation& attenuation_);

	void setModelMatrix(const glm::mat4& modelMatrix_);

	void setLightObject(Shape* lightObject_);

protected:
	//����Light���updateShaderProgram��������PointLight����
	//���µ����ݣ����������ظ���ϸ��
	virtual void updateShaderProgram() override;

private:
	void updateAttenuation();

	void updateModelMatrix();

	void updateLightPosition();

protected:
	glm::mat4		_modelMatrix;

private:
	Attenuation	_attenuation;
	std::string		_attenuationName;
	Shape*			_lightObject;

	ShaderProgram		_lightShaderProgram;	//��������ʱʹ��
};

#endif