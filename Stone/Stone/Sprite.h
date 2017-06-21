#ifndef					__SPRITE_H__
#define					__SPRITE_H__

/*	精灵	---	2017.6.19
*/

#include <string>

#include "Common.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

class Sprite
{
public:
	Sprite();

	Sprite(const Texture2D& texture_);

	~Sprite();

private:
	void init();

	void initData();

	void initRender();	//初始化渲染

	void initShaderProgram();	//初始化着色器程序

public:
	void setPosition(const glm::vec2& poistion_);

	void setContentSize(const glm::vec2& size_);

	void setRotate(const float angle_);

	void setColor(const glm::vec3& color_);

	void setTexture(const Texture2D& texture_);

public:
	glm::vec2 getContentSize() const;

	glm::vec2 getPosition() const;

public:
	void draw();

private:
	GLuint				_quadVAO;	//四边形顶点数组对象
	Texture2D			_texture;
	ShaderProgram	_shaderProgram;

	glm::vec2			_position;	//位置
	glm::vec2			_size;		//尺寸
	float					_rotateAngle;	//旋转角度
	glm::vec3			_color;	//设置图片颜色
};

#endif