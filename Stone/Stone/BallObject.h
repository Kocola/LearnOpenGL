#ifndef					__BALL_OBJECT_H__
#define					__BALL_OBJECT_H__

/*	Çò		---	2017.6.20
*/

#include "GameObject.h"
#include "Texture2D.h"

class BallObject : public GameObject
{
public:
	BallObject();

	BallObject(const glm::vec2& pos_, float radius_, const glm::vec2& velocity_, Texture2D texture_);

	~BallObject();

public:
	glm::vec2 move(float dt_, int windowWidth_);

	void reset(const glm::vec2& position_, const glm::vec2& velocity_);

public:
	void setStuck(bool stuck_);

public:
	bool isStuck() const;

	float getRadius() const;

private:
	float				_radius;
	bool				_stuck;
};

#endif