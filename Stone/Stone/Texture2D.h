#ifndef					__TEXTURE_2D_H__
#define					__TEXTURE_2D_H__

#include "Common.h"

class Texture2D
{
public:
	Texture2D();

	Texture2D(GLuint texture2D_);

	void generate(GLuint width_, GLuint height_, unsigned char* data_);

	void bind() const;

	void release() const;

	void setInternalFormat(GLuint internalFormat_);

	void setImageFormat(GLuint imageFormat_);

	void setDataType(GLuint dataType_);

	void setWrapType(GLuint wrapType_);

	GLuint getInternalFormat() const;

	GLuint getTextureID() const;

private:
	GLuint					_textureID;

	GLuint					_width;
	GLuint					_height;

	GLuint					_wrapS;
	GLuint					_wrapT;
	GLuint					_filterMin;
	GLuint					_filterMag;

	GLuint					_internalFormat;
	GLuint					_imageFormat;

	GLuint					_dataType;
};

#endif