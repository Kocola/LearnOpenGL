#include "ParticleGenerator.h"

#include "GameObject.h"
#include "ResourceManager.h"

ParticleGenerator::ParticleGenerator(int amount_)
	: _lastUsedParticleIndex(0), _amount(amount_)
{
	init();
}

void ParticleGenerator::init()
{
	initRender();
	initShaderProgram();
	initTexture();
	initParticles();
}

void ParticleGenerator::initRender()
{
	GLfloat particleQuad[] = {
		//位置					//纹理坐标
		0.0f,		1.0f,			0.0f,		1.0f,
		1.0f,		0.0f,			1.0f,		0.0f,
		0.0f,		0.0f,			0.0f,		0.0f,

		0.0f,		1.0f,			0.0f,		1.0f,
		1.0f,		1.0f,			1.0f,		1.0f,
		1.0f,		0.0f,			1.0,		0.0f
	};

	GLuint	VBO;
	glGenVertexArrays(1, &this->_VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(this->_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleQuad), particleQuad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 4, BUFFER_OFFSET(0));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ParticleGenerator::initShaderProgram()
{
	_shaderProgram = ResourceManager::getInstance().getShaderProgram("particle");
	glm::mat4 projection = glm::ortho(0.0f, SCREEN_WIDTH * 1.0f, SCREEN_HEIGHT * 1.0f, 0.0f, -1.0f, 1.0f);
	_shaderProgram.setUniformValue("projection", projection);
}

void ParticleGenerator::initTexture()
{
	_texture = ResourceManager::getInstance().getTexture2D("particle");
}

void ParticleGenerator::initParticles()
{
	for (int i = 0; i < this->_amount; ++i)
	{
		this->_particles.push_back(Particle());
	}
}

int ParticleGenerator::searchFirstUnusedParticle()
{
	for (int i = _lastUsedParticleIndex; i < _amount; ++i)
	{
		if (_particles[i].life <= 0.0f)
		{
			_lastUsedParticleIndex = i;
			return i;
		}
	}

	//如果没有找到，做线性查找
	for (int i = 0; i < _lastUsedParticleIndex; ++i)
	{
		if (_particles[i].life <= 0.0f)
		{
			_lastUsedParticleIndex = i;
			return i;
		}
	}

	//依然没有找到，覆盖第一个粒子
	_lastUsedParticleIndex = 0;

	return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle_, const GameObject& object_,
	const glm::vec2& offset_)
{
	float random = ((rand() % 100) - 50) / 10.0f;
	float rColor = 0.5 + ((rand() % 100) / 100.0f);
	particle_.position = object_.getPosition() + random + offset_;
	particle_.color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle_.life = 1.0f;
	particle_.velocity = object_.getVelocity() * 0.1f;
}

void ParticleGenerator::update(float dt_, const GameObject& object_, int newParticles_, 
	const glm::vec2& offset_)
{
	//发射粒子
	for (int i = 0; i < newParticles_; ++i)
	{
		int unusedParticleIndex = searchFirstUnusedParticle();
		this->respawnParticle(_particles[unusedParticleIndex], object_, offset_);
	}
	//更新所有的粒子
	for (int i = 0; i < _amount; ++i)
	{
		Particle& particle = this->_particles[i];
		particle.life -= dt_;
		if (particle.life > 0.0f)
		{
			particle.position -= particle.velocity * dt_;
			particle.color.a -= dt_ * 2.5f;
		}
	}
}

void ParticleGenerator::draw()
{
	glCheckError();

	glEnable(GL_BLEND);
	//实现火焰效果
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	_shaderProgram.use();
	for (Particle& particle : _particles)
	{
		if (particle.life > 0.0f)
		{
			_shaderProgram.setUniformValue("offset", particle.position);
			_shaderProgram.setUniformValue("color", particle.color);
			_texture.bind();
			glBindVertexArray(this->_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}

	//恢复混合方程
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glCheckError();
}
