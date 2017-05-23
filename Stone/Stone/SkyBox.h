#ifndef					__SKY_BOX_H__
#define					__SKY_BOX_H__

#include "Common.h"
#include "CubeMap.h"
#include "ShaderProgram.h"

class SkyBox
{
public:
	SkyBox(const float sideLen_ = 1.0f);

	~SkyBox();

public:
	void draw();

public:
	void setCubeMap(const CubeMap& cubeMap_);

	void setSideLen(const float sideLen_);

private:
	void render();

private:
	RenderInfo	_renderInfo;
	float				_sideLen;
	CubeMap		_cubeMap;
	ShaderProgram	_shaderProgram;
};

#endif