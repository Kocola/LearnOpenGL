#ifndef					__HDR_H__
#define					__HDR_H__

#include "Application.h"
#include "Common.h"
#include "PointLight.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

class Shape;
class Plane;

class HDR	: public Application
{
public:
	HDR();

	virtual ~HDR();

public:
	virtual void draw() override;

private:
	void init();

	void initTexture();

	void initFBO();

	void initShaderProgram();

	void initShape();

private:
	void firstDraw();

	void secondDraw();

private:
	GLuint				_hdrFBO;
	Texture2D			_colorTexture;
	float					_exposure;
	ShaderProgram	_hdrShaderProgram;
	PointLight			_pointLight;
	Shape*				_plane;
	Shape*				_cube;
	Shape*				_lightObject;
};

#endif