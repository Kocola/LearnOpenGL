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

GLenum glCheckError_(const char* file_, int line_)
{
	GLenum errorCode;
	GLboolean hasError = GL_FALSE;
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
		std::cout << error << "	|	" << file_ << "	(" << line_ << ")	" << std::endl;
		hasError = GL_TRUE;
	}
	//存在错误时，输出错误信息后，抛出异常
	if (hasError == GL_TRUE)
	{
		throw 1;
	}
	return errorCode;
}

GLenum checkFrameBufferStatus_(const char* file_, int line_)
{
	GLuint status = GL_FRAMEBUFFER_COMPLETE;
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		std::string errorInfo = "ERROR::TYPE::	";
		switch (status)
		{
		case GL_FRAMEBUFFER_UNDEFINED: errorInfo += "GL_FRAMEBUFFER_UNDEFINED";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: errorInfo += "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: errorInfo += "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
			break;
		case	GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: errorInfo += "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: errorInfo += "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED: errorInfo += "GL_FRAMEBUFFER_UNSUPPORTED";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: errorInfo += "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
			break;
		default:
			errorInfo += "UNKNOWN ERROR TYPE";
		}
		std::cout << errorInfo << std::endl;
		std::cout << file_ << "	(" << line_ << ")	" << std::endl;
	}
	return status;
}

void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, const GLvoid* useParam)
{
	//忽略一些不重要的错误/警告代码
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
	{
		return;
	}

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:							std::cout << "Source: API";		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:	std::cout << "Source: Window System";		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:	std::cout << "Source: Shader Compiler";	break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:			std::cout << "Source: Third Party";		break;
	case GL_DEBUG_SOURCE_APPLICATION:				std::cout << "Source:	 Application";	break;
	case GL_DEBUG_SOURCE_OTHER:						std::cout << "Sourve: Source: Other";		break;
	}
	std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:							std::cout << "Type: Error";		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	std::cout << "Type: Deprecated Bahavior";		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	std::cout << "Type: Undefined Behavior";		break;
	case GL_DEBUG_TYPE_PORTABILITY:					std::cout << "Type: Portability";		break;
	case GL_DEBUG_TYPE_PERFORMANCE:				std::cout << "Type: Performance";		break;
	case GL_DEBUG_TYPE_MARKER:							std::cout << "Type: Marker";		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:					std::cout << "Type: Push Group";		break;
	case GL_DEBUG_TYPE_POP_GROUP:					std::cout << "Type: Pop Group";		break;
	case GL_DEBUG_TYPE_OTHER:							std::cout << "Type: Other";		break;
	}
	std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:						std::cout << "Severity: high";		break;
	case GL_DEBUG_SEVERITY_MEDIUM:					std::cout << "Severity: medium";		break;
	case GL_DEBUG_SEVERITY_LOW:						std::cout << "Severity: low";		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:			std::cout << "Severity: notification";		break;
	}
	std::cout << std::endl;

	std::cout << std::endl;
}
