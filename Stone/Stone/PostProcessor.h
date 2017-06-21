#ifndef					__POST_PROCESSOR_H__
#define					__POST_PROCESSOR_H__

/*	���ڴ���		---	2017.6.21
*/

#include "Common.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

class PostProcessor
{
public:
	PostProcessor();

	PostProcessor(int width_, int height_);

private:
	void init();

	void initRenderData();

	void initTexture();

	void initFBO();

	void initShaderProgram();

public:
	void startRender();

	void endRender();

	void render();

public:
	void setIsShake(bool isShake_);

	void setShakeTime(float shakeTime_);

public:
	bool isShake() const;

	float getShakeTime() const;

private:
	int							_width;
	int							_height;

	GLboolean				_isConfuse;
	GLboolean				_isChaos;
	GLboolean				_isShake;

	float							_shakeTime;

	GLuint						_MSFBO;	//�����FBO
	GLuint						_FBO;
	GLuint						_RBO;		//���ڶ������ɫbuffer
	GLuint						_VAO;

	Texture2D					_texture;
	ShaderProgram			_shaderProgram;
};

#endif