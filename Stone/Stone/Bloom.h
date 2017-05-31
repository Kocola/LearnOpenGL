#ifndef					__BLOOM_H__
#define					__BLOOM_H__

/* ทบนโ  ----  2017.5.31 */	

#include "Application.h"
#include "Common.h"
#include "PointLight.h"
#include "Texture2D.h"
#include "ShaderProgram.h"

class Shape;

class Bloom : public Application
{
public:
	Bloom();

	~Bloom();

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
	GLuint						_hdrFBO;
	GLuint						_pingpongFBO[2];
	Texture2D					_colorBuffers[2];
	Texture2D					_pingpongBuffers[2];
	Texture2D					_depthTexture;
	ShaderProgram			_hdrShaderProgram;
	ShaderProgram			_bloomShaderProgram;
	PointLight					_pointLight;
	Shape*						_plane;
	Shape*						_cube;
	Shape*						_lightObject;
};

#endif