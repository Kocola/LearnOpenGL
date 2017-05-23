#ifndef				__SUN_EARTH_MOON_H__
#define				__SUN_EARTH_MOON_H__

#include <vector>

#include "Application.h"
#include "Common.h"
#include "CubeMap.h"
#include "ShaderProgram.h"
#include "Shape.h"

class SunEarthMoon : public Application
{
public:
	SunEarthMoon();

	~SunEarthMoon();

public:
	void draw() override;

private:
	void init();
	
	void initFBO();

	void initShadowMVP();

	void initShaderProgram();

	void initShape();

	void initShapeInfo();

private:
	void firstDraw();

	void secondDraw();

private:
	CubeMap				_cubeMap;
	GLuint					_depthMapFBO;
	GLfloat					_farPlane;
	glm::vec3				_lightPos;
	glm::mat4				_shadowProj;
	std::vector<glm::mat4>				_shadowTransforms;
	ShaderProgram		_depthShaderProgram;
	ShaderProgram		_shadowShaderProgram;

private:
	Light*				_sunLight;
	Shape*				_sun;
	Shape*				_earth;
	Shape*				_moon;
	GLfloat				_earthRotateAngle;
	GLfloat				_earthRotateSpeed;
	GLfloat				_moonRotateAngle;
	GLfloat				_moonRotateSpeed;
	glm::vec3			_earthPosition;	//地球相对太阳的位置
	glm::vec3			_moonPosition;	//月亮相对地球的位置
};

#endif