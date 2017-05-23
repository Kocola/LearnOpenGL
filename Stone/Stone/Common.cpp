#include "Common.h"

RenderInfo::RenderInfo(GLuint VAO_, GLuint VBO_, GLuint VEO_, GLuint count_)
{
	this->VAO = VAO_;
	this->VBO = VBO_;
	this->VEO = VEO_;
	this->count = count_;
}

Material::Material(const glm::vec3 ambient_ /* = glm::vec3(1.0f) */, const glm::vec3& diffuse_ /* = glm::vec3(1.0f) */,
	const glm::vec3& specular_ /* = glm::vec3(1.0f) */, float shiness_ /* = 32.0f */)
{
	this->ambient = ambient_;
	this->diffuse = diffuse_;
	this->specular = specular_;
	this->shiness = shiness_;
}

Image::Image(int width_, int height_, unsigned char* data_)
	: width(width_), height(height_), data(data_)
{
}

GLenum glCheckError_(const char* file_, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case	GL_INVALID_ENUM:										error = "INVALID_ENUM";				break;
		case GL_INVALID_VALUE:										error = "INVALID_VALUE";				break;
		case GL_INVALID_OPERATION:								error = "INVALID_OPERATION";		break;
		case GL_STACK_OVERFLOW:									error = "STACK_OVERFLOW";			break;
		case GL_STACK_UNDERFLOW:								error = "STACK_UNDERFLOW";		break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:		error = "INVALID_FRAMEBUFFER_OPERATION";	break;
		}
		std::cout << error << "	|	" << file_ << "	(" << line << ")	" << std::endl;
	}
	return errorCode;
}