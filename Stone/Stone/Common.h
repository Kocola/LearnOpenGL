#ifndef					__COMMON_H__
#define					__COMMON_H__

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <GL/GLAUX.H>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL/SOIL.h>

#include <iostream>
#include <string>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

const int SHADOW_WIDTH = 1024;
const int SHADOW_HEIGHT = 1024;

const GLuint CAMERA_UNIFORM_BIND_POINT = 1;

#define BUFFER_OFFSET(offset) (GLvoid*)(offset)

typedef unsigned int IndiceType;

struct MeshFace
{
	using type = unsigned int;

	explicit MeshFace(type first_, type second_, type third_)
		: first(first_), second(second_), third(third_)
	{
	}

	type first;
	type second;
	type third;
};

struct RenderInfo
{
	GLuint VAO;
	GLuint VBO;
	GLuint VEO;
	GLuint count;	//glDrawArrays、glDrawElements渲染顶点或索引数目

	RenderInfo(GLuint VAO_, GLuint VBO_, GLuint VEO_, GLuint count_);
};

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shiness;

	Material(const glm::vec3 ambient_ = glm::vec3(1.0f), const glm::vec3& diffuse_ = glm::vec3(1.0f),
		const glm::vec3& specular_ = glm::vec3(1.0f), float shiness_ = 32.0f);
};

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

struct Image
{
	int width;
	int height;
	unsigned char* data;

	Image() = default;

	Image(int width_, int height_, unsigned char* data_);
};

//检测调用OpenGL函数产生的错误
GLenum glCheckError_(const char* file_, int line_);

#define glCheckError()	glCheckError_(__FILE__, __LINE__)

//检测帧缓冲状态
GLenum checkFrameBufferStatus_(const char* file_, int line_);

#define checkFrameBufferStatus() checkFrameBufferStatus_(__FILE__, __LINE__)

//调试输出
void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* useParam);

#endif