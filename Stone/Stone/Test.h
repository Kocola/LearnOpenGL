#ifndef					__TEST_H__
#define					__TEST_H__

#include "Application.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

class Test : public Application
{
public:
	Test();

	virtual void draw();

private:
	void init();

	GLuint generateAttachmentTexture(GLboolean depth_, GLboolean stencil_);

private:
	void firstDraw();

	void secondDraw();

private:
	GLuint _frameBuffer;
	GLuint _rbo;
	GLuint _textureColorBuffer;
	GLuint textures[2];
	Texture2D _depthMap;
	GLuint _depthMapFBO;
	ShaderProgram _simpleDepthShaderProgram;
	ShaderProgram _simpleShadowShaderProgram;
	glm::vec3 _lightPos;
	glm::mat4 _lightSpaceMatrix;
};

#endif