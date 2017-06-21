#include "PostProcessor.h"

#include "ResourceManager.h"
#include "Timer.h"

PostProcessor::PostProcessor()
	: PostProcessor(SCREEN_WIDTH, SCREEN_HEIGHT)
{
}

PostProcessor::PostProcessor(int width_, int height_)
	: _width(width_), _height(height_),
	_isConfuse(GL_FALSE), _isChaos(GL_FALSE), _isShake(GL_FALSE)
{
	init();
}

void PostProcessor::init()
{
	initRenderData();
	initTexture();
	initFBO();
	initShaderProgram();
}

void PostProcessor::initRenderData()
{
	glCheckError();

	GLfloat vertices[] = {
		//位置			//纹理
		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f
	};
	GLuint	VBO;

	glGenVertexArrays(1, &this->_VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(this->_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glCheckError();
}

void PostProcessor::initTexture()
{
	//存放颜色缓冲区
	_texture.setDataType(GL_FLOAT);
	_texture.setImageFormat(GL_RGB);
	_texture.setInternalFormat(GL_RGB16F);
	_texture.generate(_width, _height, nullptr);
}

void PostProcessor::initFBO()
{
	glCheckError();

	glGenFramebuffers(1, &this->_MSFBO);
	glGenFramebuffers(1, &this->_FBO);
	glGenRenderbuffers(1, &this->_RBO);

	//初始化带多重采样颜色缓冲的渲染缓冲存储（不需要深度和模板缓冲）
	glBindFramebuffer(GL_FRAMEBUFFER, this->_MSFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, this->_RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, _width, _height);//采样数为8
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _RBO);
	checkFrameBufferStatus();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//初始化FBO，用于存储渲染缓冲的结果，然后用于着色器操作
	glBindFramebuffer(GL_FRAMEBUFFER, this->_FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		_texture.getTextureID(), 0);
	checkFrameBufferStatus();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCheckError();
}

void PostProcessor::initShaderProgram()
{
	glCheckError();

	_shaderProgram = ResourceManager::getInstance().getShaderProgram("breakoutPostprocess");
	//偏移数组（用于着色器中计算纹理坐标的偏移量）
	GLfloat offset = 1.0f / 300.0f;
	GLfloat offsets[9][2] = {
			{ -offset, offset },		//top-left
			{ 0.0f, offset },		//top-center
			{ offset, offset },		//top-right
			{ -offset, 0.0f },		//center-left
			{ 0.0f, 0.0f },			//center-center
			{ offset, 0.0f },			//center-right
			{ -offset, -offset },	//bottom-left
			{ 0.0f, -offset },		//bottom-center
			{ offset, -offset }		//bottom-right
	};
	for (int i = 0; i < 9; ++i)
	{
		std::string str = std::string("offsets[") + std::to_string(i) + std::string("]");
		_shaderProgram.setUniformValue(str.c_str(), offsets[i][0], offsets[i][1]);
	}
	//边缘检测算子
	GLint edgeKernel[9] = {
		-1, -1, -1,
		-1, 8, -1,
		-1, -1, -1
	};
	_shaderProgram.setUniformValue("edgeKernel", 1, 9, edgeKernel);
	//模糊算子
	GLfloat blurKernel[9] = {
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
		2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
	};
	_shaderProgram.setUniformValue("blurKernel", 1, 9, blurKernel);

	glCheckError();
}

void PostProcessor::startRender()
{
	glCheckError();

	glBindFramebuffer(GL_FRAMEBUFFER, this->_MSFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glCheckError();
}

void PostProcessor::endRender()
{
	glCheckError();

	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->_MSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->_FBO);
	glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	//将读写帧缓冲绑定至默认帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCheckError();
}

void PostProcessor::render()
{
	glCheckError();

	//设置着色器uniform变量值
	static Timer timer;
	_shaderProgram.use();
	_shaderProgram.setUniformValue("time", timer.calcElapsedTime());
	_shaderProgram.setUniformValue("confuse", _isConfuse);
	_shaderProgram.setUniformValue("chaos", _isChaos);
	_shaderProgram.setUniformValue("shake", _isShake);
	//渲染纹理四边形
	_texture.bind();
	glBindVertexArray(this->_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glCheckError();
}

void PostProcessor::setIsShake(bool isShake_)
{
	this->_isShake = isShake_;
}

void PostProcessor::setShakeTime(float shakeTime_)
{
	this->_shakeTime = shakeTime_;
}

bool PostProcessor::isShake() const
{
	return this->_isShake;
}

float PostProcessor::getShakeTime() const
{
	return this->_shakeTime;
}
