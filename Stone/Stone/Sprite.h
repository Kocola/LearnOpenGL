#ifndef					__SPRITE_H__
#define					__SPRITE_H__

/*	����	---	2017.6.19
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

	void initRender();	//��ʼ����Ⱦ

	void initShaderProgram();	//��ʼ����ɫ������

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
	GLuint				_quadVAO;	//�ı��ζ����������
	Texture2D			_texture;
	ShaderProgram	_shaderProgram;

	glm::vec2			_position;	//λ��
	glm::vec2			_size;		//�ߴ�
	float					_rotateAngle;	//��ת�Ƕ�
	glm::vec3			_color;	//����ͼƬ��ɫ
};

#endif