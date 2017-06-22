#ifndef					__POWERUP_H__
#define					__POWERUP_H__

/*	µÀ¾ß		---	2017.6.22
*/

#include "GameObject.h"
#include "Texture2D.h"

class Powerup : public GameObject
{
public:
	Powerup(const std::string type_, const glm::vec2& position_,
		const Texture2D& texture_,
		const glm::vec3& color_ = glm::vec3(1.0f),
		GLfloat duration_ = 1.0f);

public:
	void setTexture(const Texture2D& texture_);

	void setActivated(bool isActivated_);

	void setDuration(float duration_);

public:
	std::string getType() const;

	bool isActivated() const;

	float getDuration() const;

private:
	std::string				_type;
	GLfloat					_duration;
	GLboolean			_activated;
};

#endif