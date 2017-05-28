#ifndef					__NORMAL_MAP_H__
#define					__NORMAL_MAP_H__

#include "Application.h"
#include "Common.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

class Shape;

class NormalMap : public Application
{
public:
	NormalMap();

	~NormalMap();

public:
	virtual void draw() override;

private:
	void init();

private:
	Texture2D				_texture2D;
	Texture2D				_normalTexture2D;
	ShaderProgram		_shaderProgram;
	Shape*					_plane;
};

#endif