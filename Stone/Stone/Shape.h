#ifndef			__SHAPE_H__
#define			__SHAPE_H__

#include <vector>

#include "Common.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

class Light;

class Shape
{
public:
	Shape(const RenderInfo& renderInfo_);

	virtual ~Shape();

public:
	void draw();

protected:
	void render();

public:
	//平移，旋转和缩放和顺序相关，最好是按照缩放->旋转->平移的顺序进行
	void setTranslate(const glm::vec3& translate_);

	void setRotate(GLfloat angle_, glm::vec3& center_);

	void setScale(const glm::vec3& scale_);

	void setModelMatrix(const glm::mat4& modelMatrix_);

	void setShaderProgram(const ShaderProgram& shaderProgram_);

	void setTexture2D(const Texture2D& texture2D_, const std::string& name_ = "diffuseTexture");

	void setTexture2D(const std::vector<Texture2D>& texture2Ds_, const std::vector<std::string>& _names);

	void setShininess(GLfloat shininess_);

	void setLightSource(Light* light_);

private:
	RenderInfo			_renderInfo;
	ShaderProgram		_shaderProgram;
	Texture2D				_texture2D;
	std::vector<Texture2D>	_texture2Ds;
	std::vector<std::string>	_textureNames;
	GLboolean			_hasMultipleTextures;
	glm::mat4				_modelMatrix;
	GLfloat					_shininess;
};

#endif