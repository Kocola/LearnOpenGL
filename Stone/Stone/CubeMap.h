#ifndef				__CUBE_MAP_H__
#define				__CUBE_MAP_H__

#include <vector>

#include "Common.h"

class CubeMap
{
public:
	CubeMap();

public:
	void generate(const std::vector<Image> images_);

	void bind() const;

	void release() const;

	void setInternalFormat(GLuint internalFormat_);

	void setImageFormat(GLuint imageFormat_);

	void setDataType(GLuint dataType_);

	GLuint getInternalFormat() const;

	GLuint getCubeMapID() const;

private:
	GLuint					_textureID;

	GLuint					_wrapS;
	GLuint					_wrapT;
	GLuint					_wrapR;
	GLuint					_filterMin;
	GLuint					_filterMag;

	GLuint					_internalFormat;
	GLuint					_imageFormat;

	GLuint					_dataType;
};

#endif