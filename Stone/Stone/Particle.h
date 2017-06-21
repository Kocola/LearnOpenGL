#ifndef					__PARTICLE_H__
#define					__PARTICLE_H__

/*	СЃзг		---	2017.6.20
*/

#include "Common.h"

struct Particle
{
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec4 color;
	float life;

	Particle();
};

#endif