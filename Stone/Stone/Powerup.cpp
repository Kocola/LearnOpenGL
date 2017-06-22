#include "Powerup.h"

const glm::vec2 POWERUP_SIZE(60.0f, 20.f);
const glm::vec2 POWERUP_VELOCITY(0.0f, 150.0f);

Powerup::Powerup(const std::string type_, const glm::vec2& position_, 
	const Texture2D& texture_,
	const glm::vec3& color_ /*= glm::vec3(1.0f)*/,
	GLfloat duration_ /*= 1.0f*/)
	:GameObject(Sprite(), POWERUP_VELOCITY),
	_type(type_), _duration(duration_), _activated(GL_FALSE)
{
	this->setPosition(position_);
	this->setColor(color_);
	this->setContentSize(POWERUP_SIZE);
	this->setTexture(texture_);
}

void Powerup::setTexture(const Texture2D& texture_)
{
	GameObject::setTexture(texture_);
}

void Powerup::setActivated(bool isActivated_)
{
	this->_activated = isActivated_;
}

void Powerup::setDuration(float duration_)
{
	this->_duration = duration_;
}

std::string Powerup::getType() const
{
	return this->_type;
}

bool Powerup::isActivated() const
{
	return this->_activated;
}

float Powerup::getDuration() const
{
	return this->_duration;
}
