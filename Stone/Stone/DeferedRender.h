#ifndef					__RENDERED_RENDER_H__
#define					__RENDERED_RENDER_H__

#include <vector>

#include "Application.h"
#include "Common.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

class Shape;

class DenderedRender : public Application
{
public:
	DenderedRender();

	~DenderedRender();

private:
	void init();

	void initTexture();

	void initFBO();

	void initShaderProgram();

	void initShape();

public:
	virtual void draw() override;

private:
	void firstDraw();

	void secondDraw();

	void commonSecondDraw();

	void lightVolumnSecondDraw();

	void stencilLightVolumnSecondDraw();

private:
	GLuint				_gBuffer;
	Texture2D			_depthStencilTexture;
	Texture2D			_stencilTexture;
	Texture2D			_positionTexture;
	Texture2D			_normalTexture;
	Texture2D			_albedoSpecTexture;

	ShaderProgram	_geometryPassShaderProgram;
	ShaderProgram	_lightRenderShaderProgram;
	ShaderProgram	_lightRenderShaderProgram2;

	std::vector<Shape*>	_objects;
	std::vector<Shape*>	_lightSources;
	Shape*						_plane;
	std::vector<Shape*> _lightVolumns;

private:
	struct LightInfo
	{
		glm::vec3		position;
		glm::vec3		color;
		float				radius;

		LightInfo(const glm::vec3& position_, float radius_, const glm::vec3& color_ = glm::vec3(1.0f));
	};

	std::vector<LightInfo>			_lightInfos;
};

#endif