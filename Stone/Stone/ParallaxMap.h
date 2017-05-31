#ifndef					__PARALLAX_MAP_H__
#define					__PARALLAX_MAP_H__

/*  ”≤ÓÃ˘Õº	--	2017.5.29	*/

#include "Application.h"
#include "Common.h"
#include "ShaderProgram.h"
#include "Shape.h"
#include "Texture2D.h"

class ParallaxMap	: public Application
{
public:
	ParallaxMap();

	~ParallaxMap();

public:
	virtual void draw() override;

private:
	void init();

private:
	Texture2D				_diffuseTexture;
	Texture2D				_normalTexture;
	Texture2D				_depthTexture;
	ShaderProgram		_shaderProgram;
	Shape*					_plane;
};

#endif