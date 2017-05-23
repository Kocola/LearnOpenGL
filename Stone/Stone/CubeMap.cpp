#include "CubeMap.h"

CubeMap::CubeMap()
	:_internalFormat(GL_RGB), _imageFormat(GL_RGB),_dataType(GL_UNSIGNED_BYTE),
	_wrapS(GL_REPEAT), _wrapT(GL_REPEAT), _wrapR(GL_REPEAT),
	_filterMin(GL_NEAREST), _filterMag(GL_LINEAR)
{
}

void CubeMap::generate(const std::vector<Image> images_)
{
	glGenTextures(1, &_textureID);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);
	glCheckError();
	for (GLuint i = 0; i < images_.size(); ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,this->_internalFormat, 
			images_[i].width, images_[i].height, 0, this->_imageFormat, this->_dataType, images_[i].data);
// 		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT32,
// 			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glCheckError();
	}
	glCheckError();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, this->_filterMin);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, this->_filterMag);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, this->_wrapS);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, this->_wrapT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, this->_wrapR);
	glCheckError();
	if (this->_wrapS == GL_CLAMP_TO_BORDER)
	{
		GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	}
	glCheckError();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glCheckError();
}

void CubeMap::bind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);
}

void CubeMap::release() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::setImageFormat(GLuint imageFormat_)
{
	this->_imageFormat = imageFormat_;
}

void CubeMap::setDataType(GLuint dataType_)
{
	this->_dataType = dataType_;
}

void CubeMap::setInternalFormat(GLuint internalFormat_)
{
	this->_internalFormat = internalFormat_;
}

GLuint CubeMap::getInternalFormat() const
{
	return this->_internalFormat;
}

GLuint CubeMap::getCubeMapID() const
{
	return this->_textureID;
}
