#include "Sprite.h"

#include "ResourceManager.h"

Sprite::Sprite()
	: Sprite(ResourceManager::getInstance().getTexture2D("white"))
{
}

Sprite::Sprite(const Texture2D& texture_)
	: _texture(texture_)
{
	init();
}

Sprite::~Sprite()
{
}

void Sprite::init()
{
	initData();
	initRender();
	initShaderProgram();
}

void Sprite::initData()
{
	_rotateAngle = 0.0f;
	_position = glm::vec2(0.0f);
	_size = glm::vec2(1.0f, 1.0f);
	_color = glm::vec3(1.0f);
}

void Sprite::initRender()
{
	glCheckError();

	//配置VAO/VBO
	GLuint	VBO;
	GLfloat vertices[] = {
		//位置			//纹理
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &_quadVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(_quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(_quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glCheckError();
}

void Sprite::initShaderProgram()
{
	glCheckError();

	_shaderProgram = ResourceManager::getInstance().getShaderProgram("sprite");
	_shaderProgram.setUniformValue("spriteColor", glm::vec3(1.0f));
	glm::mat4 projection = glm::ortho(0.0f, SCREEN_WIDTH * 1.0f, SCREEN_HEIGHT * 1.0f, 0.0f,
		-1.0f, 1.0f); //(左上角为原点)
	_shaderProgram.setUniformValue("projection", projection);

	glCheckError();
}

void Sprite::setPosition(const glm::vec2& poistion_)
{
	this->_position = poistion_;
}

void Sprite::setContentSize(const glm::vec2& size_)
{
	this->_size = size_;
}

void Sprite::setRotate(const float angle_)
{
	this->_rotateAngle = angle_;
}

void Sprite::setColor(const glm::vec3& color_)
{
	this->_color = color_;
}

void Sprite::setTexture(const Texture2D& texture_)
{
	this->_texture = texture_;
}

glm::vec2 Sprite::getContentSize() const
{
	return this->_size;
}


glm::vec2 Sprite::getPosition() const
{
	return this->_position;
}

void Sprite::draw()
{
	glCheckError();

	//准备变换
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(_position, 0.0f));
	model = glm::translate(model, glm::vec3(0.5f * _size.x, 0.5f * _size.y, 0.0f));
	model = glm::rotate(model, _rotateAngle, glm::vec3(0.0, 0.0, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * _size.x, -0.5f * _size.y, 0.0f));	//?
	model = glm::scale(model, glm::vec3(_size, 1.0f));

	_shaderProgram.use();
	_shaderProgram.setUniformValue("model", model);
	_shaderProgram.setUniformValue("spriteColor", _color);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	_texture.bind();

	glBindVertexArray(this->_quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glDisable(GL_BLEND);

	glCheckError();
}
