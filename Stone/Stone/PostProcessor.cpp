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
		//λ��			//����
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
	//�����ɫ������
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

	//��ʼ�������ز�����ɫ�������Ⱦ����洢������Ҫ��Ⱥ�ģ�建�壩
	glBindFramebuffer(GL_FRAMEBUFFER, this->_MSFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, this->_RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, _width, _height);//������Ϊ8
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _RBO);
	checkFrameBufferStatus();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//��ʼ��FBO�����ڴ洢��Ⱦ����Ľ����Ȼ��������ɫ������
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
	//ƫ�����飨������ɫ���м������������ƫ������
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
	//��Ե�������
	GLint edgeKernel[9] = {
		-1, -1, -1,
		-1, 8, -1,
		-1, -1, -1
	};
	_shaderProgram.setUniformValue("edgeKernel", 1, 9, edgeKernel);
	//ģ������
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
	//����д֡�������Ĭ��֡����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCheckError();
}

void PostProcessor::render()
{
	glCheckError();

	//������ɫ��uniform����ֵ
	static Timer timer;
	_shaderProgram.use();
	_shaderProgram.setUniformValue("time", timer.calcElapsedTime());
	_shaderProgram.setUniformValue("confuse", _isConfuse);
	_shaderProgram.setUniformValue("chaos", _isChaos);
	_shaderProgram.setUniformValue("shake", _isShake);
	//��Ⱦ�����ı���
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
