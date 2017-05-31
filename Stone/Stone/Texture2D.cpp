#include "Texture2D.h"

Texture2D::Texture2D()
	: _width(0), _height(0), _internalFormat(GL_RGB), _imageFormat(GL_RGB),
	_dataType(GL_UNSIGNED_BYTE), _wrapS(GL_REPEAT), _wrapT(GL_REPEAT),
	_filterMin(GL_NEAREST), _filterMag(GL_LINEAR)
{
}

Texture2D::Texture2D(GLuint texture2D_)
	: _textureID(texture2D_)
{
}

void Texture2D::generate(GLuint width_, GLuint height_, unsigned char* data_)
{
	glCheckError();

	glGenTextures(1, &this->_textureID);
	this->_width = width_;
	this->_height = height_;
	glBindTexture(GL_TEXTURE_2D, this->_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->_internalFormat, _width, _height,
		0, this->_imageFormat, this->_dataType, data_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->_wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->_wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->_filterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->_filterMag);

	if (this->_wrapS == GL_CLAMP_TO_BORDER)
	{
		GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	glCheckError();
}

void Texture2D::bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->_textureID);
}

void Texture2D::release() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::setInternalFormat(GLuint internalFormat_)
{
	this->_internalFormat = internalFormat_;
}

void Texture2D::setImageFormat(GLuint imageFormat_)
{
	this->_imageFormat = imageFormat_;
}

void Texture2D::setDataType(GLuint dataType_)
{
	this->_dataType = dataType_;
}

void Texture2D::setWrapType(GLuint wrapType_)
{
	this->_wrapS = wrapType_;
	this->_wrapT = wrapType_;
}

GLuint Texture2D::getInternalFormat() const
{
	return _internalFormat;
}

GLuint Texture2D::getTextureID() const
{
	return this->_textureID;
}
