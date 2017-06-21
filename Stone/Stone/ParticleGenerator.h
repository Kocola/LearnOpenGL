#ifndef					__PARTICLE_GENERATOR_H__
#define					__PARTICLE_GENERATOR_H__

/*	Á£×Ó·¢ÉäÆ÷	---	2017.6.20
*/

#include <vector>

#include "Particle.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

class GameObject;

class ParticleGenerator
{
public:
	ParticleGenerator() = default;

	ParticleGenerator(int amount_);

private:
	void init();

	void initRender();

	void initShaderProgram();

	void initTexture();

	void initParticles();

private:
	int searchFirstUnusedParticle();

	void respawnParticle(Particle& particle_, const GameObject& gameObject_, const glm::vec2& offset);

public:
	void update(float dt_, const GameObject& object, int newParticles_, const glm::vec2& offset_);

	void draw();

private:
	std::vector<Particle>				_particles;
	int										_amount; 
	int										_lastUsedParticleIndex;

	GLuint									_VAO;
	ShaderProgram						_shaderProgram;
	Texture2D								_texture;
};

#endif